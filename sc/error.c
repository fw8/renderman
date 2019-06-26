/* \subsection{error.c} \label{error.c}
 *      yywhere() -- Eingabeposition fuer yyparse()
 *      yymark() -- '\# Zeile Datei' speichern
\begin{verbatim} */

#include <stdio.h>

FILE *yyerfp = stderr;	/* Fehlermeldungen */

extern char yytext[];   /* aktuelles Symbol */
extern int yyleng;      /* und Laenge */
extern int yylineno;    /* aktuelle Eingabezeile */

static char *source;    /* aktuelle Eingabedatei */


yywhere()               /* Positionsmeldung */
{
  char colon = 0; /* Trenner notwendig ? */

  if (source && *source && strcmp(source,"\"\"")) {
    char *cp = source;
    int len = strlen(source);

    if (*cp == '"') ++cp, len -= 2;
    if (strcmp(cp,"./",2) == 0) cp += 2, len -= 2;
    fprintf(yyerfp,"\"%.*s",len,cp);
    colon = 1;
  }

  if (yylineno > 0) {
    if (colon) fputs(", ",yyerfp);
    fprintf(yyerfp,"line %d",
            yylineno+1 - (*yytext == '\n' || *yytext));       
    colon = 1;
  }

  if (*yytext) {
    register int i;

    for (i=0;i<20;++i) if (!yytext[i] || yytext[i] == '\n') break;
                
    if (i) {
      if (colon) fputc(' ',yyerfp);
      fprintf(yyerfp,"near \"%.*s\"",i,yytext);
      colon = 1;
    }
  }

  if (colon) fputs(": ",yyerfp);
}

yymark()
{
  extern void *xmalloc();
  extern void xfree();

  if (source) xfree(source);
  source = (char *) xmalloc(yyleng+1);
  sscanf(yytext,"# %d %s",&yylineno,source);
}

yyerror(s)
	register char *s;
{
	extern int yynerrs;	/* gesammte Fehlerzahl */

	fprintf(yyerfp,"[error %d] ",yynerrs+1);
	yywhere();
	fputs(s,yyerfp);
	fputc('\n',yyerfp);
}
/*\end{verbatim}*/
