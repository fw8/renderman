/*\subsection{AdminHash.h} \label{AdminHash.h}
 *      AdminHash : Verwaltung diverser Hash-Tabellen.
\begin{verbatim}*/

extern void       RmInstallNewShader();
extern RtFunc     RmLookUpShader();

extern void       RmInstallNewTexture();
extern RtPointer  RmLookUpTexture();

extern RtToken    RmDeclareToken();

extern int        RmDeclareTrafo();
extern transform *RmLookUpTrafo();
/*\end{verbatim}*/
