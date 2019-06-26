/*\subsection{wood2.sh} \label{wood2.sh}
Noch ein Holz-Shader.
\begin{verbatim}*/

#include "fBm.h"

surface
wood2(
	float Ka	= .2,
	      Kd	= .4,
	      Ks	= .6,
	      roughness = .1)
{
	point Nf = faceforward(normalize(N),I);
	float i,o,l;
	float chaos,brownLayer, greenLayer, red, grn, blu;
	float perturb, brownPerturb, greenPerturb, grnPerturb;
	color r = color (1,0,0),
	      g = color (0,1,0),
	      b = color (0,0,1); 

        CHAOS(P,7);
	l=sin(sin(8*chaos+7*xcomp(P)+3*ycomp(P)));
	greenLayer = abs(l); brownLayer = greenLayer;

	perturb = abs(sin(40*chaos+50*zcomp(P)));

	brownPerturb = .6*perturb + 0.3;
	greenPerturb = .2*perturb + 0.8;
	grnPerturb = .15*perturb + 0.85;
	grn = 0.5 * pow(abs(brownLayer), 0.3);
	brownLayer = pow(0.5 * (brownLayer+1.0), 0.6) * brownPerturb;
	greenLayer = pow(0.5 * (greenLayer+1.0), 0.6) * greenPerturb;
	red = (0.5*brownLayer + 0.35*greenLayer)*2.*grn;
	blu = (0.25*brownLayer + 0.35*greenLayer)*2.0*grn;
	grn = grn * max(brownLayer, greenLayer) * grnPerturb;
	Ci = r*red + g*grn + b*blu;
	Oi = 1; /* total opaque */
	Ci = Ci * (Ka*ambient()+Kd*diffuse(Nf)) 
                 + Ks*specular(Nf,normalize(-I),roughness);
}
/*\end{verbatim}*/
