/*\subsection{AdminGlobals.c} \label{AdminGlobals.c}
 * Verwaltung aller globalen Variablen.
\begin{verbatim}*/
#include "RiTokens.h"
#include "MathTrafo.h"
#include "AdminStack.h"
#include "AdminGlobals.h"
#include "Screen.h"

  Globals	RmGlobals;

void
_init_AdminGlobals()
{
  /* Einige Defaultwerte f"ur die RmGlobals setzen ! */

  RmGlobals.solidblockstack = RmNewStack(sizeof(SBlock),100);
  RmGlobals.attributstack = RmNewStack(sizeof(attributstackunion),100);
  RmGlobals.activattributes.orientation = RI_LH;
  setV3(0.,0.,0.,RmGlobals.activattributes.Cs);
  setV3(1.,1.,1.,RmGlobals.activattributes.Os);
  RmIdentityM44(RmGlobals.activattributes.trans.OtoW);
  RmGlobals.solidblock.primflag = 1; /* Flags and Pointer for handle */
  RmGlobals.solidblock.solidflag = 0;/* the Solid-Blocks */
  RmGlobals.solidblock.compflag = 0;
  RmGlobals.ModelRoot = (Topnode *)RmMalloc(sizeof(Topnode));
  RmGlobals.currenttopnode = RmGlobals.ModelRoot;
  RmGlobals.currenttopnode->type = TOPNODE;
  RmGlobals.currenttopnode->csg_tree = 0;
  RmGlobals.currenttopnode->next = 0;
  RmGlobals.solidblock.currentcompnode = 0;
  RmGlobals.solidblock.currentsolidnode = 0;
  RmGlobals.GenRay = RmGenRayOrthographic;
  strcpy(RmGlobals.fname,"a.rle");
  RmGlobals.MaxLevel = 3;
  RmGlobals.inworld = 0;
  RmGlobals.lightnumber = 0;
  RmGlobals.innerlightpos = -1;
  RmGlobals.outerlightpos = -1;
  RmMemClr(RmGlobals.activattributes.outerlightstate,sizeof(RtBoolean *) 
           * NumberofLights);
  RmMemClr(RmGlobals.activattributes.innerlightstate,sizeof(RtBoolean *) 
           * NumberofLights);
  RmMemClr(RmGlobals.innerlights,sizeof(Shader *) * NumberofLights);
  RmMemClr(RmGlobals.outerlights,sizeof(Shader *) * NumberofLights);

  RmIdentityM44(RmGlobals.WtoC);
  RmIdentityM44(RmGlobals.CtoW);

  /* Default settings for RiExposure() */
  RmGlobals.Exposure_gain = 1.0;
  RmGlobals.Exposure_gamma = 1.0;

  /* Default settings for RiQuantisize() */
  RmGlobals.Quant_type = "rgb";	/* wird noch nicht ausgewertet */
  RmGlobals.Quant_one = 255;
  RmGlobals.Quant_min = 0;
  RmGlobals.Quant_max = 255;
  RmGlobals.Quant_ditheramplitude = .5;

}

void
_exit_AdminGlobals()
{
  /* Stacks wieder freigeben */
  RmFreeStack(RmGlobals.attributstack);
  RmFreeStack(RmGlobals.solidblockstack);
}

/*\end{verbatim}*/
