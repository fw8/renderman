/*\subsection{metal.sh} \label{metal.sh}
\begin{verbatim}*/

surface
metal(
	float	Ks = 1,
		Ka = 1,
		roughness = .25)
{
	point Nf = faceforward(normalize(N),I);

	Oi=Os;
	Ci=Os*Cs*(Ka*ambient()+Ks*specular(Nf,-I,roughness));
}
/*\end{verbatim}*/
