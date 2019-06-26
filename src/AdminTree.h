/* \subsection{AdminTree.h} \label{AdminTree.h}
  HeaderFile f"ur die Baumverwaltung.
\begin{verbatim} */
#ifndef _AdminTree_
#define _AdminTree_

#include "Prim.h"

typedef struct{
        RtBoolean       primflag;
        RtBoolean       solidflag;
        RtBoolean       compflag;
        Comp            *currentcompnode;
        Solid           *currentsolidnode;
} SBlock;

extern void RmFillPrim();
extern void RmChainPrim();
extern void RmMakeBounds();
extern void RmPrintModelRoot();

#endif _AdminTree_
/*\end{verbatim}*/
