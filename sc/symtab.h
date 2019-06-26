/*\subsection{symtab.h} \label{symtab.h}
Definitionsdatei zur Symboltabelle
\begin{verbatim} */

enum types {Undef,Float,Point,Color,String,Bool,Void,More,Error};
enum access {Local,Param,In,Out,InOut,Func};

struct symtab {
	char *name;		/* Name */
	enum types type;	/* Typ des Symbols */
	enum access access;	/* Zugriffsart */
	enum types *params;	/* Typen der Parameter */
	};

#define isdefined(s) (((s)->type != Undef) ? 1 : 0)
#define isndefined(s) (((s)->type == Undef) ? 1 : 0)
#define NEW(s) s = (struct symtab *)xmalloc(sizeof(struct symtab))

extern int is_global();

/*
 *	Typvereinbarungen fuer Buechereifunctionen
 */

char *strsave();	/* Zeichenkette dynamisch speichern */
void *xmalloc();	/* dynamisch Speicher beschaffen */
/*\end{verbatim}*/
