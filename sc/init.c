/* \subsection{init.c} \label{init.c}
 *      RenderMan Shading Language -- Initialisierung 
\begin{verbatim} */

#include "symtab.h"

/*
 * Hier werden alle ben"otigten Arten von Parameterlisten erzeugt.
 * Eine Parameterliste beschreibt die erwarteten Parametertypen bei
 * einem Funktionsaufruf.
 */
enum types 	no_params[1] = {Undef},
		one_float[2] = {Float,Undef},
		two_floats[3] = {Float,Float,Undef},
		three_floats[4] = {Float,Float,Float,Undef},
		one_color[2] = {Color,Undef},
		two_colors_one_float[4] = {Color,Color,Float,Undef},
		one_point[3] = {Point,Undef},
		two_points[3] = {Point,Point,Undef},
		two_points_one_float[4] = {Point,Point,Float,Undef},
		one_point_one_float[3] = {Point,Float,Undef},
		one_string_and_more[2] = {String,More};
                more_params[1] = {More};

static struct { /* Eingebaute Functionen */
	enum types type;  /* Rueckgabewert der Funktion */
	char *name;       /* Name der Funktion */
        enum types *list; /* Parametertypen der Funktion */
	} builtin[]={ 	
                        {Float,"sin",one_float},
                        {Float,"cos",one_float},
                        {Float,"tan",one_float},
                        {Float,"radians",one_float},
                        {Float,"degrees",one_float},
                        {Float,"sqrt",one_float},
                        {Float,"pow",two_floats},
                        {Float,"mod",two_floats},
                        {Float,"abs",one_float},
                        {Float,"min",two_floats},
                        {Float,"max",two_floats},
                        {Float,"floor",one_float},
                        {Float,"step",two_floats},
                        {Float,"smoothstep",three_floats},

                        {Color,"ambient",no_params},
                        {Color,"diffuse",one_point},
                        {Color,"specular",two_points_one_float},
                        {Color,"trace",two_points},

                        {Float,"xcomp",one_point},
                        {Float,"ycomp",one_point},
                        {Float,"zcomp",one_point},
                        {Void,"setxcomp",one_point_one_float},
                        {Void,"setycomp",one_point_one_float},
                        {Void,"setzcomp",one_point_one_float},
                        {Float,"length",one_point},
                        {Float,"distance",two_points},
                        {Point,"normalize",one_point},
                        {Point,"faceforward",two_points},
                        {Point,"reflect",two_points},

			{Color,"mix",two_colors_one_float},

			{Void,"printf",one_string_and_more},

			{Undef,"",no_params} /* last entry */
		    };


static struct { /* Ein/Ausgabe Variablen */
	enum types type;    /* Typ der Variable */
        char *name;	    /* Name der Variable */
	enum access access; /* Zugriffsrechte auf die Variable */ 
	} globals[]={	
			{Color,"Cs",In},
			{Color,"Os",In},
			{Point,"P",In},
			{Point,"N",In},
			{Point,"Ng",In},
			{Float,"u",In},
			{Float,"v",In},
			{Float,"s",In},
			{Float,"t",In},
			{Color,"L",In},
			{Color,"CI",In},
			{Point,"I",In},
			{Float,"PI",In},
			{Color,"Ci",Out},
			{Color,"Oi",Out},

			{Undef,"",In}
		    };


/*
 * init: Hier wird die Symboltabelle initialisiert und mit den Symbolen
 *		der eingebauten Variablen und Funktionen gef"ullt.
 */
init()
{
	extern struct symtab *s_create();
	struct symtab *symbol;
	int i;

	s_init(); /* Initialisieren der Symboltabelle */

	/* Generierung der Symbole f"ur die eingebauten Funktionen. */

	for (i=0; builtin[i].type != Undef; i++) {
		symbol=s_create(builtin[i].name);	
		symbol->type = builtin[i].type;
		symbol->access = Func;
		symbol->params = builtin[i].list;
	}

        /* Generierung der Symbole f"ur die globalen Variablen. */

	for (i=0; globals[i].type != Undef; i++) {
		symbol=s_create(globals[i].name);
		symbol->type = globals[i].type;
		symbol->access = globals[i].access;
	}		
}
/*\end{verbatim}*/
