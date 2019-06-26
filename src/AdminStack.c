/*\subsection{AdminStack.c} \label{AdminStack.c}
 * AdminStack : Uebernimmt die Stackverwaltung.
\begin{verbatim}*/

#include "Admin.h"
#include "Misc.h"

/* \end{verbatim} \label{AdminStack.c:RmNewStack} \begin{verbatim}
 NewStack generiert einen neuen UserStack !
 Uebergabe:
	size	: Groesse eines Stackelements
	number	: Anzahl der vorzusehenden Elemente
*/
stackhandle *
RmNewStack(size,number)
	int	size;
	int	number;
{
	stackhandle *handle;

 	handle = (stackhandle *)RmMalloc(sizeof(stackhandle));
	handle->stackbegin = RmMalloc(number*sizeof(char *));	/* Pointer auf neuen Stack */
	handle->sizeofelement = size; 	/* Groesse eines Elements */
	handle->numberofelements = number;	/* Anzahl der Elemente */
	handle->position = 0;		/* Momentane Stackposition (Index) */
	return(handle);	
}

/* \end{verbatim} \label{AdminStack.c:RmFreeStack} \begin{verbatim}
 FreeStack loescht einen mit NewStack angelegten Stackbereich 
*/
void 
RmFreeStack(handle)
	stackhandle	*handle;
{
	RmFree(handle->stackbegin);
    	RmFree(handle);
}

/*\end{verbatim} \label{AdminStack.c:RmPushStack} \begin{verbatim}
 PushStack pushed ein Element auf einen UserStack.
 Uebergabe:
	handle	:	Pointer auf Stackhandle
	element :	Pointer auf Quellelement
*/
int 
RmPushStack(handle,element)
	stackhandle	*handle;
	char		*element;
{
	char		*mem;
	char		**curpos;

	if (handle->position == handle->numberofelements)
		return(1);	/* Fehler */

	curpos = (char **)(handle->stackbegin + handle->position*sizeof(char *));
	if (handle->sizeofelement > sizeof(char *))	/* Auslagerung */
	{
		mem = RmMalloc(handle->sizeofelement);
		RmMemCpy(element,mem,handle->sizeofelement); /* Element kopieren */
		*curpos = mem;
	}
	else	/* Element kleiner als ein Pointer, also keine Auslagerrung */
	{
		RmMemCpy(element,(char *)curpos,handle->sizeofelement);
	}
	handle->position++;	
	return(0);
}

/* \end{verbatim} \label{AdminStack.c:RmPopStack} \begin{verbatim}
 PopStack holt ein Element von einem Userstack.
 Uebergabe:
	handle	:	Pointer auf Stackhandle
	element	:	Pointer auf Zielelement
*/
int 
RmPopStack(handle,element)
	stackhandle	*handle;
	char		*element;
{
	char		*mem;
	char		**curpos;

	if (handle->position == 0)
		return(1);		/* Fehler */

	curpos = (char **)(handle->stackbegin + (handle->position-1)*sizeof(char *));
	if (handle->sizeofelement > sizeof(char *))     /* Auslagerung */
        {
                mem = *curpos;
                RmMemCpy(mem,element,handle->sizeofelement); /* Element kopieren *
/
		xfree(mem);	/* BUffer wieder freigeben */
                *curpos = 0;	/* und Eintrag loeschen */
        }
        else    /* Element kleiner als ein Pointer, also keine Auslagerrung */
        {
                RmMemCpy((char *)curpos,element,handle->sizeofelement);
        }
        handle->position--;
	return(0);
}
/*\end{verbatim}*/
