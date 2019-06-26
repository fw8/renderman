/*\subsection{plastic.sh} \label{plastic.sh}
\begin{verbatim}*/

surface
plastic(
	float	Ks = .5,
		Kd = .5,
		Ka = 1,
		roughness = .1;
	color   specularcolor = 1)
{
	point Nf = faceforward(normalize(N),I);
        point V = normalize(-I);

        Ci= texture("test.rle"[0],s,t);
	Oi=Os;
	Ci=Os*(Cs*(Ka*ambient()+Kd*diffuse(Nf))+
            specularcolor*Ks*specular(Nf,V,roughness));
}
/*\end{verbatim}*/
