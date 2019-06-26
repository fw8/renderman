/* \subsection{PrimSphere.c} \label{PrimSphere.c}
  Dieses File enth"alt alle Routinen, die zur Verwaltung und Berechnung
  des Objekts Kugel n"otig sind!
\begin{verbatim} */
#include "Prim.h"
#include "Math.h"

typedef struct {	/* SPHERE (pointed to by prim->data) */
	RtFloat radius,radius2;	/* radius and radius^2 */
	RtFloat zmin,zmax,asin_zmin,asin_zmin_pl_asin_zmax;
	RtFloat thetamax;
} Sph;

typedef struct {
        RtPoint hp;  /* hit point */
        Sph    *o;  /* private data */
} Param;


/* \end{verbatim} \label{PrimSphere.c:SphName} \begin{verbatim}
 * SphName: Gibt den Namen dieses Objektes zur"uck.
 *
 */
static char *
SphName()
{
	return "sphere" ;
}
/* \end{verbatim} \label{PrimSphere.c:SphInfo} \begin{verbatim}
 * SphInfo: Gibt Informationen "uber dieses Objektes zur"uck.
 *
 */
static void
SphInfo(prim,stream)
Prim *prim;
FILE *stream;
{
        Sph *o;

        o = (Sph *)prim->data;
        fprintf(stream,"sphere: radius %g,\
\n        z[min,max] %g,%g,\n        thetamax %g\n",
             o->radius,o->zmin,o->zmax,o->thetamax);
}

/*
 * SphBound: give bounds of object (in object coordinates).
 */
static void
SphBound(prim,bound)
Prim *prim;
RtBound bound;
{
  Sph *o = (Sph *)prim->data;

  /* lower edge */
  bound[0] = - o->radius;
  bound[1] = - o->radius;
  bound[2] =   o->zmin;

  /* higher edge */
  bound[3] =   o->radius;
  bound[4] =   o->radius;
  bound[5] =   o->zmax;
}


/* \end{verbatim} \label{PrimSphere.c:SphIntersect} \begin{verbatim}
 * SphIntersect: intersects ray with sphere prim and put intersections in
 * hit list
 *              INPUT : ray =  Zeiger auf Strahl, der schneiden soll
 *                      prim = Zeiger auf das Primitiv
 *                      hit  = Zeiger auf die Hit-Struktur
 *              OUTPUT: 0 = kein Schnitt gefunden
 *                      sonst Anzahl der Schnitte
 */
static int
SphIntersect(ray,prim,hit)
Ray   *ray;
Prim  *prim;
Isect *hit;
{
	int nroots=0;
	RtFloat b,di,t1,t2,DD;
	RtPoint V,hp;
	RtFloat *P;
	Sph *o=(Sph *)prim->data;
	Param *param;

	mulV3(ray->P,-1.,V);  /* V goes from ray->P to the center */
                              /* of the sphere */
	DD=smulV3(ray->D,ray->D);

	b = smulV3(V,ray->D);
	di = b*b-(smulV3(V,V)-o->radius2)*DD;
	if (di<=0.) return 0;		/* doesn`t hit */
	di = sqrt(di);
	t2 = (b+di)/DD;			/* 2nd root */
	if (t2<=RI_EPSILON) return 0;	/* behind ray origin */

	t1 = (b-di)/DD;			/* 1st root */
	RayPoint(ray,t1,hp);	/* 1st hitpoint */
      if (t1>RI_EPSILON && hp[2] < o->zmax && hp[2] > o->zmin &&
            intervall(hp[0],hp[1],o->thetamax)) {
                param=(Param *)RmGetJunk(sizeof(Param));
                copyV3(hp,param->hp);
                param->o=o;
                IsectAdd(hit,t1,prim,param); /* entering cylinder */
                hit++;
                nroots++;
        }
        RayPoint(ray,t2,hp);    /* 2nd hitpoint */
        if (hp[2] < o->zmax && hp[2] > o->zmin &&
            intervall(hp[0],hp[1],o->thetamax)) {
                param=(Param *)RmGetJunk(sizeof(Param));
                copyV3(hp,param->hp);
                param->o=o;
                IsectAdd(hit,t2,prim,param); /* exiting cylinder */
                nroots++;
        }
        return nroots;
}

static int
intervall(x,y,w)
RtFloat x,y,w;
{
   RtFloat  d;

   if (x == 0.0) d = PId2*sgn(y);
   else{
      d = atan(y/x);
      if (x<0.) d = d+PI;
   }

   if (d<0.) d = d+PIm2;

   if (d<=w) return 1;
   else      return 0;
}


/*
 * SphSetup: give shader what he needs.
 */
static void
SphSetup(hit,iof,iob)
Isect *hit;
IOF *iof;
IOB *iob;
{
  RtFloat rho,theta;
  Param *param=(Param *)hit->data;
        
  unitV3(param->hp,param->hp);

  /* surface properties */

  if (iof->N || iof->Ng) {                      /* apply a normal */
    copyV3(param->hp,iob->N);
  }
  if (iof->Ng) copyV3(iob->N,iob->Ng);          /* apply a geom. normal */

  /* parameter space mapping */

  if (iof->v) {
    iob->v=(asin(Vz(param->hp)) - param->o->asin_zmin) /
           (param->o->asin_zmin_pl_asin_zmax);
  }

  if (iof->u) {
    if (Vx(param->hp) == 0.0) theta = PId2*sgn(Vy(param->hp));
    else{
      theta = atan(Vy(param->hp)/Vx(param->hp));
      if (Vx(param->hp)<0.) theta = theta+PI;
    }
    if (theta<0.) theta = theta+PIm2;
    iob->u=theta/param->o->thetamax; /* PIm2; */
  }

}


static PrimProcs SphProgs = {
	SphName,
	SphInfo,		/* Die Funktionen f"ur dieses Objekt */
	SphBound,
	SphIntersect,
	SphSetup
};
/* \end{verbatim} \label{PrimSphere.c:Rm_GenSphere} \begin{verbatim}
 * Rm_GenSphere: Generiert aus den "ubergebenen Parametern eine neue
 *                Sphere!
 *
 *              INPUT : radius = radius der Kugel
 *			zmin   = untere Kappe
			zmax   = obere Kappe
			thetamax = "Offnungswinkel
 *              OUTPUT: Zeiger auf die neue Objektstruktur
 */
Prim *
Rm_GenSphere(radius,zmin,zmax,thetamax)
RtFloat radius;
RtFloat zmin,zmax;
RtFloat thetamax;
{
	Sph *o;
	Prim *p;
	
	o = (Sph *)RmMalloc(sizeof(Sph));	/* Platz machen f"ur Kugel */
	p = (Prim *)RmMalloc(sizeof(Prim));	/* und Objektstruktur */
	
	o->radius2=(o->radius=radius)*radius;   /* Werte eintragen */
	o->zmin=zmin;
	o->zmax=zmax;
	o->asin_zmin=asin(zmin);
	o->asin_zmin_pl_asin_zmax=asin(zmax)-o->asin_zmin;
	o->thetamax=DegToRad(thetamax);
	
	p->type=PRIMITIVE;			/* Und in die Objektstruktur */
	p->data=(char *)o;				/* einbinden */
	p->procs= &SphProgs;
	return p;
}
