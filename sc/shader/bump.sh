/*\subsection{bump.sh} \label{bump.sh}
Der Bump-Shader. Er erzeugt durch verbiegen der Normalen 
den Eindruck einer gewellten Oberfl"ache.
\begin{verbatim}*/

#include "fBm.h"

surface
bump(
	float offset	= 0,
	      scale	= 1,
	      h		= .5,
	      lambda	= 2.,
	      octaves	= 6,
	      Ka	= .2,
	      Kd	= .4,
	      Ks	= .6,
	      roughness = .1)
{
	point Nf = faceforward(normalize(N),I);
        float beta = 1+2*h, omega = pow(lambda,-.5*beta);
	float i,o,l;
	point VfBm;

	VFBM(P,omega,lambda,octaves);

	Nf = Nf + offset + scale * VfBm;
        Nf = normalize(Nf);
	Oi = 1; /* total opaque */
	Ci = Cs * (Ka*ambient()+Kd*diffuse(Nf)) 
                 + Ks*specular(Nf,normalize(-I),roughness);
}
/*\end{verbatim}*/
