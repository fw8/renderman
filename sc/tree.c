/* \subsection{tree.c} \label{tree.c}
Strukturen und Verwaltung der der internen Darstellung des Parse-Tree's.
\begin{verbatim} */

#include "tree.h"

#define ALLOC(n,t) n=(tree)xmalloc(sizeof(struct tree_node));\
			n->nxt=NULL_TREE;\
			n->tag=t

static char *opcode_str[] = OPCODE_STR;

extern double  atof();

/*
 * type_error: Gibt eine Fehlermeldung f"ur unpassende Operanten. 
 */
type_error(op)
enum opcode op;
{
  error("operands of '%s' have incompatible types",opcode_str[(int)op]);
}

/*
 * typeof: Ermittelt den Typ eines Knotens.
 */ 
enum types
typeof(node)
tree node;
{
  switch(node->tag) {
    case IDENT_NODE  : return node->u.Ident.sym->type;
                       break;
    case FUNC_NODE   : return node->u.Func.sym->type;
                       break;
    case NOISE_NODE  : return node->u.Noise.return_type;
                       break;
    case TEXTURE_NODE: return node->u.Texture.return_type;
                       break;
    case CONST_NODE  : return node->u.Const.type;
                       break;
    case EXPR_NODE   : return node->u.Expr.result_type;
                       break;
    case ERROR_NODE  : return Undef;
                       break;
    default          : internal("<typeof> case error %d",node->tag); break;
 }
 return  Error;
}

/*
 * merge_types: Berechnet den Ergebniss-typ der Operation op mit den 
 *		Operanten-typen ltyp und rtyp.
 *	Input: Operator, Typen der Ausdr"ucke links und rechts des Operators.
 */
enum types
merge_types(op,ltyp,rtyp)
enum opcode op;
enum types ltyp,rtyp;
{
   if (ltyp==Error || rtyp==Error) return Error; /* not too much error-msg. */

   switch(op) {
     case OP_ADD :
     case OP_SUB :
     case OP_MUL :
     case OP_DIV :
        switch(ltyp) {
          case Float: return rtyp; /* maybe Float,Color or  Point */
          case Color: if (rtyp==Point) {
                        type_error(op);
                        return Error;
                      } else return Color;
          case Point: if (rtyp==Color) {
                        type_error(op);
                        return Error;
                      } else return Point;
          case String: type_error(op); return Error;
          default: internal("<merge_types> case error"); break;
        }
        break;
     case OP_DOT :
        switch(ltyp) {
          case Float: 
          case Color: 
          case String: type_error(op);
                       return Error;
          case Point: if (rtyp!=Point) {
                        type_error(op);
                        return Error;
                      } else return Float;
          default: internal("<merge_types> case error"); break;
        } 
        break;

     case OP_CROSS :
        switch(ltyp) {
          case Float: 
          case Color: 
          case String: type_error(op);
                       return Error;
          case Point: if (rtyp!=Point) {
                        type_error(op);
                        return Error;
                      } else return Point;
          default: internal("<merge_types> case error"); break;
        } 
        break;

     case OP_EQ :
     case OP_NE :
        switch(ltyp) {
          case Float: return Bool;
          case Color: if (rtyp==Point) {
                        type_error(op);
                        return Error;
                      } else return Bool;
          case Point: if (rtyp==Color){
                        type_error(op);
                        return Error;
                      } return Point;
          case String: if (rtyp!=String){
                         type_error(op); 
                         return Error;
                       } else return Bool;
          default: internal("<merge_types.relop1> case error"); break;
        }
        break;

     case OP_GT :
     case OP_LT :
     case OP_GE :
     case OP_LE :
        switch(ltyp) {
          case Float: if (rtyp!=Float) {
                        type_error(op);
                        return Error;
                      } else return Bool;
          case Color:
          case Point: 
          case String: type_error(op);
                       return Error;
          default: internal("<merge_types.relop2> case error"); break;
        }
        break;

     case OP_AND:
     case OP_OR:  return Bool;
     default: internal("<merge_types> case error %d",op); break;
   }
}
          
tree
build_node(typ)
enum node_type typ;
{
  tree node; 
  ALLOC(node,typ);

  return node;
}

tree
append_nodelist(list1,list2)
tree list1,list2;
{
  tree list=list1;

  while(list->nxt) list=list->nxt;
  list->nxt=list2;
  return list1;
}
 
tree
build_decl_node(sym,init_expr)
struct symtab  *sym;
tree           init_expr;
{
  tree node; ALLOC(node,DECL_NODE);

  if (init_expr) {
    if (typeof(init_expr) == Error) {
       node->tag=ERROR_NODE;
       return node;
    }
    if (typeof(init_expr) != sym->type) {
      switch(sym->type) {
        case Float:
        case String: type_error(OP_ASSIGN);
		     node->tag=ERROR_NODE;
                     return node;
                     break;
        case Color: if (typeof(init_expr)==Point) {
                      type_error(OP_ASSIGN);
		      node->tag=ERROR_NODE;
                      return node;
                    } break;
        case Point: if (typeof(init_expr)==Color) {
                      type_error(OP_ASSIGN);
		      node->tag=ERROR_NODE;
                      return node;
                    } break;
        default: internal("<build_decl_node> case error"); break;
      }
    }
  }

  node->u.Decl.sym=sym;
  node->u.Decl.init=init_expr;

  return node;
}

tree
build_if_node(bool_expr,then_part,else_part)
tree bool_expr;
tree then_part;
tree else_part;
{
  tree node; ALLOC(node,IF_NODE); 

  node->u.If.expr = bool_expr;
  node->u.If.then = then_part;
  node->u.If.optional_else = else_part;

  return node;
}

tree
build_while_node(bool_expr,stmt)
tree bool_expr;
tree stmt;
{
  tree node; ALLOC(node,WHILE_NODE);

  node->u.While.expr = bool_expr;
  node->u.While.stmt = stmt;

  return node;
}

tree
build_for_node(init,bool_expr,assign,stmt)
tree init,bool_expr,assign,stmt;
{
  tree node; ALLOC(node,FOR_NODE);

  node->u.For.init = init;
  node->u.For.expr  = bool_expr;
  node->u.For.assign = assign;
  node->u.For.stmt = stmt;

  return node;
}

tree
build_func_node(sym,arg_list)
struct symtab *sym;
tree           arg_list;
{
  tree node; ALLOC(node,FUNC_NODE);

  node->u.Func.sym = sym;
  node->u.Func.args = arg_list; 

  return node;
}

tree
build_noise_node(typ,arg_list)
enum types typ;
tree       arg_list;
{
  tree node; ALLOC(node,NOISE_NODE);

  node->u.Noise.return_type = typ;
  node->u.Noise.args = arg_list;

  return node;
}

tree
build_texture_node(typ,fn,chan,arg_list)
enum types typ;
tree       fn,chan,arg_list;
{
  tree node; ALLOC(node,TEXTURE_NODE);

  node->u.Texture.return_type = typ;
  node->u.Texture.fn = fn;
  node->u.Texture.chan = chan;
  node->u.Texture.args = arg_list;

  return node;
}

tree
build_ident_node(sym)
struct symtab *sym;
{
  tree node; ALLOC(node,IDENT_NODE);

  node->u.Ident.sym = sym;

  return node;
}

tree
build_assign_node(sym,expr)
struct symtab *sym;
tree           expr;
{
  tree node; ALLOC(node,ASSIGN_NODE);

  if (typeof(expr) == Error || sym->type == Undef) {
    node->tag=ERROR_NODE;
    return node;
  }
  if (typeof(expr) != sym->type) {
    switch(sym->type) {
      case Float:
      case String: type_error(OP_ASSIGN);
		   node->tag=ERROR_NODE;
                   return node;
                   break; 
      case Color: if (typeof(expr)==Point) {
                    type_error(OP_ASSIGN); 
		    node->tag=ERROR_NODE;
                    return node;
                  } break; 
      case Point: if (typeof(expr)==Color) {
                    type_error(OP_ASSIGN); 
		    node->tag=ERROR_NODE;
                    return node;
                  } break; 
      default: internal("<build_assign_node> case error"); break;
    }
  }

  node->u.Assign.sym = sym;
  node->u.Assign.expr = expr;

  return node;
}

tree
build_const_node(str)
char  *str;
{
  tree node; ALLOC(node,CONST_NODE); 

  node->u.Const.type = Float;
  node->u.Const.u.scalar = atof(str); 

  return node;
}

tree
build_string_const_node(str)
char  *str;
{
  tree node; ALLOC(node,CONST_NODE);

  node->u.Const.type = String;
  node->u.Const.u.string = strsave(str);

  return node;
}

tree
build_vector_const_node(typ,conv,str1,str2,str3)
enum types typ;
char *conv,*str1,*str2,*str3;
{
  tree node; ALLOC(node,CONST_NODE); 
 
  node->u.Const.type = typ;
  node->u.Const.u.vector[0] = atof(str1);
  node->u.Const.u.vector[1] = atof(str2);
  node->u.Const.u.vector[2] = atof(str3);

  return node;
}

tree
build_expr_node(op,lexpr,rexpr)
enum opcode;
tree lexpr,rexpr;
{
  tree node; ALLOC(node,EXPR_NODE);

  if (rexpr)
    node->u.Expr.result_type = merge_types(op,typeof(lexpr),typeof(rexpr));
  else
    node->u.Expr.result_type = typeof(lexpr); /* unary minus */

  node->u.Expr.op = op;
  node->u.Expr.left = lexpr;
  node->u.Expr.right = rexpr;

  return node;
}
/*\end{verbatim}*/
