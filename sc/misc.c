/*\subsection{sc:misc.c} \label{sc:misc.c}
 * 	Allgemeine Funktionen
\begin{verbatim} */

/*
 * strsave:	Erzeugt dynamisch eine Kopie des "ubergebenen Strings. 
 *
 */

char *
strsave(s)
char *s;
{
  char *new;
  new = (char *) xmalloc(strlen(s) + 1);
  strcpy(new,s);
  return(new);
}
/*\end{verbatim}*/
