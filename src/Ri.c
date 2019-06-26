
/* \subsection{Ri.c} \label{Ri.c}
Alle Funktionen die nach Vorgabe aus \cite{renderman} zur Interaktion zwischen
User und RenderMan-Bibliothek ben"otigt werden!
\begin{verbatim} */

#include <stdio.h>
#include <varargs.h>
#include <string.h>

#include "RiTypes.h"
#include "RiTokens.h"
#include "RiError.h"
#include "MathVector.h"
#include "Admin.h"
#include "Misc.h"
#include "Screen.h"

#define PI 3.1415927
#define DegToRad(x) ((x)*PI/180.0)

#define EQ(a,b) (!strcmp((a),(b)))
#define NEQ(a,b) (!EQ(a,b))

#define toupper(c)      ((unsigned char)(c)-'a'+'A')
#define strtoupper(str) {char *s=str; while (*s) { *s=toupper(*s); s++; }}

extern void RmInitShaders();

/************************************************************************/
/*      GENERAL ROUTINES                                                */
/************************************************************************/
void
Ri_ErrorHandler(handler)
        RtFunc handler;
{
  RmErrorHandler=handler;
}  

void 
RiDeclare()
{
 /* $$$ */
}

/************************************************************************/
/*  BEGIN-END-BLOCK							*/
/************************************************************************/
/* \end{verbatim} \label{Ri.c:RmBegin} \begin{verbatim}
 * RiBegin: Hier werden alle n"otigen inits durchgef"uhrt!
 *
 *	INPUT: token = spezielle RenderMethode
 */
void
RmBegin(token)
	RtToken token;
{
  if (token != RI_NULL) {
    strtoupper(token);
    sprintf(RmMsg,"[RiBegin] unknown or wrong token RI_%s (ignored)",token);
    warning(2,RmMsg);
  }
}
/*\end{verbatim} \label{Ri.c:RiEnd} \begin{verbatim}
 * RiEnd: Hier werden alle n"otigen exits durchgef"uhrt!
 */
void
RiEnd()
{
  _exit_all_files();
}



/************************************************************************/
/*      FRAME BLOCK                                                     */
/************************************************************************/
void
RiFrameBegin()
{
  /* $$$ */
}

void
RiFrameEnd()
{
  /* $$$ */
}


/************************************************************************/
/*      WORLD BLOCK                                                     */
/************************************************************************/
/*\end{verbatim} \label{Ri.c:RiWorldBegin} \begin{verbatim}
 * RiWorldBegin: Hier  werden die Matrizen initialisiert und alle
 * $_begin_xyz Funktionen aufgerufen.
 */
void
RiWorldBegin()
{
  RtMatrix tmp;

  extern void RiSurface();

  _begin_all_files();

  RmIdentityM44(tmp);
  RmScalM44(tmp,1.,-1.,1.);
  RmMulM44(tmp,RmGlobals.activattributes.trans.OtoW,
           RmGlobals.activattributes.trans.OtoW);
     /* flip picture bottom up (rle need`s it this way) */

  RmCpyM44(RmGlobals.activattributes.trans.OtoW,RmGlobals.WtoC);
  if (RmInvertM44(RmGlobals.WtoC,RmGlobals.CtoW))
    internal(1,"[RiWorldBegin] can`t compute inverse matrix");
  RmIdentityM44(RmGlobals.activattributes.trans.OtoW);

  RmGlobals.inworld = 1;

  RmOpenPic(RmGlobals.fname,RmScreenGlobals.res_x,RmScreenGlobals.res_y,
            RmScreenGlobals.org_x,RmScreenGlobals.org_y);

  RiSurface("constant",RI_NULL);

}
/*\end{verbatim} \label{Ri.c:RiWorldEnd} \begin{verbatim}
 * RiWorldEnd: Aufruf der Bildgenerierung und danach alle
 * $_end_xyz() Funktionen aufgerufen.
 */
void
RiWorldEnd()
{
#ifdef DEBUG
  RmPrintModelRoot();
  fprintf(stderr,"\nnow calculate bounds\n");
#endif
  RmMakeBounds();
#ifdef DEBUG
  RmPrintModelRoot();
#endif

  RmGeneratePic();	/* Die eigentliche Bildgenerierung */
  _end_all_files();	/* Abschlu"s aller entsprechenden Files */
  RmGlobals.inworld = 0;/* Wir befinden uns wieder au"serhalb des
			   Worlblockes */
}


/************************************************************************/
/*      ATTRIBUTE BLOCK                                                 */
/************************************************************************/
/*\end{verbatim} \label{Ri.c:RiAttributeBegin} \begin{verbatim}
 * RiAttributeBegin: rettet alle Environment Attribute und alle 
 * Transformationen auf einen Stack!
*/
void RiAttributeBegin()
{
        attributstackunion      stackelement;
        attributes      *helpattr;

	helpattr = (attributes *)RmMalloc(sizeof(attributes)); /* Platz f"ur neues Element */
        *helpattr = RmGlobals.activattributes; /* Globals kopieren */
        stackelement.tag = ATTRIBUT; /* Es sind Attribute ! */
        stackelement.un.attr = helpattr;
        if (RmPushStack(RmGlobals.attributstack,&stackelement)) /*und drauf !*/
	  error(1,"[RiAttributeBegin] AttributeStackError!");
}

/*\end{verbatim} \label{Ri.c:RiAttributeEnd} \begin{verbatim}
 * RiAttributeEnd: restauriert alle Environment Attribute und setzt auch die
 * Transformation wieder auf den Zustand vor dem Block zurueck.
*/
void RiAttributeEnd()
{
        attributstackunion      stackelement;

	/* Attributblock vom Stack holen */
        if (RmPopStack(RmGlobals.attributstack,&stackelement)) 
 	  error(1,"[RiAttributeEnd] AttributeStackError!");
	/* Waren es gar keine Attr. */
        if (stackelement.tag != ATTRIBUT)
	  error(1,"[RiAttributeEnd] AttributeStackError!");
	/* Und die Globals entsprechend setzen */
        RmGlobals.activattributes = *stackelement.un.attr;
   	/* Platz wieder freigeben */
	RmFree(stackelement.un.attr);
}



/************************************************************************/
/*      TRANSFORM BLOCK                                                 */
/************************************************************************/
/*\end{verbatim} \label{Ri.c:RiTransformBegin} \begin{verbatim}
 * RiTranformBegin: Rettet die Transformationsmatrix auf den Stack
 */
void
RiTransformBegin()
{
	attributstackunion      stackelement;
        transform       *helptrans;

	/* Platz f"ur neues Element */
	helptrans = (transform *)RmMalloc(sizeof(transform));
	/* Globals kopieren */
        *helptrans = RmGlobals.activattributes.trans;
	/* Es sind Transformationen */
        stackelement.tag = TRANSFORMATION;
        stackelement.un.trans = helptrans;
	/* Und drauf schieben */
        if (RmPushStack(RmGlobals.attributstack,&stackelement))
 	  error(1,"[RiTransformBegin] TransformationStackError!");
}
/*\end{verbatim} \label{Ri.c:RiTransformEnd} \begin{verbatim}
 * RiTransformEnd: Holt die Transformationen vom Stack zur"uck.
 */
void
RiTransformEnd()
{
	attributstackunion      stackelement;

	/* Transformationsblock vom Stack holen */
        if (RmPopStack(RmGlobals.attributstack,&stackelement))
	  error(1,"[RiTransformEnd] TransformationStackError!");
	/* Waren es gar keine Transformationen */
        if (stackelement.tag != TRANSFORMATION)
	  error(1,"[RiTransformEnd] TransformationStackError!");
	/* Und die Globals entsprechend setzen */
	RmGlobals.activattributes.trans = *stackelement.un.trans;
	/* Platz wieder freigeben */
        RmFree(stackelement.un.trans);
}


/************************************************************************/
/*      SOLID BLOCK                                                     */
/************************************************************************/
/*\end{verbatim} \label{Ri.c:RiSolidBegin} \begin{verbatim}
 * RiSolidBegin: Einleitung eines Blocks f"r einen geschlossenen K"orper.
 *		 Alle CSG-Objekte m"ussen so beginnen, auch z.B. eine
 *		 Kugel.
 * 	INPUT :	 Typ des Solids (Primitive,Union,Difference,Intersection
 */
void
RiSolidBegin(type)
	RtToken type;
{
  Topnode	*tnode;
  Solid		*sbnode;
  Comp		*conode,*cnode;

  /* Flags des Solidblocks sichern */
  if (RmPushStack(RmGlobals.solidblockstack,&RmGlobals.solidblock))
      error(1,"[RiSolidBegin] SolidBlockStackError!");

  if (EQ(type,RI_PRIMITIVE)) {		/* Typ = Primitive */

    sbnode = (Solid *)RmMalloc(sizeof(Solid));	/* Neuen SolidKnoten mallocieren */
    sbnode->type = SOLID;		/* und f"ullen */
    sbnode->prim = 0;
    sbnode->next = 0;

    if (RmGlobals.solidblock.primflag) { 	/* Laut Flags soll Prim.
						   kommen */

      /* Ist der momentane Topknoten des Baumes schon belegt ? */
      if (RmGlobals.currenttopnode->csg_tree) { /* ja ! */
        tnode = (Topnode *)RmMalloc(sizeof(Topnode)); /* Neuen knoten erzeugen */
  	tnode->csg_tree = (Comp *)sbnode;  /* Primitiv einh"angen */
  	tnode->next = 0;		   /* NAchfolger noch leer */
  	tnode->type = TOPNODE;		   /* Typ ist Topknoten */
       	RmGlobals.currenttopnode->next = tnode; /* Wird zum momentanen */
  	RmGlobals.currenttopnode = tnode;  
      } else {
        /* Na sonst einfach dort einh"angen */
  	RmGlobals.currenttopnode->csg_tree = (Comp *)sbnode;
      }
    } 

    if (RmGlobals.solidblock.solidflag) {     /* Kein SOLID in einem SOLID !*/
	error(1,"[RiSolidBegin] Nothing else but Primitives allowed in Solid.");
    }

    if (RmGlobals.solidblock.compflag) {      /* Wir sind in einem CSG */

      if (RmGlobals.solidblock.currentcompnode->left) {
	/* Linker Knoten schon belegt ! */
        if (RmGlobals.solidblock.currentcompnode->right) {
          /* Rechter Knoten auch schon belegt ! */
	  cnode = (Comp *)RmMalloc(sizeof(Comp)); /* Also neuen erzeugen und umketten*/
	  *cnode = *RmGlobals.solidblock.currentcompnode;
          RmGlobals.solidblock.currentcompnode->right = (Comp *)sbnode;
	  RmGlobals.solidblock.currentcompnode->left = cnode;
        } else {
	  /* Na sonst rechts einh"angen. */
          RmGlobals.solidblock.currentcompnode->right = (Comp *)sbnode;
        }
      } else {
        /* Na sonst links einh"angen */
        RmGlobals.solidblock.currentcompnode->left = (Comp *)sbnode;
      }
    }

    /* SolidBlock neu belegen, so da"s klar ist, wir sind in nem SOLID */
    RmGlobals.solidblock.currentsolidnode = sbnode;
    RmGlobals.solidblock.primflag = 0;
    RmGlobals.solidblock.solidflag = 1;
    RmGlobals.solidblock.compflag = 0;

  } else {                              /* Typ = Irgendein CSG */
    
    conode = (Comp *)RmMalloc(sizeof(Comp));	/* Neuen Konten erzeugen */
    conode->type = COMPOSITE;		/* und defaults setzen */
    conode->left = 0;
    conode->right = 0;
    if      (EQ(type,RI_INTERSECTION)) conode->op = INTERSECTION;
    else if (EQ(type,RI_UNION))        conode->op = UNION;
    else if (EQ(type,RI_DIFFERENCE))   conode->op = DIFFERENCE;
    else {
      sprintf(RmMsg,"[RiSolidBegin] unknown token RI_%s ",type);
      warning(2,RmMsg);
    }

    if (RmGlobals.solidblock.primflag) {      /* Wir haben Prim. erwartet */

      /* Ist der momentane Topknoten noch frei ? */
      if (RmGlobals.currenttopnode->csg_tree) { /* nein */
        tnode = (Topnode *)RmMalloc(sizeof(Topnode));	/* neuen Topknoten erzeugen */
        tnode->csg_tree = conode;	/* und f"ullen */
        tnode->next = 0;
        tnode->type = TOPNODE;
        RmGlobals.currenttopnode->next = tnode; /* und zu momentanem machen */
        RmGlobals.currenttopnode = tnode;
      } else {
	/* Na sonst einfach einh"angen */
        RmGlobals.currenttopnode->csg_tree = conode;
      }
    }

    if (RmGlobals.solidblock.solidflag) {     /* Kein CSG innerhalb von einem
						 Solid ! */
      error(1,"[RiSolidBegin] Nothing else but Primitives allowed in Solid.");
    }

    if (RmGlobals.solidblock.compflag) {      /* Wir sind in einem CSG */

	/* An der passenden Stelle einketten ! */
      if (RmGlobals.solidblock.currentcompnode->left) {
        if (RmGlobals.solidblock.currentcompnode->right) {
          cnode = (Comp *)RmMalloc(sizeof(Comp));
	  *cnode = *RmGlobals.solidblock.currentcompnode;
	  RmGlobals.solidblock.currentcompnode->left = cnode;
          RmGlobals.solidblock.currentcompnode->right = conode;
        } else {
          RmGlobals.solidblock.currentcompnode->right = conode;
        }
      } else {
        RmGlobals.solidblock.currentcompnode->left = conode;
      }
    }

    /* SOlidBlock so setzen, da"s klar ist, wir sind in nem CSG! */
    RmGlobals.solidblock.currentcompnode = conode;
    RmGlobals.solidblock.primflag = 0;
    RmGlobals.solidblock.solidflag = 0;
    RmGlobals.solidblock.compflag = 1;

  }
}
/*\end{verbatim} \label{Ri.c:RiSolidEnd} \begin{verbatim}
 * RiSolidEnd: Beendet einen SolidBlock und holt die alten Flags vom Stack
 */
void
RiSolidEnd()	/* restore SolidBlockFlags */
{
  if (RmPopStack(RmGlobals.solidblockstack,&RmGlobals.solidblock))
    error(1,"[RiSolidEnd] SolidBlockStackError!");
}


/************************************************************************/
/*      OBJECT BLOCK                                                    */
/************************************************************************/
void
RiObjectBegin()
{
  /* $$$ */
}

void
RiObjectEnd()
{
  /* $$$ */
}


/************************************************************************/
/*      MOTION BLOCK                                                    */
/************************************************************************/
void
RiMotionBegin()
{
  /* $$$ */
}

void
RiMotionEnd()
{
  /* $$$ */
}

/************************************************************************/
/*      OPTIONS                                                         */
/************************************************************************/
/*\end{verbatim} \label{Ri.c:RiFormat} \begin{verbatim}
 * RiFormat: Setzt das Ausgabeformat des Bildes!
 *
 *	INPUT :	xresolution = Aufl"osung in x-Richtung
 *		yresolution = Aufl"osung in y-Richtung
 *		pixelaspectratio = Verh"altnis Pixelh"ohe zu -breite
 */		
void
RiFormat(xresolution,yresolution,pixelaspectratio)
        RtInt   xresolution,yresolution;
        RtFloat pixelaspectratio;
{
  RmScreenGlobals.res_x = xresolution;
  RmScreenGlobals.res_y = yresolution;

  if (pixelaspectratio > 0.) RmScreenGlobals.PAR = pixelaspectratio;
}
/*\end{verbatim} \label{Ri.c:RiFrameAspectRatio} \begin{verbatim}
 * RiFrameAspectRatio: Setzt selbiges!
 *
 *	INPUT :	frameaspectratio
 */
void
RiFrameAspectRatio(frameratio)
        RtFloat frameratio;
{
  RmScreenGlobals.FAR = frameratio;
}

/*\end{verbatim} \label{Ri.c:RiScreenWindow} \begin{verbatim}
 * RiScreenWindow: Setzt Begrenzungen des Bildschirmfensters
 *
 *	INPUT :	Ecken des Fensters
 */
void
RiScreenWindow(left,right,bottom,top)
        RtFloat left,right,bottom,top;
{
  RmScreenGlobals.scr_left=left;
  RmScreenGlobals.scr_right=right;
  RmScreenGlobals.scr_bottom=bottom;
  RmScreenGlobals.scr_top=top;
  RmScreenGlobals.scr_default=RI_FALSE;
}
/*\end{verbatim} \label{Ri.c:RiCropWindow} \begin{verbatim}
 * RiCropWindow: Setzen des zu berechnenden Windowausschnittes
 *
 *	INPUT : Ecken des Windowausschnittes
 */
void
RiCropWindow(left,right,top,bottom)
        RtFloat left,right,top,bottom;
{
  RmScreenGlobals.crop_left   = left;
  RmScreenGlobals.crop_right  = right;
  RmScreenGlobals.crop_top    = top;
  RmScreenGlobals.crop_bottom = bottom;
}

/*\end{verbatim} \label{Ri.c:RiProjection} \begin{verbatim}
 * RiProjection: Setzen der Projektionsart
 * 
 * 	INPUT :	Projektionsart + Parameter
 */
void
RiProjection(name,va_alist)
	RtString name;
	va_dcl
{
  va_list argp;
  RtToken token;
 
  /* Projektionsart setzen */ 
  if       (EQ(name,"orthographic")) {
    RmGlobals.GenRay = RmGenRayOrthographic; 
  }else if (EQ(name,"perspective")) {
    RmGlobals.GenRay = RmGenRayPerspective; 
  }else{
    sprintf(RmMsg,"[RiProjection] unknown projection method '%s' (ignored)",name);
    warning(1,RmMsg);
  }
  
  va_start(argp);
  
  token=va_arg(argp,RtToken);

  /* Parameter abarbeiten */
  while (token != RI_NULL) {
  
    if (EQ(token,RI_FOV)) {
      RtFloat value = *(RtFloat *)va_arg(argp,RtPointer);
      
      if (value<=0. || value>=PI) {
        error(1,"[RiProjection] fov not between 0 and PI (set to PI)");
        RmScreenGlobals.field_of_view = PI/2.;
      }else{
        RmScreenGlobals.field_of_view = value;
      }
      /* Daraus neues Window berechnen */
      value = tan(RmScreenGlobals.field_of_view/2.);
      RmScreenGlobals.scr_left   = -value;
      RmScreenGlobals.scr_right  =  value;
      RmScreenGlobals.scr_bottom = -value;
      RmScreenGlobals.scr_top    =  value;
     
    }else{

      sprintf(RmMsg,"[RiProjection] unknown or wrong token RI_%s (ignored)",token);          
      warning(2,RmMsg);
      va_arg(argp,RtPointer); /* fetch dummy value */

    }

    token=va_arg(argp,RtToken); /* fetch next token */
  }
  
  va_end(argp);
}

/*\end{verbatim} \label{Ri.c:RiClipping} \begin{verbatim}
 * RiCliping: Clipping setzen 
 * 
 *	INPUT :	near = Clipping Untergrenze
 *		far  = Clipping Obergrenze
 */
void
RiClipping(near,far)
        RtFloat near,far;
{
	/* Untergrenze < Epsilon */
  if (near < RI_EPSILON) {
    error(2,"[RiClipping] near < RI_EPSILON");
  }else{
     RmScreenGlobals.z_clip_near = near;
  }
 	/* Obergrenze > Unendlich */
  if (far > RI_INFINITY) {
    error(2,"[RiClipping] far > RI_INFINITY");
  }else{
     RmScreenGlobals.z_clip_far  = far;
  }
}


void
RiDepthOfField()
{
 /* $$$ */
}

void
RiShutter()
{
 /* $$$ */
}

void
RiPixelVariance()
{
 /* $$$ */
}
/*\end{verbatim} \label{Ri.c:RiPixelSamples} \begin{verbatim}
 * RiPixelSamples: Setzen der Samplezahlen
 *
 *	INPUT :	x = Anzahl der Samples in x-Richtung
 *		y = Anzahl der Samples in y-Richtung
 */
void
RiPixelSamples(x,y)
        RtFloat x,y;
{
  RmScreenGlobals.xsamples = x;
  RmScreenGlobals.ysamples = y;
}
/*\end{verbatim} \label{Ri.c:RiPixelFilter} \begin{verbatim}
 * RiPixelFilter: Setzen der Filterfunktion und Parameter
 * 
 * 	INPUT :	function = gew"unschte Filterfuktion
 *		xwidth   = Filterausdehnung in X
 *		ywidth   = Filterausdehnung in Y
 */
void
RiPixelFilter(function,xwidth,ywidth)
        RtFloatFunc function;
        RtFloat     xwidth,ywidth;
{
  RmScreenGlobals.filterfunction = function;
  RmScreenGlobals.xwidth = xwidth;
  RmScreenGlobals.ywidth = ywidth;
}

/*\end{verbatim} \label{Ri.c:RiExposure} \begin{verbatim}
 * RiExposure: Transformation des berechneten Bildwertes nach der Formel: 
 *		output = (input*gain)^(1/gamma)
 *	Input: gain und gamma.
 */
void
RiExposure(gain,gamma)
RtFloat gain,gamma;
{
  RmGlobals.Exposure_gain = gain;
  RmGlobals.Exposure_gamma = gamma;
}

void
RiImager()
{
 /* $$$ */
}

/*\end{verbatim} \label{Ri.c:RiQuantize} \begin{verbatim}
 * RiQuantize: Quantisieren und Dithern des Bildwertes.
 *	Input: type wird ignoriert. Der Rest wie im RenderMan Companion
 *		 beschrieben.
 */
void
RiQuantize(type,one,min,max,ditheramplitude)
RtToken type;
RtInt   one,min,max;
RtFloat ditheramplitude;
{

  /* type is ignored */

  RmGlobals.Quant_one = one;
  if (min<0 || min>255) {
    warning(2,"[RiQuantize] parameter <min> out of range (ignored)");
  }else{
    RmGlobals.Quant_min = min;
  }

  if (max<0 || max>255) {
    warning(2,"[RiQuantize] parameter <max> out of range (ignored)");
  }else{
    RmGlobals.Quant_max = max;
  }

  RmGlobals.Quant_ditheramplitude = ditheramplitude;
}

/*\end{verbatim} \label{Ri.c:RiDisplay} \begin{verbatim}
 * RiDisplay: Ausgabeformat setzen.
 *
 *	INPUT :	name = Filename der Ausgabe
 *		type = Typ der Ausgabe (im Moment nur Datei)
 *		mode = Art der Ausgabe (im Moment nur RGB)
 *		va_alist = Paramter
 */
void
RiDisplay(name,type,mode,va_alist)
char *name;
RtToken type,mode;
va_dcl
{
  va_list argp;
  RtToken token;
  
  if (NEQ(type,RI_FILE)) {
    sprintf(RmMsg,"[RiDisplay] illegal value type = %s (ignored)",type);
    warning(2,RmMsg);     
  }
  if (NEQ(mode,RI_RGB)) {
    sprintf(RmMsg,"[RiDisplay] illegal value mode = %s (ignored)",mode);
    warning(2,RmMsg);
  }
  strcpy(RmGlobals.fname,name);

  va_start(argp);
  token=va_arg(argp,RtToken);
  if (token != RI_NULL) {
    sprintf(RmMsg,"[RiDisplay] No Parameterlist supported!");
    warning(2,RmMsg);
  }
}

void
RiHider()
{
 /* $$$ */
}

void
RiColorSamples()
{
 /* $$$ */
}

void
RiRelativeDetail()
{
 /* $$$ */
}

void
RiOption()
{
 /* $$$ */
}


/************************************************************************/
/*      LIGHTS                                                          */
/************************************************************************/
/*\end{verbatim} \label{Ri.c:RiLightSource} \begin{verbatim}
 * RiLightSource: LightShader ausw"ahlen.
 * 
 * 	INPUT :	name = Name des LightShaders
 *		va_alist = Instanzierungsliste dieses Shaders
 *	OUTPUT:	handle des LightShaders
 */
RtLightHandle
RiLightSource(name,va_alist)
  char *name;
  va_dcl
{
  LightShader   *(*a)();
  LightShader   *light;
  va_list       argptr;

  va_start(argptr);

  a = (LightShader *(*)())RmLookUpShader(name);     /* Adresse der Shaderfunction holen */
  if (a) {
    light=(*a)(argptr);
    if (RmGlobals.inworld) {  /* Innerhalb des WorldBlocks !*/
      if (RmGlobals.innerlightpos == NumberofLights) {
	error(1,"[RiLightSurface] No more inner LightShader possible !");
        return 0;
      } else { 
      RmGlobals.innerlightpos++;	/* Lichtquellenanzahl hochsetzen */
      RmGlobals.innerlights[RmGlobals.innerlightpos] = light;
	/* Und LightShader eintragen */ 
      RmGlobals.activattributes.innerlightstate[RmGlobals.innerlightpos]=1;
      return (RmGlobals.innerlightpos);
      }
    } else {	/* Au"serhalb des Worldblocks */
      if (RmGlobals.outerlightpos == NumberofLights) {
        error(1,"[RiLightSurface] No more outer LightShader possible !");
        return 0;
      } else {
      RmGlobals.outerlightpos++;	/* Lichtquellenanzahl hochsetzen */
      RmGlobals.outerlights[RmGlobals.outerlightpos] = light;
      RmGlobals.activattributes.outerlightstate[RmGlobals.outerlightpos]=1;
      return (RmGlobals.outerlightpos);
      }
    }
  } else {
        error(1,"[RiLightSurface] LightShader does not exist!");
 	return 0;
  }
  va_end(argptr);
  
}

void
RiAreaLightSource()
{
  /* $$$ */
}


/************************************************************************/
/*      ATTRIBUTES                                                      */
/************************************************************************/
void
RiAttribute()
{
 /* $$$ */
}
/*\end{verbatim} \label{Ri.c:RiColor} \begin{verbatim}
 * RiColor: Neue Farbe setzen.
 * 
 *	INPUT :	Cs = Farbvektor
 */
void
RiColor(Cs)
	RtColor	Cs;
{
  copyV3(Cs,RmGlobals.activattributes.Cs);
}
/*\end{verbatim} \label{Ri.c:RiOpacity} \begin{verbatim}
 * RiOpacity: Neue Transparenz setzen.
 *
 *	INPUT :	Os = Transparenzvektor
 */
void
RiOpacity(Os)
	RtColor Os;
{
  copyV3(Os,RmGlobals.activattributes.Os);
}

/* $$$ alle instanzierten Shader werden im Moment nicht freigegeben !!!
   , auch wenn sie garnicht benutzt worden sind!!
   Am Ende des Programms muss noch eine List, in die alle neuen
   Shader bei ihrer Generierung eingetragen werden muessen,
   durchlaufen werden, die dann alle freigibt !!! $$$
*/

/*\end{verbatim} \label{Ri.c:RiSurface} \begin{verbatim}
 * RiSurface: Instanzierung eines SurfaceShaders
 * 
 *	INPUT :	name = Name des SurfaceShaders
 *		va_alist = Instanzierungsparameter
 */
void
RiSurface(name,va_alist)
	char *name;
	va_dcl
{
  Shader        *(*a)();
  Shader   	*shptr;
  va_list	argptr;

  va_start(argptr);

  a = (Shader *(*)())RmLookUpShader(name);	/* Get Adress of Shaderfunction */
  if (a) { 
    shptr=(*a)(argptr);
    if(RmInvertM44(RmGlobals.activattributes.trans.OtoW,shptr->WtoS))
      internal(1,"[RiSurface] can`t compute inverse matrix");
	/* Shader als momentan aktiven in die Globals eitragen! */
    RmGlobals.activattributes.AllShader.SurfaceShader = shptr;
  } else {
        sprintf(RmMsg,"[RiSurface] Shader %s does not exist!",name);
        error(1,RmMsg);
  }
  va_end(argptr);
}
/*\end{verbatim} \label{Ri.c:RiAtmosphere} \begin{verbatim}
 * RiAtmosphere: Instanzierung eines AtmosphereShader
 *
 *      INPUT : name = Name des AtmosphereShader
 *              va_alist = Instanzierungsparameter
 */
void
RiAtmosphere(name,va_alist)
        char *name;
        va_dcl
{
  va_list       argptr;
  Shader        *(*a)();
  Shader        *shptr;

  va_start(argptr);               /* Also neuen generieren ! */
  a = (Shader *(*)())RmLookUpShader(name);     /* Get Adress of Shaderfunction */
  if (a) {
    shptr=(*a)(argptr);
    if(RmInvertM44(RmGlobals.activattributes.trans.OtoW,shptr->WtoS))
      internal(1,"[RiAtmosphere] can`t compute inverse matrix");
    RmGlobals.activattributes.AllShader.AtmosphereShader = shptr;
  } else {
	error(1,"[RiAtmosphere] Shader does not exist!");
  }
  va_end(argptr);

}
/*\end{verbatim} \label{Ri.c:RiInterior} \begin{verbatim}
 * RiInterior: Instanzierung eines Interior
 *
 *      INPUT : name = Name des Interior
 *              va_alist = Instanzierungsparameter
 */
void
RiInterior(name,va_alist)
        char *name;
        va_dcl
{
  va_list       argptr;
  Shader        *(*a)();
  Shader        *shptr;

  va_start(argptr);               /* Also neuen generieren ! */
  a = (Shader *(*)())RmLookUpShader(name);     /* Get Adress of Shaderfunction */
  if (a) {
    shptr=(*a)(argptr);
    if(RmInvertM44(RmGlobals.activattributes.trans.OtoW,shptr->WtoS))
      internal(1,"[RiInterior] can`t compute inverse matrix");
    RmGlobals.activattributes.AllShader.InteriorShader = shptr;
  } else {
	error(1,"[RiInterior] Shader does not exist!");
  }
  va_end(argptr);

}

/*\end{verbatim} \label{Ri.c:RiExterior} \begin{verbatim}
 * RiExterior: Instanzierung eines Exterior
 *
 *      INPUT : name = Name des Exterior
 *              va_alist = Instanzierungsparameter
 */
void
RiExterior(name,va_alist)
        char *name;
        va_dcl
{
  va_list       argptr;
  Shader        *(*a)();
  Shader        *shptr;

  va_start(argptr);               /* Also neuen generieren ! */
  a = (Shader *(*)())RmLookUpShader(name);     /* Get Adress of Shaderfunction */
  if (a) {
    shptr=(*a)(argptr);
    if(RmInvertM44(RmGlobals.activattributes.trans.OtoW,shptr->WtoS))
      internal(1,"[RiExterior] can`t compute inverse matrix");
    RmGlobals.activattributes.AllShader.ExteriorShader = shptr;
  } else {
	error(1,"[RiExterior] Shader does not exist!");
  }
  va_end(argptr);

}

void
RiIlluminate()
{
 /* $$$ */
}


void
RiDisplacement()
{
 /* $$$ */
}

void
RiTextureCoordinates()
{
 /* $$$ */
}

void
RiShadingRate()
{
 /* $$$ */
}

void
RiShadingInterpolation()
{
 /* $$$ */
}

void
RiMatte()
{
 /* $$$ */
}

void
RiBound()
{
 /* $$$ */
}

void
RiDetail()
{
 /* $$$ */
}

void
RiDetailRange()
{
 /* $$$ */
}

void
RiGeometricApproximation()
{
 /* $$$ */
}
/*\end{verbatim} \label{Ri.c:RiOrientation} \begin{verbatim}
 * RiOrientation: Orientierung setzen
 * 
 *	INPUT :	orient = Links- oder Rechtsh"andig
 */
void
RiOrientation(orient)
	RtToken	orient;
{
  if ((EQ(orient,RI_LH)) || (EQ(orient,RI_RH)))
     RmGlobals.activattributes.orientation = orient;
  else {
    sprintf(RmMsg,"[RiOrientation] illegal value orientation = %s (ignored)",
            orient);
    warning(2,RmMsg);
  }
}
/*\end{verbatim} \label{Ri.c:RiReverseOrientation} \begin{verbatim}
 * RiReverseOrientation: Orientierung umdrehen
 */
void
RiReverseOrientation()
{
  if (EQ(RmGlobals.activattributes.orientation,RI_LH))
	RmGlobals.activattributes.orientation = RI_RH;
  else
	RmGlobals.activattributes.orientation = RI_LH;
}
/*\end{verbatim} \label{Ri.c:RiSides} \begin{verbatim}
 * RiSides: Anzahl der sichtbaren Seiten einer Fl"ache.
 *
 *	INPUT :	nsides = 1 oder 2
 */
void
RiSides(nsides)
	RtInt nsides;
{
  if ((nsides == 1) || (nsides == 2))
     RmGlobals.activattributes.sides = nsides;
  else {
    sprintf(RmMsg,"[RiSides] illegal value sides = %d (ignored)",nsides);
    warning(2,RmMsg);
  }
}

void
RiBasis()
{
 /* $$$ */
}

void
RiTrimCurve()
{
 /* $$$ */
}



/************************************************************************/
/*      TRANSFORMATIONS                                                 */
/************************************************************************/
/*\end{verbatim} \label{Ri.c:RiIdentity} \begin{verbatim}
 * RiIdentity: Die ObjecttoWorld Matrix auf Einheitsmatrix bringen.
 */
void
RiIdentity()
{
  RmIdentityM44(RmGlobals.activattributes.trans.OtoW);
}
/*\end{verbatim} \label{Ri.c:RiTransform} \begin{verbatim}
 * RiTransform: OtoW-Matrix neu besetzen.
 *
 * 	INPUT :	mat = Transformationsmatrix
 */
void
RiTransform(mat)
	RtMatrix mat;
{
  RmCpyM44(mat,RmGlobals.activattributes.trans.OtoW);
}
/*\end{verbatim} \label{Ri.c:RiConcatTransform} \begin{verbatim}
 * RiConcatTransform: Neue Transformation an OtoW-Matrix anh"angen.
 *
 *	INPUT : t = Neue Transformation
 */
void
RiConcatTransform(t)
	RtMatrix t;
{
  RmMulM44(RmGlobals.activattributes.trans.OtoW,t,
           RmGlobals.activattributes.trans.OtoW);
}

void
RiPerspective()
{
 /* $$$ */
}
/*\end{verbatim} \label{Ri.c:RiTranslate} \begin{verbatim}
 * RiTranslate: Translation der OtoW-Matrix
 * 
 * 	INPUT : dx,dy,dz = Translation um dx,dy,dz
 */
void
RiTranslate(dx,dy,dz)
	RtFloat	dx,dy,dz;
{
  RmTransM44(RmGlobals.activattributes.trans.OtoW,dx,dy,dz); 
}
/*\end{verbatim} \label{Ri.c:RiRotate} \begin{verbatim}
 * RiRotate: Rotation der OtoW-Matrix
 *
 *	INPUT : angle = Rotationwinkel
 *		dx,dy,dz = Rotationsachse
 */
void
RiRotate(angle,dx,dy,dz)
	RtFloat angle,dx,dy,dz;
{
  if (angle==0.) {
    warning(1,"[RiRotate] zero-angle specified"); return;
  }
  if (dx == 0. && dy == 0. && dz == 0.) {
    error(1,"[RiRotate] dx=dy=dz=0");
  }
  angle=DegToRad(angle);
  RmRotM44(RmGlobals.activattributes.trans.OtoW,dx,dy,dz,angle);
}
/*\end{verbatim} \label{Ri.c:RiScale} \begin{verbatim}
 * RiScale: Skalierung der OtoW-Matrix.
 *
 * 	INPUT : sx,sy,sz = Skalierungsfaktoren
 */
void
RiScale(sx,sy,sz)
	RtFloat sx,sy,sz;
{
  RmScalM44(RmGlobals.activattributes.trans.OtoW,sx,sy,sz);
}

void
RiSkew()
{
 /* $$$ */
}

void
RiDeformation()
{
 /* $$$ */
}
/*\end{verbatim} \label{Ri.c:RiCoordinateSystem} \begin{verbatim}
 * RiCoordinateSystem: Momentanes koordinatensystem mit Namen versehen.
 *
 *	INPUT : space = Zu vergebender Name
 */
void
RiCoordinateSystem(space)
	RtToken space;
{
  transform *mat;

  mat = (transform *)RmMalloc(sizeof(transform));
  RmCpyM44(RmGlobals.activattributes.trans.OtoW,mat->OtoW);
  if (RmDeclareTrafo(space,mat)) {
	warning(1,"[RiCoordinateSystem] Name not possible!");
	RmFree(mat);
  }
}
/*\end{verbatim} \label{Ri.c:RiTransFormPoints} \begin{verbatim}
 * RiTransformPoints: Transformation von Punkten zwischen zwei
 *		      Koordinatensystemen.
 *	
 *	INPUT :	fromspace = Ursprungskoordinatensystem
 *		tospace   = Zielkoordinatensystem
 *		npoints   = Anzahl der Punkte
 *		points    = Liste der Punkte
 */
void
RiTransformPoints(fromspace,tospace,npoints,points)
	RtToken fromspace,tospace;
	RtInt	npoints;
	RtPoint points[];
{
  transform *atrans,*btrans;
  RtMatrix  invmat;    
  RtInt     i;

  atrans = RmLookUpTrafo(fromspace);
  if (!atrans) error(1,"[RiTransformPoints] Name not found!");
  btrans = RmLookUpTrafo(tospace);
  if (!btrans) error(1,"[RiTransformPoints] Name not found!");
  if (RmInvertM44(btrans->OtoW,invmat))
   internal(1,"[RiTransformPoints] can`t compute inverse matrix");
  RmMulM44(atrans->OtoW,invmat,invmat);	/* invmat = Konvert.Matrix */
  for (i=0;i<npoints;i++) 
	RmTransformPoint(invmat,points[i],points[i]);
}



/************************************************************************/
/*      POLYGONS                                                        */
/************************************************************************/
/*\end{verbatim} \label{Ri.c:RiPolygon} \begin{verbatim}
 * RiPolygon: Einf"ugen eines Polygons in den Objektbaum.
 * 
 *	INPUT : nvertics = Anzahl der Eckpunkte
 *		va_alist = Parameterliste
 */
void
RiPolygon(nvertics,va_alist)
  RtInt nvertics;
  va_dcl
{
  Prim          *primitive;
  va_list	argptr;

  va_start(argptr);
  primitive = Rm_GenPolygon(nvertics,argptr);/* Polygonobjekt erzeugen */
  RmFillPrim(primitive);        /* Fill rest of Primstruct */
  RmChainPrim(primitive);       /* Chain it to Objecttree (same for all types */
  va_end(argptr);
}

void
RiGeneralPolygon()
{
 /* $$$ */
}
/*\end{verbatim} \label{Ri.c:RiPointsPolygon} \begin{verbatim}
 * RiPointsPolygon: Polygonnetz in den Objektbaum einf"ugen.
 *
 * 	INPUT : npolys = Anzahl der Polygone
 *		nverts = Anzahl der unterschiedlichen Eckpunkte
 *		verts  = Liste der Eckpunkte
 *		va_alsit = Paramterliste
 */
void
RiPointsPolygon(npolys,nverts,verts,va_alist)
  RtInt npolys,nverts[],verts[];
  va_dcl
{
  Prim          *primitive;
  va_list       argptr;

  va_start(argptr);
  /* Polygonnetzobjekt erzeugen */
  primitive = Rm_GenPolyNet(npolys,nverts,verts,argptr);
  RmFillPrim(primitive);        /* Fill rest of Primstruct */
  RmChainPrim(primitive);       /* Chain it to Objecttree (same for all types */
  va_end(argptr);
}

void
RiPointsGeneralPolygon()
{
 /* $$$ */
}


/************************************************************************/
/*      PATCHES                                                         */
/************************************************************************/
void
RiPatch()
{
 /* $$$ */
}

void
RiPatchMesh()
{
 /* $$$ */
}

void
RiNuPatch()
{
 /* $$$ */
}


/************************************************************************/
/*      QUADRICS                                                        */
/************************************************************************/
/*\end{verbatim} \label{Ri.c:RiSphere} \begin{verbatim}
 * RiSphere: Kugel in Objektbaum einf"ugen.
 *
 *	INPUT : radius = Kugelradius
 *		zmin,zmax = zum "kappen" der Kugelkuppen
 *		thetamax = "Offnungswinkel
 */
void
RiSphere(radius,zmin,zmax,thetamax,va_alist)
	RtFloat	radius;
	RtFloat zmin,zmax;
	RtFloat thetamax;
	va_dcl
{
  va_list	argp;
  Prim		*primitive;

  va_start(argp);

  primitive = Rm_GenSphere(radius,zmin,zmax,thetamax);
  RmFillPrim(primitive);	/* Fill rest of Primstruct */
  RmChainPrim(primitive);	/* Chain it to Objecttree (same for all types */

  va_end(argp);
}

void
RiCone()
{
 /* $$$ */
}

void
RiDisk(height,radius,thetamax,va_alist)
	RtFloat height;
        RtFloat radius;
        RtFloat thetamax;
        va_dcl
{
    va_list       argp;
  Prim          *primitive;

  va_start(argp);

  primitive = Rm_GenDisk(height,radius,thetamax);
  RmFillPrim(primitive);        /* Fill rest of Primstruct */
  RmChainPrim(primitive);       /* Chain it to Objecttree (same for all types */

  va_end(argp);
}

void
RiCylinder(radius,zmin,zmax,thetamax,va_alist)
        RtFloat radius;
        RtFloat zmin,zmax;
        RtFloat thetamax;
        va_dcl
{
  va_list       argp;
  Prim          *primitive;

  va_start(argp);

  primitive = Rm_GenCylinder(radius,zmin,zmax,thetamax);
  RmFillPrim(primitive);        /* Fill rest of Primstruct */
  RmChainPrim(primitive);       /* Chain it to Objecttree (same for all types */

  va_end(argp);
}

void
RiHyperboloid()
{
 /* $$$ */
}

void
RiParaboloid()
{
 /* $$$ */
}


void
RiTorus()
{
 /* $$$ */
}


/************************************************************************/
/*      GENERAL OBJECTS                                                 */
/************************************************************************/
void
RiObjectInstance()
{
 /* $$$ */
}

void
RiProcedural()
{
 /* $$$ */
}

void
RiGeometry()
{
 /* $$$ */
}


/************************************************************************/
/*      MAP-MAKING                                                      */
/************************************************************************/
void
RiMakeTexture()
{
 /* $$$ */
}

void
RiMakeBump()
{
 /* $$$ */
}

void
RiMakeLatLongEnvironment()
{
 /* $$$ */
}

void
RiMakeCubeFaceEnvironment()
{
 /* $$$ */
}

void
RiMakeShadow()
{
 /* $$$ */
}

/* \end{verbatim} */
