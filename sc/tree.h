/* \subsection{tree.h} \label{tree.h}
Headerfile zu tree.c 
\begin{verbatim} */

#include "symtab.h"
#include "gen.h"

enum node_type {ERROR_NODE,DECL_NODE,IF_NODE,WHILE_NODE,FOR_NODE,
                FUNC_NODE,NOISE_NODE,TEXTURE_NODE,IDENT_NODE,
		ASSIGN_NODE,CONST_NODE,EXPR_NODE,BREAK_NODE,
		CONTINUE_NODE,RETURN_NODE};


typedef struct tree_node {
	struct tree_node *nxt;
	enum node_type    tag;
        union {
	  struct decl_node{
            struct symtab    *sym;   /* symbol */
            struct tree_node *init;  /* init-expr. (NULL if no init)  */
	  } Decl;
	  struct const_node {
	    enum types     type; /* type of const */
	    union {
              double scalar;
              double vector[3]; 
	      char  *string;
            } u;
	  } Const;
	  struct if_node {
	    struct tree_node *expr;  /* conditional expression  */
	    struct tree_node *then; /* 'then' part */
	    struct tree_node *optional_else; /* optional 'else' part */
	  } If;
	  struct while_node {
	    struct tree_node *expr;  /* conditional expression  */
	    struct tree_node *stmt; /* body */
	  } While;
	  struct for_node {
	    struct tree_node *init; /* init-expr. (NULL if no init)  */
	    struct tree_node *expr;  /* conditional expression  */
	    struct tree_node *assign; /* loop control */
	    struct tree_node *stmt; /* body */
	  } For;
	  struct func_node {
	    struct symtab    *sym;   /* symbol */
	    struct tree_node *args; /* function args. */
	  } Func;
	  struct noise_node {
	    enum types  return_type; /* return type of noise */
	    struct tree_node *args; /* function args. */
	  } Noise;
          struct texture_node {
            enum types  return_type; /* return type of noise */
            struct tree_node *fn;    /* texture-filename. */
            struct tree_node *chan; /* color channel number. */
            struct tree_node *args; /* function args. */
          } Texture;
	  struct ident_node {
	    struct symtab    *sym;   /* symbol */
	  } Ident;
	  struct assign_node {
	    struct symtab    *sym;   /* symbol */
	    struct tree_node *expr;  /* expression to be assigned. */
	  } Assign;
	  struct expr_node {
	    enum types  result_type; /* result type of op */
	    enum opcode op; /* op-code */
	    struct tree_node *left; /* left expression  */
	    struct tree_node *right; /* right expression */
	  } Expr;
	} u;
} *tree;

#define NULL_TREE ((tree)0)


extern tree build_node(), append_nodelist();
extern tree build_vector_const_node();
extern tree build_string_const_node();
extern tree build_const_node();
extern tree build_decl_node();
extern tree build_if_node();
extern tree build_while_node();
extern tree build_for_node();
extern tree build_func_node();
extern tree build_noise_node();
extern tree build_texture_node();
extern tree build_ident_node();
extern tree build_assign_node();
extern tree build_expr_node();

extern enum types typeof();
/*\end{verbatim}*/
