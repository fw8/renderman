/* \subsection{Trace.c} \label{Trace.c}
 * Trace : Trace St"osst die Schnittermittlung und den Shader an.
\begin{verbatim}*/

#include "RiTypes.h"
#include "AdminGlobals.h"
#include "MathVector.h"
#include "MathTrafo.h"
#include "Prim.h"
#include "Shader.h"
#include "RiError.h"

static RtColor black = {1.,1.,1.};
/* \end{verbatim} \label{Trace.c:ShadeBackGround} \begin{verbatim}
*/
static void
ShadeBackground(ray,col)
Ray *ray;
RtColor col;
{
  copyV3(black,col);  /* Immoment wird der Hintergrund immer Schwarz */
}

/* \end{verbatim} \label{Trace.c:RmGetJunk} \begin{verbatim}
 * getjunk : reserviert <size> Bytes auf einem Stack, der nur w"ahrend 
 *           der Bearbeitung eines Strahls existiert.
 */

static int junk_ptr;
static char junkmem[MAXJUNKSIZE];

char *
RmGetJunk(size)
unsigned size;
{
  char *adr= &junkmem[junk_ptr];
  if ((junk_ptr+=size) > MAXJUNKSIZE) 
		fatal(1,"[Trace.RmGetJunk] junkmem overflow");
  return adr;
}


/*
 * \end{verbatim} \label{Trace.c:GetNearst} \begin{verbatim}
 * GetNearst : N"achstgelegenen Schnitt suchen.
 */

static Isect thit[2][ISECTMAX];

static RtBoolean
GetNearst(ray,hit)
Ray *ray;
Isect **hit;
{
  Topnode *ptr = RmGlobals.ModelRoot;
  int i=0;

  RtFloat dt = RI_INFINITY;

  while (ptr) {  /* Solange noch K^Trper im Datenbaum */

    if (RmIntersect(ray,ptr->csg_tree,thit[i])) {

      if (thit[i][0].t < dt) {  /* N"aherer Schnitt als alle bisherigen ? */

            dt = thit[i][0].t;
            i = !i; 
      }
    }
    ptr = ptr->next ;

  }
  if (dt==RI_INFINITY) return RI_FALSE;
  *hit = thit[!i];
  return RI_TRUE;
 
}

/*
 * \end{verbatim} \label{Trace.c:RmTrace} \begin{verbatim}
 * RmTrace: Verfolgt einen Strahl (in Weltkoordinaten) durch die Szene
 *		und gibt seinen Farbwert zur"uck.
 *		Sucht den ersten Schnittpunkt f"ur t>0 und ruft den Shader
 *		auf.
 */
void
RmTrace(level,ray,col)
int level;
Ray *ray;
RtColor col;
{
  Prim *prim;
  Ray  tray;
  RtColor tcol;
  RtPoint P;
  Isect *hit;
  Shader *shader;
  IOB iob;
  IOF *iof;

  junk_ptr=0;	/* init junkmem */

  /* intersect ray with everything in scene */
  if (GetNearst(ray,&hit)) {

    /* find prim at first intersection */
    prim = hit[0].prim;

    RayPoint(ray,hit[0].t,iob.Ps);

    iof=(*prim->shader.SurfaceShader->procs->support)();

    if (iof->P) copyV3(iob.Ps,iob.P);

    if (iof->Cs) copyV3(prim->attrib.Cs,iob.Cs);
    if (iof->Os) copyV3(prim->attrib.Os,iob.Os);
    if (iof->I) copyV3(ray->D,iob.I); 

    (*prim->procs->setup)(hit,iof,&iob); /* F"ullen des I/O-Blocks */

#if 0
    printV3("N in oc : ",iob.N,stdout);
#endif

    if (iof->N) RmTransformVector(iob.N,prim->OtoW,iob.N);
    if (iof->Ng) RmTransformVector(iob.Ng,prim->OtoW,iob.Ng);

#if 0
    printV3("N in wc : ",iob.N,stdout);
#endif

    if (iof->s) iob.s = iob.u;   /* Solange RiTexturCoordinates() */
    if (iof->t) iob.t = iob.v;   /* noch nicht implementiert ist. */

    setV3(1.,1.,1.,iob.Oi);
    setV3(0.,0.,0.,iob.Ci);

    /* f"ur den gefundenen Punkt wird nun der zugeh"orige Shader aufgerufen */
    shader=prim->shader.SurfaceShader;
    (* shader->procs->shade)(level,shader,prim->lightset,&iob);

    if (Cr(iob.Oi)<1. || Cg(iob.Oi)<1. || Cb(iob.Oi)<1.) {/* transparent ? */
      if (level+1<RmGlobals.MaxLevel) {  /* Rekursionstiefe "uberschritten ? */
        SetRay(iob.Ps,ray->D,tray);
        RmTrace(level+1,&tray,tcol); 
        Cr(col)=Cr(iob.Ci)+(1.-Cr(iob.Oi))*Cr(tcol);
        Cg(col)=Cg(iob.Ci)+(1.-Cg(iob.Oi))*Cg(tcol);
        Cb(col)=Cb(iob.Ci)+(1.-Cb(iob.Oi))*Cb(tcol);
      }else{
        copyV3(iob.Ci,col);
      }
    }else{
      copyV3(iob.Ci,col);
    }
  }else{

    /* wenn es keine Schnittpunkte gibt dann ermittle die Hintergrundfarbe */
    ShadeBackground(ray,col);
  }
}
/*\end{verbatim}*/
