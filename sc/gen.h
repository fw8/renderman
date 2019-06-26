/* \subsection{gen.h} \label{gen.h}
 *      RenderMan Shading Language
 *      Codegenerierung.
\begin{verbatim} */

enum opcode    {OP_MM,
		OP_PP,
		OP_ASSIGN,
		OP_MINUS,
		OP_ADD,
		OP_SUB,
		OP_MUL,
		OP_DIV,
		OP_GT,
		OP_LT,
		OP_GE,
		OP_LE,
		OP_EQ,
		OP_NE,
		OP_AND,
		OP_OR,
		OP_DOT,
		OP_CROSS
		};

#define OPCODE_STR {	"--",\
			"++",\
			"=",\
			"-",\
			"+",\
			"-",\
			"*",\
			"/",\
			">",\
			"<",\
			">=",\
			"<=",\
			"==",\
			"!=",\
			"&&",\
			"||",\
			".",\
			"^"\
		    }
extern GenCode();
/*\end{verbatim}*/
