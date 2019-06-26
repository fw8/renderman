/* \subsection{AdminStack.h} \label{AdminStack.h}
 * AdminStack : Uebernimmt die Stackverwaltung.
\begin{verbatim}*/

#ifndef _AdminStack_
#define _AdminStack_

typedef struct {
        int     sizeofelement;
        int     numberofelements;
        int     position;
        char    *stackbegin;
} stackhandle;

extern stackhandle *RmNewStack();
extern void RmFreeStack();
extern int RmPushStack();
extern int RmPopStack();

#endif _AdminStack_
/*\end{verbatim}*/
