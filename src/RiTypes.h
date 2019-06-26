/* \subsection{RiTypes.h} \label{RiTypes.h}
 * Definitions of Abstract Types used in RI
\begin{verbatim}*/

#ifndef _RiTypes_
#define _RiTypes_
 
typedef char		RtBoolean;
typedef long		RtInt;
typedef double		RtFloat;

typedef char		*RtToken;

typedef RtFloat		RtColor[3]; 
typedef RtFloat		RtPoint[3];
typedef RtFloat		RtMatrix[4][4];
typedef RtFloat		RtBasis[4][4];
typedef RtFloat		RtBound[6];
typedef char		*RtString;

typedef char		*RtPointer;
#define RtVoid void
typedef double		(*RtFloatFunc)();
typedef RtVoid		(*RtFunc)();

typedef RtPointer	RtObjectHandle;
typedef int             RtLightHandle;

#define RI_FALSE	0
#define RI_TRUE		(! RI_FALSE)
#define RI_INFINITY	1.0e38
#define RI_EPSILON	1.0e-10
#define RI_NULL		((RtPointer)0)

#endif _RiTypes_
/*\end{verbatim}*/
