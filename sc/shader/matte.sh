/*\subsection{matte.sh} \label{matte.sh}
\begin{verbatim}*/

surface
matte2(
	float	Ka = 1,
		Kd = 1)
{
	point Nf = faceforward(normalize(N),I);

	Oi=Os;
	Ci=Os*Cs*(Ka*ambient()+Kd*diffuse(Nf));
}
/*\end{verbatim}*/
