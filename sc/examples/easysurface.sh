/*\subsection{easysurface.sh} \label{easysurface.sh}
Dieser Shader ben"otigt keine Lichtquelle.
\begin{verbatim}*/

surface
easysurface(
	float Kd = .8,
	      Ka = .2,
	      falloff=2.0)
{
  float d;

  d=I.N/(I.I*N.N);
  d=pow(d,falloff);

  Ci = Cs * (Ka + Kd * d);
}
/*\end{verbatim}*/
