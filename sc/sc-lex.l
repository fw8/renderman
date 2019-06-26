%{
/*\subsection{sc-lex.l} \label{sc-lex.l}
 *	RenderMan Shading Language
 *	Lexikalische Analyse
\begin{verbatim} */

#ifdef DEBUG   /* erzeuge Testversion - falls 'assert' es erlaubt */

#	include <assert.h>

	main()
	{	char *p;

		assert(sizeof(int) >= sizeof(char *));

		while (p=(char *) yylex())
			printf("%-10.10s is \"%s\"\n",p,yytext);
	}

	s_lookup() {}
	yymark() {}
	int yynerrs = 0;

#	define token(x)	(int) "x"

#else ! DEBUG	/* Version fuer Compiler */

#	include "tree.h"
#	include "tokens.h"
#	define token(x)	x

	extern s_lookup();

#endif DEBUG

#define END(v) (v-1 + sizeof v / sizeof v[0])
static int screen();
%}


letter		[a-zA-Z_]
digit		[0-9]
digits		([0-9]+)
pt		"."
sign		[+-]?
exponent	([eE]{sign}{digits})
letter_or_digit [a-zA-Z_0-9]
white_space	[ \t\n]
blank		[ \t]
other		.


%%



^"#"{blank}*{digit}+({blank}+.*)?\n	yymark();

">="	return token(GE);
"<="	return token(LE);
"=="	return token(EQ);
"!="	return token(NE);
"+="	return token(PE);
"-="	return token(ME);
"*="	return token(TE);
"/="	return token(DE);
"%="	return token(RE);
"++"	return token(PP);
"--"	return token(MM);
"&&"	return token(AND);
"||"	return token(OR);

{letter}{letter_or_digit}*	return screen();

{digits}({pt}{digits}?)?{exponent}? |
{digits}?{pt}{digits}{exponent}?	{ s_lookup(token(Constant));
					  return token(Constant);
					}
\".*\"					{ s_lookup(token(StrConstant));
					  return token(StrConstant);
					}
{white_space}+			;

{other}				return token(yytext[0]);


%%


/*
 *	Extraktion der reservierten Worte
 */

static struct rwtable {		/* Tabelle */
	char *rw_name;		/* Eingabetext */
	int rw_yylex;		/* yylex() Wert */
	} rwtable[] = {		/* muss sortiert sein ! */
	"athmosphere",	token(ATHMOSPHERE),
	"break",	token(BREAK),
	"color",	token(COLOR),
	"continue",	token(CONTINUE),
	"else",		token(ELSE),
	"exterior",	token(EXTERIOR),
	"float",	token(FLOAT),
	"for",		token(FOR),
	"if",		token(IF),
	"illuminance",	token(ILLUMINANCE),
	"illuminate",	token(ILLUMINATE),
	"interior",	token(INTERIOR),
	"light",	token(LIGHT),
	"noise",	token(NOISE),
	"point",	token(POINT),
	"return",	token(RETURN),
	"string",	token(STRING),
	"surface",	token(SURFACE),
	"texture",	token(TEXTURE),
	"while",	token(WHILE) 
	};


 
 
static int 
screen()
{	struct rwtable *low = rwtable,
	               *high = END(rwtable),
	               *mid;
	int c;

	while (low <= high) {
		mid = low + (high-low)/2;
		if ((c=strcmp(mid->rw_name,yytext)) == 0)
			return mid->rw_yylex;
		else if (c < 0)
			low = mid+1;
		else
			high = mid-1;
	}
	s_lookup(token(Identifier));
	return token(Identifier);
} 
/*\end{verbatim}*/
