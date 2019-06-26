/*\subsection{bumptrace.sh} \label{bumptrace.sh}
Der Bumptrace-Shader. Erzeugt wie "`Bump"' eine gewellte
Oberfl"ache, jedoch spiegelnd.
\begin{verbatim}*/

#include "fBm.h"

surface
bumptrace(
	float offset	= 0,
	      scale	= 1,
	      h		= .5,
	      lambda	= 2.,
	      octaves	= 6,
	      Ka	= .2,
	      Kd	= .4,
	      Ks	= .9,
	      roughness = .1)
{
	point Nf = faceforward(normalize(N),I);
	point V  = normalize(-I);
        float beta = 1+2*h, omega = pow(lambda,-.5*beta);
	float i,o,l;
	point VfBm;

	VFBM(P,omega,lambda,octaves);

	Nf = Nf + offset + scale * VfBm;
        Nf = normalize(Nf);
        
	Oi = 1; /* total opaque */
	Ci = Cs * (Ka*ambient()+Kd*diffuse(Nf)) 
                 + Ks*(specular(Nf,V,roughness)+trace(P,reflect(I,Nf)));
}
/*\end{verbatim}*/
