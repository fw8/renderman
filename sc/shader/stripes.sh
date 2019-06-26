/*\subsection{stripes.sh} \label{stripes.sh}
 * Der Stripes-Shader.
\begin{verbatim}*/
surface
stripes(
	float	Ka = .2,
	        Kd = .8,
	        freq = 20)
{
  float x,diff = I.N/(I.I*N.N);

  diff = diff * diff;
  x = xcomp(P);

  if (mod(x*freq,1)<.5)
    Ci=Ka+Kd*diff;
  else
    Ci=0;
}
/*\end{verbatim}*/
