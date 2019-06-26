/*\subsection{AdminGlobals.h} \label{AdminGlobals.h}
 * Verwaltung aller globalen Variablen.
\begin{verbatim}*/
#ifndef _AdminGlobals_
#define _AdminGlobals_

#define	NumberofLights 128

#include "RiTypes.h"
#include "Shader.h"
#include "AdminTree.h"
#include "AdminStack.h"

typedef struct {
	RtMatrix 	OtoW;
} transform;

typedef struct {
	RtColor		Cs;		/* Current Surface Color */
	RtColor		Os;		/* Current Surface Opacity */
	ShaderSet	AllShader;	/* All current shaders */
	RtBoolean	outerlightstate[NumberofLights];/* State out Lights */
	RtBoolean	innerlightstate[NumberofLights];/* State inner Lights */
	RtFloat		s1,t1,s2,t2,s3,t3,s4,t4;/* Texture Coordinates */
	RtFloat		shadingrate;	/* Shading Rate */
	RtToken		shadinginterpolation;	/* "constnant" or "smooth" */
	RtBoolean	matte;		/* On or Off */
	RtBound		boundingbox;	/* Current bounding Box */
	RtBound		detail;		/* Box for calc. Level of Detail */
	RtFloat		offlow,onlow,onhigh,offhigh; /* Current Detail Range */
	RtToken		gatype;		/* Type of Geometric Approximation */
	RtFloat		gavalue;	/* Value of  "            "        */
	RtToken		orientation;	/* Coordinate Orientation */
	RtInt		sides;		/* Number of visible Sides */
	RtBasis		ubasis,vbasis;	/* Current basis matrices */
	RtInt		ustep,vstep;	/*  " step values in u,v direction */
	transform       trans;		/* Current transformation */	
} attributes;	/* All attributes of environment */


typedef struct {
        RtString        rendermethod; /* Render Method */
	RtFunc		GenRay; /* method of projection */
        RtMatrix	WtoC; /* World to Camera Matrix */
        RtMatrix	CtoW; /* Camera to World Matrix */
        Topnode		*ModelRoot;	/* Begin of ObjectTree */
	Topnode		*currenttopnode; /* Current Topnode */
        RtInt		lightnumber; /* Number of current switched on lights */
 	RtInt		innerlightpos; /* Index to last inner light */
	RtInt		outerlightpos; /* Index to last outer light */
        LightShader	*innerlights[NumberofLights]; /* List of inner Lights */
        LightShader	*outerlights[NumberofLights]; /* List of outer Lights */
	stackhandle	*attributstack;
	stackhandle     *solidblockstack;
	attributes	activattributes;
	SBlock  	solidblock;
	int		MaxLevel;
        int		inworld;
        char		fname[80];

	/* Parameters of RiExposure() call */
        RtFloat		Exposure_gain;
	RtFloat		Exposure_gamma;

	/* Parameters of RiQuantisize() call */
	RtToken		Quant_type;
	RtInt		Quant_one, Quant_min, Quant_max;
	RtFloat		Quant_ditheramplitude;

} Globals;


typedef struct{
	enum{ATTRIBUT,TRANSFORMATION} tag;
	union {
		attributes	*attr;
		transform	*trans;
	} un;
} attributstackunion;

extern Globals     RmGlobals;

#endif _AdminGlobals_

/*\end{verbatim}*/
