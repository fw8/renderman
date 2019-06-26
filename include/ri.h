/*
 *   RenderMan Interface Standard Include File
 */
#ifndef _ri_
#define _ri_
 
	/* Definitions of Abstract Types used in RI */

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

typedef void		*RtPointer;
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

	/* Extern Declaration for Predefined RI Data Structures */		

extern RtToken	RI_FRAMEBUFFER,RI_FILE;
extern RtToken	RI_RGB,RI_RGBA,RI_RGBZ,RI_RGBAZ,RI_A,RI_Z,RI_AZ;
extern RtToken	RI_MERGE,RI_ORIGIN;
extern RtToken	RI_PERSPECTIVE,RI_ORTHOGRAPHIC;
extern RtToken	RI_HIDDEN,RI_PAINT;
extern RtToken	RI_CONSTANT,RI_SMOOTH;
extern RtToken	RI_FLATNESS,RI_FOV;

extern RtToken	RI_AMBIENTLIGHT,RI_POINTLIGHT,
		RI_DISTANTLIGHT,RI_SPOTLIGHT;
extern RtToken	RI_INTENSITY,RI_LIGHTCOLOR,RI_FROM,RI_TO,
		RI_CONEANGLE,RI_CONEDELTAANGLE,
		RI_BEAMDISTRIBUTION;
extern RtToken	RI_MATTE,RI_METAL,RI_PLASTIC;
extern RtToken	RI_KA,RI_KD,RI_KS,RI_ROUGHNESS,
		RI_SPECULARCOLOR;
extern RtToken	RI_DEPTHCUE,RI_FOG;
extern RtToken	RI_MINDISTANCE,RI_MAXDISTANCE,
		RI_BACKGROUND,RI_DISTANCE;
	
extern RtToken	RI_RASTER,RI_SCREEN,RI_CAMERA,RI_WORLD,
		RI_OBJECT;
extern RtToken	RI_INSIDE,RI_OUTSIDE,RI_LH,RI_RH;
extern RtToken	RI_P,RI_PZ,RI_PW,RI_N,RI_NP,RI_CS,RI_OS,
		RI_S,RI_T,RI_ST;
extern RtToken	RI_BILINEAR,RI_BICUBIC;
extern RtToken	RI_PRIMITIVE,RI_INTERSECTION,RI_UNION,
		RI_DIFFERENCE;
extern RtToken	RI_WRAP,RI_NOWRAP,RI_PERIODIC,RI_CLAMP,
		RI_BLACK;

extern RtBasis	RiBezierBasis,RiBSplineBasis,RiCatmullRomBasis,
		RiHermiteBasis,RiPowerBasis;

#define RI_BEZIERSTEP		((RtInt)3)
#define RI_BSPLINESTEP		((RtInt)1)
#define RI_CATMULLROMSTEP	((RtInt)1)
#define RI_HERMITESTEP		((RtInt)2)
#define RI_POWERSTEP		((RtInt)4)

extern RtInt 	RiLastError;

extern RtToken 	RiDeclare();

extern RtVoid 	RiBegin(), RiEnd(),
		RiWorldBegin(), RiWorldEnd();

extern RtVoid	RiFormat(), RiFrameAspectRatio(), RiScreenWindow(),
		RiCropWindow(), RiProjection(),
		RiClipping();

extern RtVoid	RiPixelSamples(), RiPixelFilter();

extern RtFloat	RiGaussianFilter(), RiBoxFilter();

extern RtVoid	RiAttributBegin(), RiAttributEnd(),
		RiColor(), RiOpacity();

extern RtVoid	RiSurface(),
		RiAtmosphere(),
		RiInterior(), RiExterior();

extern RtVoid	RiOrientation(), RiSides();

extern RtVoid	RiIdentity(), RiTransform(), RiConcatTransform(),
		RiTranslate(), RiRotate(), RiScale(),
		RiCoordinateSystem();

extern RtPoint	*RiTransformPoints();

extern RtVoid	RiTransformBegin(), RiTransformEnd();

extern RtVoid	RiSphere();

extern RtVoid	RiSolidBegin(), RiSolidEnd();

extern RtVoid	RiErrorHandler();

extern RtVoid	RiErrorIgnore(), RiErrorPrint(), RiErrorAbort();

#endif _ri_
