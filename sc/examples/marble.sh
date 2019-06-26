/*\subsection{marble.sh} \label{marble.sh}
Dieser Shader erzeugt eine marmorartige Oberfl"ache.
\begin{verbatim}*/

#include "fBm.h"

surface
marble(
	float Ka	= .2,
	      Kd	= .4,
	      Ks	= .6,
	      roughness = .1)
{
	point Nf = faceforward(normalize(N),I);
	float i,o,l;
	float chaos,marble;

	MARBLE(P);

	Oi = 1; /* total opaque */
	Ci = marble * Cs * (Ka*ambient()+Kd*diffuse(Nf)) 
                 + Ks*specular(Nf,normalize(-I),roughness);
}
/*\end{verbatim}*/
