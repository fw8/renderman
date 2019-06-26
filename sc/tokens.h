
typedef union  	{struct symtab *y_sym; /* Identifier */
	 char          *y_str; /* Constant */
         enum types     y_typ; /* Type */
         tree           y_tree; /* Syntax-Tree */
	} YYSTYPE;
extern YYSTYPE yylval;
# define Identifier 257
# define Constant 258
# define StrConstant 259
# define FLOAT 260
# define POINT 261
# define COLOR 262
# define STRING 263
# define LIGHT 264
# define SURFACE 265
# define ATHMOSPHERE 266
# define INTERIOR 267
# define EXTERIOR 268
# define ILLUMINANCE 269
# define ILLUMINATE 270
# define NOISE 271
# define TEXTURE 272
# define IF 273
# define ELSE 274
# define WHILE 275
# define FOR 276
# define BREAK 277
# define CONTINUE 278
# define RETURN 279
# define GE 280
# define LE 281
# define EQ 282
# define NE 283
# define AND 284
# define OR 285
# define PE 286
# define ME 287
# define TE 288
# define DE 289
# define RE 290
# define PP 291
# define MM 292
# define UNARY 293
