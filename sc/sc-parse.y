/*  \subsection{sc-parse.y} \label{sc-parse.y}
 *	RenderMan Shading Language
 *	Syntaxanalyse
 *	(s/r Konflikt bei ELSE)
\begin{verbatim} */

%{
#include "tree.h"

extern int yynerrs;     /* gesammte Fehlerzahl */
static enum types current_type; /* aktueller typ fuer nachfolgende dekl. */
static enum types more_params[1] = {More};
static int        arg_nr;	/* Argument-Zaehler fuer Fkt.-Argumente */
static enum types *arg_list;	/* aktuelle Liste der Fkt.-Argumente */
static struct symtab *func_result; /* aktueller Fkt.-Rueckgabetyp und name */
%}

%union 	{struct symtab *y_sym; /* Identifier */
	 char          *y_str; /* Constant */
         enum types     y_typ; /* Type */
         tree           y_tree; /* Syntax-Tree */
	}

/* Eingabesymbole */

%token <y_sym> Identifier
%token <y_str> Constant
%token <y_str> StrConstant
%token FLOAT
%token POINT
%token COLOR
%token STRING
%token LIGHT
%token SURFACE
%token ATHMOSPHERE
%token INTERIOR
%token EXTERIOR
%token ILLUMINANCE
%token ILLUMINATE
%token NOISE
%token TEXTURE
%token IF
%token ELSE
%token WHILE
%token FOR
%token BREAK
%token CONTINUE
%token RETURN
%token ';'
%token '('
%token ')'
%token '{'
%token '}'
%token '+'
%token '-'
%token '*'
%token '/'
%token '%'
%token '>'
%token '<'
%token GE	/* >= */
%token LE	/* <= */
%token EQ	/* == */
%token NE	/* != */
%token AND 	/* && */
%token OR	/* || */
%token '^'	/* kreuzprodukt */
%token '.'	/* skalarprodukt */
%token '='
%token PE	/* += */
%token ME	/* -= */
%token TE	/* *= */
%token DE	/* /= */
%token RE	/* %= */
%token PP	/* ++ */
%token MM 	/* -- */
%token ','

/*
 *	Syntaxbegriffe mit Typ
 */

%type <y_typ> type 
%type <y_str> optional_conversion 
%type <y_tree> init assignment expression bool_expr function constant
%type <y_tree> parameter_list parameter_declaration parameter_declarator_list
%type <y_tree> parameter_with_init compound_statement declarations declaration
%type <y_tree> declarator_list identifier_with_optional_init 
%type <y_tree> statements statement procedure noise texture 
%type <y_tree> append_argument_list optional_argument_list argument_list chan

/*
 * Praezedenztabelle
 */

%right '=' PE ME TE DE RE
%left  OR 
%left  AND
%left  EQ NE
%left  '<' '>' GE LE
%left  '+' '-'
%left  '*' '/' '%'
%left  '.'
%left  '^'
%right PP MM
%right UNARY

%%

shader
	: shader_type shader_definition

shader_type
	: LIGHT        { fatal("only surface shader supported");}
	| SURFACE
	| ATHMOSPHERE  { fatal("only surface shader supported");}
	| INTERIOR     { fatal("only surface shader supported");}
	| EXTERIOR     { fatal("only surface shader supported");}

shader_definition
	: Identifier '(' parameter_list ')'
	  '{' declarations statements '}'     
                        { if(yynerrs) exit(1);
                          GenCode($1->name,$3,$6,$7);}

parameter_list
	: parameter_declaration
	| parameter_declaration ';' parameter_list 
                                { $$=append_nodelist($1,$3); }
	| /* keine Parameter */ { $$=NULL_TREE; }

parameter_declaration
	: type { current_type = $1; } 
          parameter_declarator_list { $$=$3; } 

parameter_declarator_list
        : parameter_with_init
        | parameter_with_init ',' parameter_declarator_list
                                { $$=append_nodelist($1,$3); }

type
	: FLOAT { $$ = Float; }
	| POINT { $$ = Point; }
	| COLOR { $$ = Color; }
	| STRING { $$ = String; }

optional_conversion
        : StrConstant /* e.g."world","rgb" */
        | /* nothing  */ { $$=(char *)0; }

constant
	: type optional_conversion '(' Constant ',' Constant ',' Constant ')'
		     {$$=build_vector_const_node(current_type,$2,$4,$6,$8);}
	| Constant   {$$=build_const_node($1);}
	| StrConstant {$$=build_string_const_node($1);}

init
	: expression {$$=$1;}

parameter_with_init
	: Identifier {if (isndefined($1)) {
                        $1->type = current_type;
                        $1->access = Param;  /* this is an parameter */
                      }else error("redeclaration of formal parameter, %s",
                                                              $1->name);
                     }
          '=' init   { $$=build_decl_node($1,$4); }


compound_statement
	: '{'
	  statements 
	  '}'              {  $$=$2; }

declarations
	: declaration declarations      { $$=append_nodelist($1,$2); }
	| /* keine lokalen Variablen */ { $$=NULL_TREE; }

declaration
	: type {current_type = $1; }
          declarator_list ';' { $$=$3; }

declarator_list
	: identifier_with_optional_init
	| identifier_with_optional_init ',' declarator_list
                                { $$=append_nodelist($1,$3); }

identifier_with_optional_init
	: Identifier /* ohne init */
		{if (isndefined($1)) $1->type = current_type;
                      else error("redeclaration of , %s",$1->name);
                 $$=build_decl_node($1,NULL_TREE);
                }
	| Identifier /* mit init */
		{if (isndefined($1)) $1->type = current_type;
                      else error("redeclaration of , %s",$1->name);
		}
	  '=' init { $$=build_decl_node($1,$4); }

statements
        : statement statements { if ($1) $$=append_nodelist($1,$2);
                                 else $$ = $2; }
	| /* keine */          { $$=NULL_TREE; }

statement
	: assignment ';' 	{$$=$1;}
	| procedure ';' 	{$$=$1;}
	| BREAK ';' 		{$$=build_node(BREAK_NODE);}
	| CONTINUE ';' 		{$$=build_node(CONTINUE_NODE);}
	| RETURN ';' 		{$$=build_node(RETURN_NODE);}
	| compound_statement    {$$=$1;}
	| IF '(' bool_expr ')' statement { $$=build_if_node($3,$5,NULL_TREE); }
	| IF '(' bool_expr ')' statement ELSE statement
                                        { $$=build_if_node($3,$5,$7); }
	| WHILE '(' bool_expr ')' statement
                                        { $$=build_while_node($3,$5); }
	| FOR '(' assignment ';' bool_expr ';' assignment ')' statement
                                        { $$=build_for_node($3,$5,$7,$9); }
	| ';'  /* leere Anweisung */    { $$=NULL_TREE;}

assignment
	: Identifier '=' {if (isdefined($1)) ;
                          else error("%s undefined",$1->name);
                         } 
          expression     {$$=build_assign_node($1,$4);}
/*
        | PP Identifier 
        | MM Identifier
*/

procedure
	: Identifier '('                {if ($1->access != Func ||
                                             $1->type != Void)
					    error("%s is not a procedure",
                                                                $1->name);
                                         push(); 
                                         arg_list=$1->params; arg_nr=0; }
          optional_argument_list ')'    {$$=build_func_node($1,$4); pop(); }

noise
        : NOISE '('                     {push();
                                         arg_list=more_params; arg_nr=0; }
	  optional_argument_list ')'    {$$=build_noise_node(Float,$4); pop(); }
        | type NOISE '('                {if ($1 != Point && $1 != Color)
                                            error("bad noise-type qualifier");
                                         push();
                                         arg_list=more_params; arg_nr=0; }
          optional_argument_list ')'    {$$=build_noise_node($1,$5); pop(); }


chan
	: '[' expression ']'		{if (typeof($2) != Float)
						error("not a channel number");
					 $$=$2;}
	|				{$$=build_const_node("0");}

texture
	: TEXTURE '(' expression chan	{if (typeof($3) != String)
                                            error("1st argument of texture must be of type string");
					
	  				 push();
                                         arg_list=more_params; arg_nr=0; }
	  append_argument_list ')'      {$$=build_texture_node(Color,$3,$4,$6); 
                                         pop();}
	| type TEXTURE '(' expression chan {if ($1 != Color && $1 != Float)
                                           error("bad texture-type qualifier");
                                         if (typeof($4) != String)
                                            error("1st argument of texture must be of type string");
                                         push();
                                         arg_list=more_params; arg_nr=0; }
          append_argument_list ')'      {$$=build_texture_node($1,$4,$5,$7);
                                         pop();}

                                           
                                         
function
	: Identifier '('                {if ($1->access != Func || 
                                             $1->type == Void)
					    error("%s is not a function",
                                                                $1->name);
                                         push();
                                         arg_list=$1->params; arg_nr=0;}
          optional_argument_list ')'     {$$=build_func_node($1,$4); pop(); }

expression
	: Identifier {if (isdefined($1)) $$=build_ident_node($1);
		      else error("%s undefined",$1->name);
		     }

	| constant { $$ = $1;}

        | '-' expression %prec UNARY
                                {$$=build_expr_node(OP_MINUS,$2,NULL_TREE); }
	| '(' expression ')' {$$ = $2;}

	| function { $$ = $1;}

	| noise {$$ = $1;}

	| texture {$$ = $1;}

/*
	| PP Identifier 
	| MM Identifier
*/
/* zweistellige operatoren */
expression
	: expression '+' expression { $$=build_expr_node(OP_ADD,$1,$3); }
	| expression '-' expression { $$=build_expr_node(OP_SUB,$1,$3); }
	| expression '*' expression { $$=build_expr_node(OP_MUL,$1,$3); }
	| expression '/' expression { $$=build_expr_node(OP_DIV,$1,$3); }
	| expression '.' expression { $$=build_expr_node(OP_DOT,$1,$3); }
	| expression '^' expression { $$=build_expr_node(OP_CROSS,$1,$3); }

bool_expr
	: '(' bool_expr ')' {$$ = $2;}
	| expression '>' expression { $$=build_expr_node(OP_GT,$1,$3); }
        | expression '<' expression { $$=build_expr_node(OP_LT,$1,$3); }
        | expression GE expression  { $$=build_expr_node(OP_GE,$1,$3); }
        | expression LE expression  { $$=build_expr_node(OP_LE,$1,$3); }
        | expression EQ expression  { $$=build_expr_node(OP_EQ,$1,$3); }
        | expression NE expression  { $$=build_expr_node(OP_NE,$1,$3); }
        | bool_expr AND bool_expr     { $$=build_expr_node(OP_AND,$1,$3); }
        | bool_expr OR bool_expr      { $$=build_expr_node(OP_OR,$1,$3); }

append_argument_list
	: ',' argument_list	{$$=$2;}
	| /* nothing */         {$$=NULL_TREE; }

optional_argument_list
	: argument_list 
	| /* keine Argumente */ { if (arg_list[arg_nr] != Undef)
				    error("too less args in function");
                                  $$=NULL_TREE; }

argument_list
	: expression { if (typeof($1) != arg_list[arg_nr])
			  if (arg_list[arg_nr] == Undef)
			    error("too much args in function");
			  else if (arg_list[arg_nr] != More)
			    error("wrong parameter type in function");
                          else arg_nr--;
                       arg_nr++;
                       $$=$1;
                     }
	| argument_list ',' 
	  expression { if (typeof($3) != arg_list[arg_nr])
                          if (arg_list[arg_nr] == Undef)
                            error("too much args in function");
                          else if (arg_list[arg_nr] != More)
                            error("wrong parameter type in function");
                          else arg_nr--;
                       arg_nr++;
                       $$=append_nodelist($1,$3);
                     }



%%

#define MAXSTACKSIZE 20

static struct {
		int arg_nr;
		enum types *arg_list;
		struct symtab *func_result;
	      } stack[MAXSTACKSIZE];

static int stack_pointer = 0;

push()
{
	if (stack_pointer>=MAXSTACKSIZE) fatal("function stack overflow");
	stack[stack_pointer].arg_nr = arg_nr;
	stack[stack_pointer].arg_list = arg_list;
	stack[stack_pointer].func_result = func_result;
	stack_pointer++;
}  

pop()
{
	if (stack_pointer<=0) fatal("function stack underflow"); 
	stack_pointer--;
	arg_nr = stack[stack_pointer].arg_nr;
	arg_list = stack[stack_pointer].arg_list;
	func_result = stack[stack_pointer].func_result;
}

info(s,x) 
char *s;struct symtab *x;
{
   printf("%s: *(%x)=(%s,",s,x,x->name);
   switch(x->type) {
     case Float: printf("Float"); break;
     case Point: printf("Point\n"); break;
     case Color: printf("Color\n"); break;
     case String: printf("String\n"); break;
     default: printf("<%d>",x->type); break;
   }
   printf(")\n");
}
/*\end{verbatim}*/
