/*\subsection{message.c} \label{message.c}
 *	Meldungen
\begin{verbatim} */

#include <stdio.h>

extern FILE *yyerfp;

message(str,arg)
	char *str;
{
	yywhere();
	fprintf(yyerfp,str,arg);
	putc('\n',yyerfp);
}

error(str,arg)
        char *str;
{        
	extern int yynerrs;

	fprintf(yyerfp,"[error %d] ",++yynerrs);
	message(str,arg);
}

warning(str,arg)
        char *str;
{        
	fputs("[warning] ",yyerfp);
	message(str,arg);
}

fatal(str,arg)
        char *str;
{        
	fputs("[fatal error] ",yyerfp);
	message(str,arg);
	fputs("*** (abort)\n",yyerfp);
	fflush(yyerfp);
	exit(1);
}

internal(str,arg)
        char *str;
{        
	fputs("[internal error] ",yyerfp);
	message(str,arg);
	fputs("wait for core dump\n",yyerfp);
	fflush(yyerfp);
	abort(); /* force core dump for debugging */
}
/*\end{verbatim}*/
