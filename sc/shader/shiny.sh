/*\subsection{shiny.sh} \label{shiny.sh}
Im Prinzip ein "`metal"'-Shader, aber mit Spiegeleffekt.
\begin{verbatim}*/

surface
shiny(
        float Ka        = .3,
              Ks        = .8,
              roughness = .05)
{
        color tr;
        point D,Nf,NI;

        Nf = faceforward(normalize(N),I);
        NI = normalize(I);

        D = reflect(NI,Nf);
        tr = trace(P,D);

        Oi = Os;
        Ci = Oi * (Ka * ambient() +
                Ks * (tr + specular(Nf,-NI,roughness)));
}
/*\end{verbatim}*/
