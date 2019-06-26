/*  \subsection{sc.c} \label{sc.c}
 *	RenderMan Shading Language
 *	main(), ccp()
\begin{verbatim}*/

/*
 *      cpp() -- lex-Eingabe mit C-Preprozessor bearbeiten
 */

#include <stdio.h>

#ifndef CPP		/* Default-Dateiname fuer cpp */
#	define CPP	"/lib/cpp"
#endif

static int
cpp(argc,argv)
        int argc;
        char **argv;
{
        char **argp, *cmd;
	extern FILE *yyin;	/* input() in lex */
	extern FILE *popen();
	int i;

	for (i=0, argp = argv; *++argp;)
		if (**argp == '-' && index("CDEIPU",(*argp)[1]))
			i += strlen(*argp)+1;

	if (! (cmd = (char *) calloc(i+sizeof CPP, sizeof(char))))
		return -1; /* kein Platz */

	strcpy(cmd,CPP);
	for (argp=argv;*++argp;)
		if (**argp == '-' && index("CDEIPU",(*argp)[1]))
			strcat(cmd," "), strcat(cmd,*argp);

	if (yyin = popen(cmd,"r")) i=0; /* alles OK */
	else i = -1;			/* kein cpp */
	cfree(cmd);
	return i;
}


/*
 *	usage(name) -- Hilfestellung bei fehlerhaftem Aufruf 
 */

static usage(name)
	register char *name;
{
	fputs("usage: ",stderr);
	fputs(name,stderr);
	fputs(" [-CDEIPU] [file]\n",stderr);
	exit(1);
}


/*
 *	main() -- Aufruf von cpp(), dann yyparse() dann Terminierung 
 */

main(argc,argv)
	int argc;
	char **argv;
{
	char **argp;
	int cppflag = 1 /*0*/ ;

	for (argp=argv; *++argp && **argp == '-';)
		switch ((*argp)[1]) {
		default:
			usage(argv[0]);
		case 'C':
		case 'D':
		case 'E':
		case 'I':
		case 'P':
		case 'U':
			cppflag = 1;
		}
	if (argp[0] && argp[1])
		usage(argv[0]);
	if (*argp && !freopen(*argp, "r", stdin))
		perror(*argp), exit(1);
	if (cppflag && cpp(argc,argv))
		perror("cpp"), exit(1);
	init();
	exit(yyparse());
}
/*\end{verbatim}*/
