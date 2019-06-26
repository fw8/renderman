/* \subsection{MathVector.c} \label{MathVector.c}
 * Was im .h File leider nicht machbar war
\begin{verbatim}*/
#include "MathVector.h"

RtFloat
RmUnitV3(A,R) 
RtPoint A,R;
{
	RtFloat f1=lenV3(A);
	RtFloat f2=1./f1;
	
	mulV3(A,f2,R);
	return f1;
}
/*\end{verbatim}*/
