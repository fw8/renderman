/* \subsection{memory.c} \label{memory.c}
 *  Abgesicherte Speicherbelegungs Funktionen.
\begin{verbatim} */

void *
xmalloc(size) unsigned size; {

   extern void *malloc();
          void *p;

   if (!(p=malloc(size))) fatal("memory exhausted"); 
   return(p);
}

void
xfree(p) void *p; {
   free(p);
}
/*\end{verbatim}*/
