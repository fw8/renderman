/* \subsection{Prim.h} \label{Prim.h}
	HeaderFile f"ur alle Prim-Module.
\begin{verbatim} */

#ifndef _Prim_
#define _Prim_

#include <stdio.h>

#include "RiTypes.h"
#include "Shader.h"
#include "MathVector.h"
#include "MathTrafo.h"

typedef struct {
    RtFloat u,v;
} Vec2d;
	
typedef enum {INTERSECTION,
              UNION,
              DIFFERENCE} CompType;
              
typedef enum {TOPNODE,
	      BOUNDINGBOX,
	      COMPOSITE,
	      SOLID,
              PRIMITIVE} ObjType;

typedef struct BBox {/* AN AXIS ALLIGNED BOUNDING BOX */
	ObjType type;

	RtBound bound;		/* bounding volume for all objects */
	struct Comp *csg_tree;  /* in this tree */
	} BBox;

typedef struct Topnode {/* A TOP-NODE */
        ObjType type;

        RtFloat mailbox;        /* cache for hitpoints */
        struct Comp *csg_tree;  /* CSG-Tree starts here */
        struct Topnode *next;   /* next tree for this voxel */
        } Topnode;

typedef struct Comp {   /* A COMPOSITE SOLID */
        ObjType type;

        CompType op;
        struct Comp *left;
        struct Comp *right;
        } Comp;

typedef struct Solid {	/* A SOLID (from a set of surface primitives) */
	ObjType type;

	struct Prim *prim;
	struct Solid *next;
	} Solid;

typedef struct {
        RtColor         Cs;             /* Current Surface Color */
        RtColor         Os;             /* Current Surface Opacity */
        RtFloat         s1,t1,s2,t2,s3,t3,s4,t4;/* Texture Coordinates */
        RtInt           sides;          /* Number of visible Sides */
        RtInt           ustep,vstep;    /*  " step values in u,v direction */
} Attrib;   /*  What a object really needs */

typedef struct Prim {	/* A PRIMITIVE SOLID */
	ObjType type;
	
	char   *data;		/* ptr to prim-depent data (must be cast) */
	struct PrimProcs *procs;/* generic procedures for this prim-type */
	RtMatrix  WtoO;		/* world to object transformation */
	RtMatrix  OtoW;		/* object to world transformation */
	ShaderSet shader;       /* surface and shading description */
	LightSet  *lightset;	/* array of lightsources */
	RtInt     lights;   	/* number of lightsources */
	Attrib    attrib;	/* object attributes */
	} Prim;

typedef struct PrimProcs {      /* GENERIC PROCEDURES ON PRIMITIVES */
        char *(*name)();        /* primitive type name */
        void (*info)();         /* print info */
        void (*bound)();	/* give object bounds */
	int (*intersect)();	/* intersect with ray */
	void (*setup)();	/* setup shading-data */	 
	} PrimProcs;
	
typedef struct Isect {	/* AN INTERSECTION POINT */
	RtFloat t;	/* line parameter at intersection (as in P+tD) */
	Prim *prim;	/* primitive we hit */
        char *data;	/* Special data the prim want's to transfer to SetUp */
	} Isect;

#define IsectAdd(h,f,p,d) {		\
		h->t=(f);		\
		h->prim=p;		\
		h->data=(char *)(d);  }



#define ISECTMAX 20
#define MAXJUNKSIZE 1024

extern char *RmGetJunk();
extern void RmSortHitList();
extern Prim *Rm_GenSphere();
extern Prim *Rm_GenDisk();
extern Prim *Rm_GenCylinder();
extern Prim *Rm_GenPolygon();
extern Prim *Rm_GenPolyNet();

#endif _Prim_

/*\end{verbatim}*/
