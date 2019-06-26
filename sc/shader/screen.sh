/*\subsection{screen.sh} \label{screen.sh}
Dieser Shader erzeugt ein Metallgitter auf einer Oberfl"ache.
\begin{verbatim}*/

surface
screen(
	float Ks	= .5,
	      Kd	= .5,
	      Ka	= .1,
	      roughness = .1,
	      density   = .25,
	      frequency = 20;
	color specularcolor = color (1,1,1))
{
	point Nf=faceforward(normalize(N),I);

	if(mod(u*frequency,1) < density || mod(v*frequency,1) < density)
		Oi=1.0;
	else
		Oi=0.0;

	Ci=Oi*(Cs*(Ka*ambient()+Kd*diffuse(Nf)) +
               specularcolor*Ks*specular(Nf,normalize(I),roughness));
}
/*\end{verbatim}*/
