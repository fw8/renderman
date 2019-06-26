/* \subsection{Misc.c} \label{Misc.c}
 * Misc : Allerlei Routinen zum allgemeinen Gebrauch.
\begin{verbatim}*/

#include <math.h>
#include "RiTypes.h"

/* \end{verbatim} \label{Misc.c:RmRnd} \begin{verbatim}
 * make a random number ranged between 0 and (x)
 */
RtFloat
RmRnd(x)
 RtFloat x;
{
  return ((rand() * x) / 2147483647.); /* 2^31 - 1 */
}

/* \end{verbatim} \label{Misc.c:RmStrSave} \begin{verbatim}
 * make a private copy of a string...
 *
 */
char *
RmStrSave(s)
char *s;
{
  char *new;
  new = (char *) RmMalloc(strlen(s) + 1);
  strcpy(new,s);
  return(new);
}

/* \end{verbatim} \label{Misc.c:RmMemCpy} \begin{verbatim}
 * memcpy : make a raw byte copy n bytes from s1 to s2, returns s2.
 */
char *
RmMemCpy(s1,s2,n)
char *s1;
char *s2;
int  n;
{
  char *s=s2;
  while (n--) *s2++ = *s1++;

  return s;
}


/* \end{verbatim} \label{Misc.c:RmMemClr} \begin{verbatim}
 * memclr : clear memory at loction s and length n, returns s.
 */
char *
RmMemClr(s,n)
char *s;
int  n;
{
  char *t=s;

  while(n--) *s++ = '\0';

  return t;
}
/* \end{verbatim}*/
