/* \subsection{symtab.c} \label{symtab.c}
 *	RenderMan Shading Language -- Symboltabellen-Manager
\begin{verbatim} */

#include "tree.h"
#include "tokens.h"
#include "hash.h"

/*
 *	Symboltabelle
 */

static HTAB symtab[HASHSIZE];
 
struct symtab *
s_create(name)
	register char *name;
{
	register struct symtab *new_entry = 
                        (struct symtab *)xmalloc(sizeof(struct symtab));

	new_entry->name = strsave(name);
	new_entry->type = Undef;
	new_entry->access = Local;
	new_entry->params = (enum types *)0;
	enter_hash(name,new_entry,symtab);
	return new_entry;
}

s_lookup(yylex)
	int yylex;	/* Constant oder Identifier */
{
	extern char yytext[];	/* Darstellung des Symbols */

	switch(yylex) {
		case Constant:
			yylval.y_str = strsave(yytext);
			break;
		case StrConstant:
			yylval.y_str = strsave(yytext);
			break;
		case Identifier:
			if (yylval.y_sym =
                           (struct symtab *) lkup_hash(yytext,symtab)) break;
			yylval.y_sym = s_create(yytext);
			break;
		default:
			internal("<s_lookup>");
	}
} 

s_init()
{
	clr_hash(symtab);
}			
/*\end{verbatim}*/
