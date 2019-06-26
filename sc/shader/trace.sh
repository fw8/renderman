/*\subsection{trace.sh} \label{trace.sh}
Dieser Shader erzeugt eine perfekt spiegelnde Oberfl"ache.
\begin{verbatim}*/

surface
trace(
	float Ks	= 1,
	      roughness = .05)
{
	point Nf = faceforward(normalize(N),I);

	Oi = 1;
	Ci = Ks*(trace(P,reflect(I,Nf)));
}
/*\end{verbatim}*/
