/* \subsection{Shader.h} \label{Shader.h}
 * Shader : typedefs and defines needed for Shaders.
\begin{verbatim}*/

#ifndef _Shader_
#define _Shader_

#include "RiTypes.h"

/*
 * IOB : I/O-Block, all global values a shader can use.
 */
typedef struct {
        RtColor Cs;
        RtColor Os;
	RtPoint P;
	RtPoint dPdu,dPdv;
	RtPoint N;
	RtPoint Ng;
	RtPoint Ps;
	RtFloat u,v;
	RtFloat du,dv;
	RtFloat s,t;
	RtPoint L;
	RtColor CI;
	RtPoint I;
	RtColor Ci;
	RtColor Oi;
	RtColor E;
        RtFloat A;
        } IOB;

/*
 * IOF : I/O-Flags, all global values a shader realy needs.
 */
typedef struct {
	RtBoolean Cs;
	RtBoolean Os;
	RtBoolean P;
	RtBoolean sPdu,dPdv;
	RtBoolean N;
	RtBoolean Ng;
	RtBoolean Ps;
	RtBoolean u,v;
	RtBoolean du,dv;
	RtBoolean s,t;
	RtBoolean L;
	RtBoolean CI;
	RtBoolean I;
	RtBoolean Ci;
	RtBoolean Oi;
	RtBoolean E;
	RtBoolean A;
        } IOF;

/*
 * ShaderType : type of Shader.
 */
typedef enum {SURFACE,
	      ATMOSPHERE,
              INTERIOR,
              EXTERIOR,
	      DISPLACEMENT } ShaderType;

/*
 * Shader : Shader-Object-Structure.
 */
typedef struct Shader {         /* A SURFACE AND SHADING DESCRIPTION */
        ShaderType type;	/* type of Shader */

        char   *data;           /* ptr to mat-depent data (must be cast) */
        struct ShadeProcs *procs; /* generic procedures for this mat-type */
	RtMatrix WtoS;		/* world to shader-space transformation */
        } Shader;

/*
 * ShadeProcs : Methods of a shader.
 */
typedef struct ShadeProcs {     /* GENERIC PROCEDURES FOR SHADING-MODULES */
        char *(*name)();        /* shading type name */
        void (*info)();         /* print info */
        IOF *(*support)();      /* need support of flagged values */
        void (*shade)();        /* do shading */
        } ShadeProcs;

/*
 * LightShader : LightShader-Object-Structure.
 */
typedef struct LightShader {    /* A SURFACE AND SHADING DESCRIPTION */
        char   *data;           /* ptr to mat-depent data (must be cast) */
        struct LightShadeProcs *procs; /* generic procedures */
        } LightShader;

/*
 * LightShadeProcs : Methods of a lightshader.
 */
typedef struct LightShadeProcs {/* GENERIC PROCEDURES FOR SHADING-MODULES */
        char *(*name)();        /* shading type name */
        void (*info)();         /* print info */
        int  (*illuminate)();   /* illuminate */
	} LightShadeProcs;


typedef struct {            /* All shaders */
        Shader  *SurfaceShader;
        Shader  *AtmosphereShader;
        Shader  *InteriorShader;
        Shader  *ExteriorShader;
        Shader  *DisplacementShader;
} ShaderSet;

typedef struct LightSet {
	LightShader *light;
	struct LightSet *next;  /* next in list */
} LightSet;

/*
 * Mathematische Funktionen.
 */
extern RtFloat Rm_sin();
extern RtFloat Rm_cos();
extern RtFloat Rm_tan();
extern RtFloat Rm_radians();
extern RtFloat Rm_degrees();
extern RtFloat Rm_sqrt();
extern RtFloat Rm_pow();
extern RtFloat Rm_mod();
extern RtFloat Rm_abs();
extern RtFloat Rm_min();
extern RtFloat Rm_max();
extern RtFloat Rm_floor();
extern RtFloat Rm_step();
extern RtFloat Rm_smoothstep();

/*
 * Shading, Coloring and Lightning Functions.
 */
extern RtVoid Rm_ambient();
extern RtVoid Rm_diffuse();
extern RtVoid Rm_specular();
extern RtVoid Rm_trace();

/*
 * Geometrische Funktionen.
 */
extern RtFloat Rm_xcomp();
extern RtFloat Rm_ycomp();
extern RtFloat Rm_zcomp();
extern RtVoid  Rm_setxcomp();
extern RtVoid  Rm_setycomp();
extern RtVoid  Rm_setzcomp();
extern RtFloat Rm_distance();
extern RtVoid  Rm_normalize();
extern RtVoid  Rm_faceforward();
extern RtVoid  Rm_reflect();

/*
 * Farbfunktionen.
 */
extern RtVoid Rm_mix();

/*
 * Noisefunktionen.
 */
extern RtFloat Rm_noisefV();
extern RtVoid  Rm_noiseVV();
extern RtFloat Rm_noiseff();

/*
 * Texturfunktionen.
 */
#ifndef _AdminGlobals_
extern RtVoid Rm_textureV();
#endif


extern RtVoid  Rm_printf();

#endif _Shader_
/*\end{verbatim}*/
