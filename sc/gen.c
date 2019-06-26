/* \subsection{gen.c} \label{gen.c}
 *      RenderMan Shading Language
 *      Codegenerierung.
\begin{verbatim} */

#include <stdio.h>
#include "tree.h"

#define EQ(a,b) (!strcmp((a),(b)))
#define EQ_TYP(a,b) ((a)->type == (b)->type)
#define NEQ_TYP(a,b) ((a)->type != (b)->type)

static FILE *codefp = stdout;

/* MAX?REGS bestimmt die Anzahl der zur Verf"ugung stehenden Tempor"aren
 * Variablen (Register). '?' = [Point|Color|Float] */

#define MAXPREGS 8
#define MAXCREGS 8
#define MAXFREGS 8

/* Die folgenden Array's representieren die Tempor"aren Variablen */

static struct symtab Pregs[MAXPREGS] = {
                       	{"$p0",Undef},
		 	{"$p1",Undef},
			{"$p2",Undef},
			{"$p3",Undef},
			{"$p4",Undef},
			{"$p5",Undef},
			{"$p6",Undef},
			{"$p7",Undef}
				};

static struct symtab Cregs[MAXCREGS] = {
                        {"$c0",Undef},
                        {"$c1",Undef},
                        {"$c2",Undef},
                        {"$c3",Undef},
                        {"$c4",Undef},
                        {"$c5",Undef},
                        {"$c6",Undef},
                        {"$c7",Undef}
                                };

static struct symtab Fregs[MAXFREGS] = {
			{"$f0",Undef},
                        {"$f1",Undef},
                        {"$f2",Undef},
                        {"$f3",Undef},
                        {"$f4",Undef},
                        {"$f5",Undef},
                        {"$f6",Undef},
                        {"$f7",Undef}
				};

 
/* gen_Ptmp: Belegt eine Tempor"are Variable vom Typ Point und liefert
 * 		einen Pointer auf diese.
 */
struct symtab *
gen_Ptmp()
{
	int i;
        for (i=0;i<MAXPREGS;i++)
                if (Pregs[i].type == Undef) {
			Pregs[i].type = Point;
                        return &Pregs[i];
                }
        fatal("expression too complex");
}

/* gen_Ctmp: Belegt eine Tempor"are Variable vom Typ Color und liefert
 *              einen Pointer auf diese.
 */
struct symtab *
gen_Ctmp()
{
        int i;
        for (i=0;i<MAXCREGS;i++)
                if (Cregs[i].type == Undef) {
                        Cregs[i].type = Color;
                        return &Cregs[i];
                }
        fatal("expression too complex");
}

/* gen_Ftmp: Belegt eine Tempor"are Variable vom Typ Float und liefert
 *              einen Pointer auf diese.
 */
struct symtab *
gen_Ftmp()
{
	int i;
        for (i=0;i<MAXFREGS;i++) 
                if (Fregs[i].type == Undef) {
			Fregs[i].type = Float;
                        return &Fregs[i];
                }
        fatal("expression too complex");
}

/* free_tmp: Gibt die Tempor"are Variable 'r' wieder frei.
 */
free_tmp(r)
	struct symtab *r;
{
	if (r->name[0] == '$') 
		switch (r->name[1])  {
		  case 's' : break; /* lokal user defined var. */
		  case 'p' : Pregs[r->name[2]-'0'].type = Undef; break;
		  case 'c' : Cregs[r->name[2]-'0'].type = Undef; break;
		  case 'f' : Fregs[r->name[2]-'0'].type = Undef; break;
		  default  : internal("<free_tmp> case error"); break; 
		}
}

/*
 * emit: Ausgabe eines Strings mit 0-4 Argumenten auf codefp
 */
emit(str,arg1,arg2,arg3,arg4)
	char *str;
{
	fprintf(codefp,str,arg1,arg2,arg3,arg4);
	putc('\n',codefp);
}

/*
 * gen_function_head: Generiert Startcode f"ur einen Funktionsaufruf.
 * 	Input: Symbol f"ur die zu generierende Funktion.
 *	Output: Symbol der tempor"aren Variable in der das Funktionsergebni\ss
 *		abgelegt wird.
 */
struct symtab *
gen_function_head(a)
	struct symtab *a;
{
        struct symtab *tmp;

	if (a->access != Func) internal("<gen_function_head> unknown Function");

	/* Manche Funktionen fallen aus dem allgemeinen Schema heraus
	 * und m"u\ssen deshalb gesondert behandelt werden. */

        if (EQ(a->name,"ambient")) {
          tmp=gen_Ctmp();
          emit("Rm_ambient(lightset");
          return(tmp);
        }else if(EQ(a->name,"diffuse") || EQ(a->name,"specular")) {
          tmp=gen_Ctmp();
          emit("Rm_%s(lightset,iob->P,",a->name);
          return(tmp);
        }else if(EQ(a->name,"trace")) {
	  tmp=gen_Ctmp();
	  emit("Rm_trace(level,");
	  return(tmp);
	}

	/* Bei der allgemeinen Behandlung wird nur nach R"uckgabetyp 
	 * unterschieden. */

	switch(a->type) {
	  case Float:
	    tmp=gen_Ftmp();
	    emit("%s=Rm_%s(",tmp->name,a->name);
	    return(tmp);
	  case Point:
	    tmp=gen_Ptmp();
	    emit("Rm_%s(",a->name);
	    return(tmp);
	  case Color:
	    tmp=gen_Ctmp();
            emit("Rm_%s(",a->name);
	    return(tmp);
          case Void:
            emit("Rm_%s(",a->name);
            return(0);
	  default: internal("<gen_function_head> case error"); break;
	}
}

/*
 * gen_function_tail: Generiert den abschlie\ssenden Code f"ur einen
 *		Funktionsaufruf.
 *	Input: Symbol der beim Aufruf von gen_function_head() erhaltenen
 *		tempor"aren Variablen.	
 *	Output: siehe Input.
 */
struct symtab *
gen_function_tail(tmp)
	struct symtab *tmp;
{
	if (!tmp) emit(")");  /* Prozedur ohne R"uckgabwert */

	else if (tmp->type == Float) emit("),");

	/* Bei Funktionen mit nicht-skalarem R"uckgabwert wird dieser
	 * als letzter Parameter der Funktion "ubergeben.  */

	else emit(",%s),",tmp->name);
	return(tmp);
}

/*
 * gen1: Generiert Code f"ur einstellige Operatoren.
 *	Input: Opcode des Operators und Symbol des Operanden.
 *	Output: Symbol der tempor"aren Variable in der das Ergebniss
 *		der Operation abgelegt wird.
 */
struct symtab *
gen1(operand,a)
        enum opcode operand;
        struct symtab *a;
{
        struct symtab *tmp;

        if (isndefined(a))
                internal("<gen1> %s undefined",a->name);

        if (a->type == String) internal("<gen1> invalid type");

	switch(operand) {
	  case OP_PP: /* ++a */
	    if (a->type == Float) emit("++%s",a->name); /* komma problem !!*/
            else internal("<gen1.OP_PP> incompatible type");
	    break;
	  case OP_MM: /* --a */
            if (a->type == Float) emit("--%s",a->name); /* komma problem !!*/
            else internal("<gen1.OP_MM> incompatible type");
	    break;
	  case OP_MINUS: /* tmp = -a */
            switch(a->type) {
              case Float:
                tmp=gen_Ftmp();
                emit("%s= -%s,",tmp->name,a->name);
                return(tmp);
              case Point:
                tmp=gen_Ptmp();
                emit("mulVf(%s,-1.,%s),",a->name,tmp->name);
                return(tmp);
              case Color:
		tmp=gen_Ctmp();
                emit("mulVf(%s,-1.,%s),",a->name,tmp->name);
                return(tmp); 
              default: internal("<gen1> unknown type '%d'",a->type); 
            }
          default: internal("<gen1> unknown opcode '%d'",operand); break;
        }
}

/*
 * gen2: Generiert Code f"ur zweistellige Operatoren.
 *      Input: Opcode des Operators und Symbol des ersten und zweiten Operanden.
 *      Output: Symbol der tempor"aren Variable in der das Ergebniss der
 *              Operation abgelegt wird.
 */
struct symtab *
gen2(operand,a,b)
	enum opcode operand;
	struct symtab *a, *b;
{
  struct symtab *tmp,*tmp2;

  if (isndefined(a))
    internal("<gen2> %s undefined",a->name);
  if (isndefined(b))
    internal("<gen2> %s undefined",b->name);

  switch(operand) {

    case OP_ASSIGN: /* tmp = a = b */
      switch(a->type) { 
        case String:
        case Float : emit("%s=%s",a->name,b->name); break;
        case Point :
        case Color : switch(b->type) {
                       case Color:
                       case Point: emit("copyV(%s,%s)",b->name,a->name); break;
                       case Float: emit("setV(%s,%s,%s,%s)",b->name,b->name,
                                                            b->name,a->name);
                                   break;
                       default: internal("<gen2.OP_ASSIGN> case error");
                     } break;
        default: internal("<gen2.OP_ASSIGN> case error");
      }
      return a;
      break;

    case OP_ADD: /* tmp = a + b */
      switch(a->type) {

        case Float: switch(b->type) {           

             case Float:
               tmp=gen_Ftmp();
               emit("%s=%s+%s,",tmp->name,a->name,b->name);
               return(tmp);
             case Point:
               tmp=gen_Ptmp();
               emit("addVf(%s,%s,%s),",b->name,a->name,tmp->name);
               return(tmp);
             case Color:
               tmp=gen_Ctmp();
               emit("addVf(%s,%s,%s),",b->name,a->name,tmp->name);
	       return(tmp);
             default: internal("<gen2.OP_ADD> unknown type '%d'",b->type);
          }

        case Point: switch(b->type) {
             case Float:
               tmp=gen_Ptmp();
               emit("addVf(%s,%s,%s),",a->name,b->name,tmp->name);
               return(tmp);
            case Point:
               tmp=gen_Ptmp();
               emit("addVV(%s,%s,%s),",a->name,b->name,tmp->name);
               return(tmp);
            default: internal("<gen2.OP_ADD> incompatible types");
          }

        case Color: switch(b->type) {
             case Float:
               tmp=gen_Ctmp();
               emit("addVf(%s,%s,%s),",a->name,b->name,tmp->name);
               return(tmp);
            case Color:
               tmp=gen_Ctmp();
               emit("addVV(%s,%s,%s),",a->name,b->name,tmp->name);
               return(tmp);
            default: internal("<gen2.OP_ADD> incompatible types"); 
          }

        default: internal("<gen2.OP_ADD> unknown type '%d'",a->type);
      }
      break;

    case OP_SUB: /* tmp = a - b */
      switch(a->type) {

        case Float: switch(b->type) {

             case Float:
               tmp=gen_Ftmp();
               emit("%s=%s-%s,",tmp->name,a->name,b->name);
               return(tmp);
             case Point:
               tmp=gen_Ptmp();
               emit("subVf(%s,%s,%s),",b->name,a->name,tmp->name);
               return(tmp);
             case Color:
               tmp=gen_Ctmp();
               emit("subVf(%s,%s,%s),",b->name,a->name,tmp->name);
               return(tmp);
             default: internal("<gen2.OP_SUB> unknown type '%d'",b->type);
          }

        case Point: switch(b->type) {
             case Float:
               tmp=gen_Ptmp();
               emit("subVf(%s,%s,%s),",a->name,b->name,tmp->name);
               return(tmp);
            case Point:
               tmp=gen_Ptmp();
               emit("subVV(%s,%s,%s),",a->name,b->name,tmp->name);
               return(tmp);
            default: internal("<gen2.OP_SUB> incompatible types");
          }

        case Color: switch(b->type) {
             case Float:
               tmp=gen_Ctmp();
               emit("subVf(%s,%s,%s),",a->name,b->name,tmp->name);
               return(tmp);
            case Color:
               tmp=gen_Ctmp();
               emit("subVV(%s,%s,%s),",a->name,b->name,tmp->name);
               return(tmp);
            default: internal("<gen2.OP_SUB> incompatible types");
          }

        default: internal("<gen2.OP_SUB> unknown type '%d'",a->type);
      }
      break;

    case OP_MUL: /* tmp = a * b */
      switch(a->type) {

        case Float: switch(b->type) {

             case Float:
               tmp=gen_Ftmp();
               emit("%s=%s*%s,",tmp->name,a->name,b->name);
               return(tmp);
             case Point:
               tmp=gen_Ptmp();
               emit("mulVf(%s,%s,%s),",b->name,a->name,tmp->name);
               return(tmp);
             case Color:
               tmp=gen_Ctmp();
               emit("mulVf(%s,%s,%s),",b->name,a->name,tmp->name);
               return(tmp);
             default: internal("<gen2.OP_MUL> unknown type '%d'",b->type);
          }

        case Point: switch(b->type) {
             case Float:
               tmp=gen_Ptmp();
               emit("mulVf(%s,%s,%s),",a->name,b->name,tmp->name);
               return(tmp);
            case Point:
               tmp=gen_Ptmp();
               emit("mulVV(%s,%s,%s),",a->name,b->name,tmp->name);
               return(tmp);
            default: internal("<gen2.OP_MUL> incompatible types");
          }

        case Color: switch(b->type) {
             case Float:
               tmp=gen_Ctmp();
               emit("mulVf(%s,%s,%s),",a->name,b->name,tmp->name);
               return(tmp);
            case Color:
               tmp=gen_Ctmp();
               emit("mulVV(%s,%s,%s),",a->name,b->name,tmp->name);
               return(tmp);
            default: internal("<gen2.OP_MUL> incompatible types");
          }

        default: internal("<gen2.OP_MUL> unknown type '%d'",a->type);
      }
      break;

    case OP_DIV: /* tmp = a / b */
      switch(a->type) {

        case Float: switch(b->type) {

             case Float:
               tmp=gen_Ftmp();
               emit("%s=%s/%s,",tmp->name,a->name,b->name);
               return(tmp);
             case Point:
               tmp=gen_Ptmp();
               emit("divVf(%s,%s,%s),",b->name,a->name,tmp->name);
               return(tmp);
             case Color:
               tmp=gen_Ctmp();
               emit("divVf(%s,%s,%s),",b->name,a->name,tmp->name);
               return(tmp);
             default: internal("<gen2.OP_DIV> unknown type '%d'",b->type);
          }

        case Point: switch(b->type) {
             case Float:
               tmp=gen_Ptmp();
               emit("divVf(%s,%s,%s),",a->name,b->name,tmp->name);
               return(tmp);
            case Point:
               tmp=gen_Ptmp();
               emit("divVV(%s,%s,%s),",a->name,b->name,tmp->name);
               return(tmp);
            default: internal("<gen2.OP_DIV> incompatible types");
          }

        case Color: switch(b->type) {
             case Float:
               tmp=gen_Ctmp();
               emit("divVf(%s,%s,%s),",a->name,b->name,tmp->name);
               return(tmp);
            case Color:
               tmp=gen_Ctmp();
               emit("divVV(%s,%s,%s),",a->name,b->name,tmp->name);
               return(tmp);
            default: internal("<gen2.OP_DIV> incompatible types");
          }

        default: internal("<gen2.OP_DIV> unknown type '%d'",a->type);
      }
      break;

    case OP_DOT:
      if (a->type != Point  ||  b->type != Point)
         internal("<gen2.OP_DOT> unexpected type");
      tmp=gen_Ftmp();
      emit("%s=dotVV(%s,%s),",tmp->name,a->name,b->name);
      return(tmp);
      break;

    case OP_CROSS: internal("<gen2.OP_CROSS> still not implemented");
      break;

    case OP_EQ: /* a == b */

      if (NEQ_TYP(a,b)) internal("<gen2.OP_EQ> wrong types");
      switch(a->type) {

        case Float:
           tmp=gen_Ftmp();
           emit("%s=(%s==%s),",tmp->name,a->name,b->name);
           return(tmp);
        case String:
           tmp=gen_Ftmp();
           emit("%s=!strcmp(%s,%s),",tmp->name,a->name,b->name);
           return(tmp);
        default: internal("<gen2.OP_EQ> wrong types");
      }
      break;

    case OP_NE: /* a != b */

      if (NEQ_TYP(a,b)) internal("<gen2.OP_NE> wrong types");
      switch(a->type) {

        case Float:
           tmp=gen_Ftmp();
           emit("%s=(%s!=%s),",tmp->name,a->name,b->name);
           return(tmp);
        case String:
           tmp=gen_Ftmp();
           emit("%s=strcmp(%s,%s),",tmp->name,a->name,b->name);
           return(tmp);
        default: internal("<gen2.OP_NE> wrong types");
      }
      break;

    case OP_LT: /* a < b */

      if (NEQ_TYP(a,b)) internal("<gen2.OP_LT> wrong types");
      switch(a->type) {

        case Float:
           tmp=gen_Ftmp();
           emit("%s=(%s<%s),",tmp->name,a->name,b->name);
           return(tmp);
        default: internal("<gen2.OP_LT> wrong types");
      }
      break;

    case OP_LE: /* a <= b */

      if (NEQ_TYP(a,b)) internal("<gen2.OP_LE> wrong types");
      switch(a->type) {

        case Float:
           tmp=gen_Ftmp();
           emit("%s=(%s<=%s),",tmp->name,a->name,b->name);
           return(tmp);
        default: internal("<gen2.OP_LE> wrong types");
      }
      break;

    case OP_GT: /* a > b */

      if (NEQ_TYP(a,b)) error("<gen2.OP_GT> wrong types");
      switch(a->type) {

        case Float:
           tmp=gen_Ftmp();
           emit("%s=(%s>%s),",tmp->name,a->name,b->name);
           return(tmp);
        default: error("<gen2.OP_GT> wrong types");
      }
      break;

    case OP_GE: /* a >= b */

      if (NEQ_TYP(a,b)) error("<gen2.OP_GE> wrong types");
      switch(a->type) {

        case Float:
           tmp=gen_Ftmp();
           emit("%s=(%s>=%s),",tmp->name,a->name,b->name);
           return(tmp);
        default: error("<gen2.OP_GE> wrong types");
      }
      break;

    case OP_OR: /* a || b */
      if (a->type != Float  ||  b->type != Float)
         internal("<gen2.OP_OR> unexpected type");

      tmp=gen_Ftmp();
      emit("%s=(%s||%s),",tmp->name,a->name,b->name);
      return(tmp);
      break;

    case OP_AND: /* a && b */
      if (a->type != Float  ||  b->type != Float)
         internal("<gen2.OP_AND> unexpected type");

      tmp=gen_Ftmp();
      emit("%s=(%s&&%s),",tmp->name,a->name,b->name);
      return(tmp);
      break;

    default: internal("<gen2> unknown opcode '%d'",operand); break;
  }
}

/*
 * gen_expr: Generiert Code f"ur einen Ausdruck.
 *	Input: Toplevel-Knoten des Ausdrucks.
 *	Output: Symbol der Tempor"aren Variable in der das Resultat
 *		abgelegt wird.
 */
struct symtab *
gen_expr(node)
tree node;
{
  struct symtab *sym;
  char buf[80];

  /* Zuerst wird gepr"uft ob der Knoten ein Blatt ist. Wenn ja dann wird
   * dieses entsprechend ausgewertet. */

  switch(node->tag) {
    case CONST_NODE:

                  /* F"ur skalare Konstanten wird ein neues Symbol erzeugt
                   * und die Konstante als String im Namen des Symbols
                   * abgelegt. Hierbei wird kein Code erzeugt.
                   * Im Falle einer nicht skalaren Konstante wird eine
                   * t"ampor"are Variable erzeugt und mit den Werten der
                   * Konstante initialisiert. */

                  switch(node->u.Const.type) {
                    case Float : NEW(sym);
                                 sprintf(buf,"%e",node->u.Const.u.scalar);
                                 sym->name=strsave(buf);
                                 sym->type=Float;
                                 return sym;
                                 break;
                    case String: NEW(sym);
                                 sprintf(buf,"%s",node->u.Const.u.string);
                                 sym->name=strsave(buf);
                                 sym->type=String;
                                 return sym;
                                 break;
                    case Color : sym=gen_Ctmp();
				/* Eigentlich sollte hier emit(...) stehen
				 * aber aus irgendwelchen obskuren Gr"unden
				 * funktioniert die "ubergabe einer variablen
                                 * Anzahl von Parametern nur mit String-
				 * paramentern. Also fprintf(...) */
                                 fprintf(codefp,"setV(%e,%e,%e,%s),\n",
                                                node->u.Const.u.vector[0],
                                                node->u.Const.u.vector[1],
                                                node->u.Const.u.vector[2],
                                                sym->name);
                                 return sym;
                                 break;
                    case Point : sym=gen_Ptmp();
                                 fprintf(codefp,"setV(%e,%e,%e,%s),\n",
                                                node->u.Const.u.vector[0],
                                                node->u.Const.u.vector[1],
                                                node->u.Const.u.vector[2],
                                                sym->name);
                                 return sym;
                                 break;
                    default: internal("<gen_expr.CONST_NODE> case error");
                  }
                  break;

    case IDENT_NODE:

                  /* Eine Variable kann entweder Lokal (vom Benutzer angelegt)
                   * Parameter oder Global (I/O-Variable) sein.
                   * Lokale Variablen werden unver"andert "ubernommen.
                   * Parameter bekommen den Pr"afix `$s->' da sie im
                   * Instanzenblock eines Shaders angelegt werden.
                   * Globale Variablen bekommen den  Pr"afix `iob->', da
                   * sie im I/O-Block an den Shader "ubergeben werden. */
                  
                  if (node->u.Ident.sym->access != Local) {
                     NEW(sym);
                     if (node->u.Ident.sym->access == Param)
                       sprintf(buf,"$s->%s",node->u.Ident.sym->name);
                     else /*  global symbol */
                       sprintf(buf,"iob->%s",node->u.Ident.sym->name);
                     sym->name=strsave(buf);
                     sym->type=node->u.Ident.sym->type;
                     sym->access=node->u.Ident.sym->access;
                     return sym;
                  } 
                  return node->u.Ident.sym;
                  break;

    case FUNC_NODE:

                 /* Bei Einer Funktion wird erst der Funktionskopf generiert.
                  * Anschlie\ssend werden die Argumente nach einander
                  * durch rekursiven Aufruf von gen_expr() abgearbeitet.
                  * Der Aufruf von gen_function_tail() schlie\sst den Rumpf
                  * der Funktion ab. */

                {
                  struct symtab *func_result,*arg_sym;
                  tree arg;

                  func_result=gen_function_head(node->u.Func.sym);
                  arg=node->u.Func.args;
                  while(arg) {
                     emit("(");
                     arg_sym=gen_expr(arg);
                     emit("%s)",arg_sym->name);
                     free_tmp(arg_sym);
                     arg=arg->nxt;
                     if (arg) emit(",");
                  }
                  gen_function_tail(func_result);

                  return func_result;
                } break;

    case NOISE_NODE: 

		  /* Die generische Funktion Noise ben"otigt eine 
		   * Sonderbehandlung da sie je nach Typ und Anzahl der
                   * Parameter sowie vorangestelltem Qualifier eine
		   * andere Auspr"agung hat. Ansonsten jedoch wie FUNC_NODE */

                {
                  struct symtab *func_result,*arg_sym,*func_sym;
                  tree arg=node->u.Noise.args;

                  NEW(func_sym);
                  func_sym->access = Func;
                  if (!arg) fatal("no args in noise() call");
                  if (typeof(arg) == Point && !(arg->nxt)) {
                     switch(node->u.Noise.return_type) {
				/* einfache 3-D Noise-Funktion */
                       case Float: func_sym->name="noisefV";
                                   func_sym->type=Float;
                                   break;

				/* dreifache 3-D Noise-Funktion */
                       case Point:
                       case Color: func_sym->name="noiseVV";
                                   func_sym->type=node->u.Noise.return_type;
                                   break;
                       default: internal("<gen_expr.NOISE_NODE> case error");
                     }
                  }else if (typeof(arg) == Float && !(arg->nxt)) {
                     switch(node->u.Noise.return_type) {
				/* einfache  2-D Noise-Funktion */
                       case Float: func_sym->name="noiseff";
                                   func_sym->type=Float;
                                   break;
                       default: 
                         fatal("no other noise-functions currently supported"); 
                     }
                  } else fatal("no other noise-functions currently supported");

                  func_result=gen_function_head(func_sym);
                  while(arg) {
                     emit("(");
                     arg_sym=gen_expr(arg);
                     emit("%s)",arg_sym->name);
                     free_tmp(arg_sym);
                     arg=arg->nxt;
                     if (arg) emit(",");
                  }
                  gen_function_tail(func_result);

                  return func_result;
                } break;

    case TEXTURE_NODE:

                  /* Die generische Funktion Texture ben"otigt eine
                   * Sonderbehandlung da sie je nach Typ und Anzahl der
                   * Parameter sowie vorangestelltem Qualifier eine
                   * andere Auspr"agung hat. "Ahnlich wie NOISE_NODE */

                {
                  struct symtab *func_result,*arg_sym,*func_sym;
                  tree fn=node->u.Texture.fn;
                  tree chan=node->u.Texture.chan;
                  tree arg=node->u.Texture.args;

                  NEW(func_sym);
                  func_sym->access = Func;
                  switch(node->u.Texture.return_type) {
                                /* einfache 3-D Texture-Funktion */
                       case Float: func_sym->name="texturef";
                                   func_sym->type=Float;
                                   break;

                                /* dreifache 3-D Noise-Funktion */
                       case Color: func_sym->name="textureV";
                                   func_sym->type=node->u.Texture.return_type;
                                   break;
                       default: internal("<gen_expr.TEXYURE_NODE> case error");
                  }

                  func_result=gen_function_head(func_sym);
                  emit("(");
                  arg_sym=gen_expr(fn);
                  emit("%s),(",arg_sym->name);
                  free_tmp(arg_sym);
                  arg_sym=gen_expr(chan);
                  emit("%s),",arg_sym->name);
                  free_tmp(arg_sym);
/* if (arg->type != String) then ... */
                  while(arg) {
                     emit("(");
                     arg_sym=gen_expr(arg);
                     emit("%s)",arg_sym->name);
                     free_tmp(arg_sym);
                     arg=arg->nxt;
                     if (arg) emit(",");
                  }
                  gen_function_tail(func_result);

                  return func_result;
                } break;

    case EXPR_NODE: 

		  /* Hier wird zun"achst durch rekursiven Aufruf von 
		   * gen_expr der Code f"ur die Kinder des aktuellen  
		   * Knotens erzeugt. Anschlie\ssend werden  die Ergebnisse
		   * die in tempor"aren Variablen abgelegt sind gem"a\ss 
		   * dem aktuellen Operator verkn"upft. */
                {
                  struct symtab *lsym,*rsym;
                  if (node->u.Expr.right) {
                    lsym=gen_expr(node->u.Expr.left);
                    rsym=gen_expr(node->u.Expr.right);
                    sym=gen2(node->u.Expr.op,lsym,rsym);
                    free_tmp(lsym);
                    free_tmp(rsym);
                  }else{
                    lsym=gen_expr(node->u.Expr.left);
                    sym=gen1(node->u.Expr.op,lsym);
                    free_tmp(lsym);
                  }
                  return sym;
                } break;

    default: internal("<gen_expr> case error %d",node->tag); break; 
  }
}

/*
 * gen_assign: Generiert Code f"ur eine Zuweisung.
 *	Input: Symbol, welchem der Ausdruck zugewiesen wird und Toplevel-
 *		knoten des Ausdrucks.
 */
gen_assign(sym,expr)
struct symtab *sym;
tree expr;
{
  struct symtab *result;
  char buf[80];
  struct symtab *new_sym;

  if (!expr) return;  /* keine Zuweisung. */

  /* Zun"achst wird der Toplevel-Knoten des Ausdrucks ausgewertet. Als
   * Ergebniss erh"alt man das Symbol einer tempor"aren Variablen.
   * Es sind drei F"alle zu unterscheiden. 
   * 	1. Zuweisung an einen Parameter.
   *	2. Zuweisung an eine globalen Variable.
   *	3. Zuweisung an eine lokale Variable.
   * siehe auch gen_expr(), IDENT_NODE. */
 
  result=gen_expr(expr);

  if (sym->access != Local) {
    NEW(new_sym);
    if (sym->access == Param)
       sprintf(buf,"$s->%s",sym->name);
    else /*  global symbol */
       sprintf(buf,"iob->%s",sym->name);
    new_sym->name=strsave(buf);
    new_sym->type=sym->type;
    new_sym->access=sym->access;
    gen2(OP_ASSIGN,new_sym,result);
  }else{
    gen2(OP_ASSIGN,sym,result);
  }
  free_tmp(result);   
}

/*
 * write_header_1: Generiert den ersten Teil des Rahmen-Programms.
 *	Input: Name des Shaders.
 */
write_header_1(shader)
char *shader;
{
   emit(
"/*\n\
 *      RenderMan: Shading-Module '%s'\n\
 */\n\
#include <stdio.h>\n\
#include <varargs.h>\n\
#include \"RiTypes.h\"\n\
#include \"RiError.h\"\n\
#include \"Shader.h\"\n\
#include \"V.h\"\n\
#define EQ(a,b) (!strcmp((a),(b)))"
  ,shader);
}

/*
 * declare_params: Generiert die Struktur welche die Parameter eines Shaders
 *		aufnimmt.
 *	Input: Name des Shaders und Kopf der Parameter-Liste.
 */
declare_params(shader,param_decl)
char *shader;
tree param_decl;
{
  emit("typedef struct {");

  /* Es wird die Parameter-Liste St"uck f"ur St"uck abgearbeitet und
   * je nach Typ des jeweiligen Parameters ein entsprechender Eintrag
   * generiert.  */

  while(param_decl) {
    if (param_decl->tag != DECL_NODE) 
      internal("<declare_params> wrong node id"); 
    switch (param_decl->u.Decl.sym->type) {
      case Float : emit("RtFloat %s;",param_decl->u.Decl.sym->name); break;
      case Color : emit("RtColor %s;",param_decl->u.Decl.sym->name); break;
      case Point : emit("RtPoint %s;",param_decl->u.Decl.sym->name); break;
      case String: emit("RtString %s;",param_decl->u.Decl.sym->name); break;
      default: internal("<declare_params> case error"); break;
    }
    param_decl=param_decl->nxt;
  }

  emit("} %s$type;",shader);
}

/*
 * write_header_2: Generiert den zweiten Teil des Rahmen-Programms.
 *		Dieser besteht aus den Funktionen ShaderName(), ShaderInfo()
 *		und ShaderSupport() sowie dem Kopf von ShaderShade().
 *      Input: Name des Shaders und Kopf der Parameter-Liste.
 */
write_header_2(shader,param_decl)
char *shader;
tree param_decl;
{
   tree node=param_decl;

   emit("\
/*\n\
 * ShaderName: Liefert den Namen des Shaders.\n\
 */\n\
static char *\n\
ShaderName()\n\
{\n\
  return \"%s\" ;\n\
}\n\
/*\n\
 * ShaderInfo: Druckt die Instanzen-Variablen des Shaders aus.\n\
 */\n\
static void\n\
ShaderInfo(shader,stream)\n\
Shader *shader;\n\
FILE *stream;\n\
{\n\
  %s$type *s = (%s$type *)shader->data;\n\
  fprintf(stream,\"%s:\\n\\"
  ,shader,shader,shader,shader);

  while(node) {
    switch (node->u.Decl.sym->type) {
      case Float : emit(" %s = %%g\\",node->u.Decl.sym->name); break;
      case Color :
      case Point : emit(" %s = (%%g;%%g;%%g)\\",node->u.Decl.sym->name); break;
      case String: emit(" %s = \\\"%%s\\\"\\",node->u.Decl.sym->name); break;
    }
    node=node->nxt;
    if (node) emit(",\\n\\");
  }
  emit("\"");
  node=param_decl;
  while(node) {
    switch (node->u.Decl.sym->type) {
      case String:
      case Float : emit(",s->%s",node->u.Decl.sym->name); break;
      case Color :
      case Point : emit(",s->%s[0],s->%s[1],s->%s[2]",node->u.Decl.sym->name,
                   node->u.Decl.sym->name,node->u.Decl.sym->name); break;
    }
    node=node->nxt;
  }

  emit(");\n\
}\n\
/*\n\
 * ShaderSupport: Waehlt die benoetigten I/O-Variablen aus.\n\
 */\n\
static IOF *\n\
ShaderSupport(shader)\n\
Shader shader;\n\
{\n\
  return &iof;\n\
}\n\
static void\n\
ShaderShade(level,shader,lightset,iob)\n\
int level;\n\
Shader *shader;\n\
LightSet *lightset;\n\
IOB *iob;\n\
{\n\
%s$type *$s = (%s$type *)shader->data;\n\
RtFloat $f0,$f1,$f2,$f3,$f4,$f5,$f6;\n\
RtColor $c0,$c1,$c2,$c3,$c4,$c5,$c6;\n\
RtPoint $p0,$p1,$p2,$p3,$p4,$p5,$p6;"
,shader,shader);
}

/*
 * declare_locals: Generiert die lokale Variablen des Shaders ggf. 
 *		mit Initialisierung.
 *      Input: Liste der lokalen Variablen.
 */
declare_locals(local_decl)
tree local_decl;
{
  tree node = local_decl;

  /* Es wird die Liste St"uck f"ur St"uck abgearbeitet und
   * je nach Typ der jeweiligen Variablen der entsprechender Code
   * generiert.  */

  while(node) {
    if (node->tag != DECL_NODE)
      internal("<declare_locals> wrong node id %d",node->tag);
    switch (node->u.Decl.sym->type) {
      case Float : emit("RtFloat %s;",node->u.Decl.sym->name); break;
      case Color : emit("RtColor %s;",node->u.Decl.sym->name); break;
      case Point : emit("RtPoint %s;",node->u.Decl.sym->name); break;
      case String: emit("RtString %s;",node->u.Decl.sym->name); break;
      default: internal("<declare_locals> case error"); break;
    }
    node=node->nxt;
  }

  /* Erneut wird die Liste durchlaufen um ggf. Code f"ur die Initialisierung
   * zu erzeugen. */

  node=local_decl;
  while(node) {
    if (node->u.Decl.init) {
      gen_assign(node->u.Decl.sym,node->u.Decl.init);
      emit(";"); 
    }
    node=node->nxt;
  }

} 

/*
 * gen_stmt: Generiert Code f"ur eine Liste von Statements.
 *	Input: Besagte Liste.
 */
gen_stmt(node)
tree node;
{

  while(node) {
    switch(node->tag) {

	/* die Statements break, continue, return werden genau so
	 * wieder ausgegeben wie sie gelesen wurden. */
 
      case BREAK_NODE: emit("break;"); break;
      case CONTINUE_NODE: emit("continue;"); break;
      case RETURN_NODE: emit("return;"); break;
 
      case ASSIGN_NODE:
		   /* F"ur ein Assign-Statement wird durch den Aufruf von
		    * gen_assign() Code erzeugt. */

                   gen_assign(node->u.Assign.sym,node->u.Assign.expr);
                   emit(";");
                   break;

      case FOR_NODE:
		   /* Ein For-Statement besteht aus drei Teilen. Diese
		    * werden durch den Aufruf von gen_assign() und  
		    * gen_expr() erzeugt. */
	           { 
                   struct symtab *expr_sym;
                   emit("for(");
                   if (node->u.For.init) {
                      gen_assign(node->u.For.init->u.Assign.sym, 
                                 node->u.For.init->u.Assign.expr);
                   }
                   emit(";");
                   expr_sym=gen_expr(node->u.For.expr);
                   emit("%s;",expr_sym->name);
                   if (node->u.For.assign) {
                      gen_assign(node->u.For.assign->u.Assign.sym,
                                 node->u.For.assign->u.Assign.expr);
                   }
                   emit("){");
                   gen_stmt(node->u.For.stmt);
                   emit("}");
                   free_tmp(expr_sym);
                  } break; 

      case IF_NODE: 
		  /* Ein If-Statement besteht aus einem (boolschen) Ausdruck
		   * und einem Body sowie eventuell einem optionalen Else-Teil.
		   * Das Ergebniss des Ausdruck steht nach der Auswertung in 
		   * der durch gen_expr() zur"uckgelieferten tempor"aren 
		   * Variablen.  */
                  {
                  struct symtab *expr_sym;
                  emit("if(");
                  expr_sym=gen_expr(node->u.If.expr); 
                  emit("%s){",expr_sym->name);
                  gen_stmt(node->u.If.then);
                  if (node->u.If.optional_else) {
                     emit("}else{");
                     gen_stmt(node->u.If.optional_else);
                  }
                  emit("}");
                  free_tmp(expr_sym);
                } break;

    case FUNC_NODE:

                 /* Eine Funktion in diesem Kontext (als einzelner Ausdruck)
                  * ist in wirklichkeit eine Prozedur !
                  * Bei Einer Prozedur wird erst der Prozedurkopf generiert.
                  * Anschlie\ssend werden die Argumente nach einander
                  * durch rekursiven Aufruf von gen_expr() abgearbeitet.
                  * Der Aufruf von gen_function_tail() schlie\sst den Rumpf
                  * der Funktion ab. */

                {
                  struct symtab *arg_sym;
                  tree arg;

                  gen_function_head(node->u.Func.sym);
                  arg=node->u.Func.args;
                  while(arg) {
                     emit("(");
                     arg_sym=gen_expr(arg);
                     emit("%s)",arg_sym->name);
                     free_tmp(arg_sym);
                     arg=arg->nxt;
                     if (arg) emit(",");
                  }
                  gen_function_tail(0);
                  emit(";");
                } break;

      default: internal("<gen_stmt> case error"); break;
    }
    node=node->nxt;
  }
}

/*
 * gen_footer_1: Generiert den ersten Teil des Epilogs. Dieser enth"alt
 *		die Adressen der Funktionen des Shaders.  (welche ja
 *		die Methoden des Shaders darstellen).	
 *	Input: Name des Shaders. 
 */
gen_footer_1(shader)
char *shader;
{
  emit("}\n\
static ShadeProcs %s$progs = {\n\
        ShaderName,\n\
        ShaderInfo,\n\
        ShaderSupport,\n\
        ShaderShade\n\
};"
  ,shader);
}

/* gen_init: Generiert den initialisierungs Teil eines Shaders. durch die
 *		Funktion NEW_Shader() kann dann eine neue Instanz dieses
 *		Shaders erzeugt werden.
 *	Input: Name des Shaders und Liste der Parameter (Instanze-Variablen).
 */
gen_init(shader,param_decl)
char *shader;
tree param_decl;
{
  int flag;
  tree node = param_decl;

  emit("static Shader *\n\
NEW_Shader(argp)\n\
  va_list argp;\n\
{\n\
  RtToken token;\n\
  %s$type *$s;\n\
  Shader   *S;\n\
  RtFloat $f0,$f1,$f2,$f3,$f4,$f5,$f6;\n\
  RtColor $c0,$c1,$c2,$c3,$c4,$c5,$c6;\n\
  RtPoint $p0,$p1,$p2,$p3,$p4,$p5,$p6;\n\
  $s=(%s$type *)RmMalloc(sizeof(%s$type));\n\
  S=(Shader *)RmMalloc(sizeof(Shader));"
  ,shader,shader,shader);

  /* Zun"achst gilt es Code f"ur die Inizialisierung  der Parameter 
   * zu erzeugen. */

  while(node) {
    if (node->tag != DECL_NODE)
      internal("<gen_init> wrong node id %d",node->tag);
    if (node->u.Decl.init) {
      gen_assign(node->u.Decl.sym,node->u.Decl.init);
      emit(";");
    }
    node=node->nxt;
  }

  /* Anschliessend werden die token-value  Paare vom Stack gepollt
   * und ggf. Code f"ur eine Zuweisung erzeugt.  */ 

  emit("token=va_arg(argp,RtToken);\nwhile (token != RI_NULL) {");
   
  node=param_decl;

  flag=0;
  while(node) {
    if (!flag) { emit("if"); flag=1; }
    else emit("else if");
   
    emit("(EQ(token,\"%s\"))",node->u.Decl.sym->name);
    switch (node->u.Decl.sym->type) { 
      case Float: emit("$s->%s= *(RtFloat *)va_arg(argp,RtPointer);",
                       node->u.Decl.sym->name); break;
      case Color:
      case Point: 
            emit("RmMemCpy(va_arg(argp,RtPointer),$s->%s,sizeof(RtPoint));",
                  node->u.Decl.sym->name); break;
      case String: emit("$s->%s=RmStrSave(va_arg(argp,RtPointer));",
                   node->u.Decl.sym->name); break;
      default: internal("<gen_init> case error"); break;
    }
    node=node->nxt;
  }

  /* Zuletzt noch einige Zuweisungen und die Funktion _init_%s$shader() welche
   * die Adresse der Instanzierungs-Routine in den Shader-Hash eintr"agt */

  if (flag) emit("else {");
  emit("sprintf(RmMsg,\"[SurfaceShader.%s] unknown or wrong token RI_%%s (ignored)\",token);",shader);
  emit("warning(2,RmMsg);"); emit("break;");
  if  (flag)  emit("}");

  emit("token=va_arg(argp,RtToken);\n\
}\n\
  S->data=(char *)$s;\n\
  S->procs= &%s$progs;\n\
  return S;\n\
}"
  ,shader);

  emit("void\n\
_init_%s$shader()\n\
{\n\
  RmInstallNewShader(\"%s\",NEW_Shader);\n\
}"
  ,shader,shader);

}

/*
 * GenCode: Generate Code.
 *	Input: Name des Shaders, Liste der Parameter, Liste der lokalen 
 *		Variablen, TopNode des Syntax-Baumes.
 */ 
GenCode(shader,param_decl,local_decl,top_node)
char *shader;
tree param_decl,local_decl,top_node;
{
  write_header_1(shader);
  declare_params(shader,param_decl);
  /* declare_iob_flags(top_node); $$$ */
  emit("static IOF iof = {1,1,1,0,0,1,0,0,1,1,0,0,0,0,0,0,1,0,0,0,0};");

  write_header_2(shader,param_decl);

  declare_locals(local_decl);

  gen_stmt(top_node);

  gen_footer_1(shader);

  gen_init(shader,param_decl);

}
/*\end{verbatim}*/
