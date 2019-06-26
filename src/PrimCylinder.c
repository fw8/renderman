/*
 * PrimCylinder.c : All the stuff to calculate the intersection between the
 *		ray and a cylinder.
 */

#include "Prim.h"
#include "Math.h"

typedef struct {	/* Cylinder (pointed to by prim->data) */
	RtFloat radius,radius2;	/* radius and radius^2 */
	RtFloat zmin,zmax;
	RtFloat thetamax;	
} Cyl;

typedef struct {
        RtPoint hp;  /* hit point */
        Cyl    *o;  /* private data */
} Param;

/*
 * CylName: returns name of this object.
 *
 */
static char *
CylName()
{
	return "cylinder" ;
}

/*
 * CylInfo: print out contents of instance-variables.
 *
 */
static void
CylInfo(prim,stream)
Prim *prim;
FILE *stream;
{
        Cyl *o;

        o = (Cyl *)prim->data;
        fprintf(stream,"cylinder: radius %g,\
\n        z[min,max] %g,%g,\n        thetamax %g\n",
             o->radius,o->zmin,o->zmax,o->thetamax);
}

/*
 * CylBound: give bounds of object (in object coordinates).
 */
static void
CylBound(prim,bound)
Prim *prim;
RtBound bound;
{
  Cyl *o = (Cyl *)prim->data;

  /* lower edge */
  bound[0] = - o->radius;
  bound[1] = - o->radius;
  bound[2] =   o->zmin;

  /* higher edge */
  bound[3] =   o->radius;
  bound[4] =   o->radius;
  bound[5] =   o->zmax;
}


/*
 * CylIntersect: intersects ray with cylinder prim and put intersections in
 * hit list. Returns No. of intersections.
 */
static int
CylIntersect(ray,prim,hit)
Ray   *ray;
Prim  *prim;
Isect *hit;
{
	int nroots=0;
	RtFloat p,q,di,t1,t2,DD;
	RtPoint V,hp;
	Cyl *o=(Cyl *)prim->data;
        Param *param;

   DD = sqr(Vx(ray->D)) + sqr(Vy(ray->D));

   if (DD)
      p = ((Vx(ray->P) * Vx(ray->D)) + (Vy(ray->P) * Vy(ray->D))) / DD;
      q = (sqr(Vx(ray->P)) + sqr(Vy(ray->P)) - o->radius2 ) / DD;
      di = sqr(p) - q;
      if (di<=0.) return 0;           /* doesn`t hit */
      t2 = -p + sqrt(di);             /* 2nd root */
      if (t2<=RI_EPSILON) return 0;   /* behind ray origin */

      t1 = -p - sqrt(di);             /* 1st root */
      RayPoint(ray,t1,hp);    /* 1st hitpoint */
      if (t1>RI_EPSILON && hp[2] < o->zmax && hp[2] > o->zmin &&
            intervall(hp[0],hp[1],o->thetamax)) {
                param=(Param *)RmGetJunk(sizeof(Param));
                copyV3(hp,param->hp);
                param->o=o;
                IsectAdd(hit,t1,prim,param); /* entering cylinder */
		hit++;
		nroots++;
	}
	RayPoint(ray,t2,hp);	/* 2nd hitpoint */
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
 * CylSetup: give shader what he needs.
 */
static void
CylSetup(hit,iof,iob)
Isect *hit;
IOF *iof;
IOB *iob;
{
  RtFloat theta,norm;
  Param *param=(Param *)hit->data;	

  norm = 1./sqrt(sqr(Vx(param->hp)) + sqr(Vy(param->hp)));        
  Vx(param->hp) *= norm;
  Vy(param->hp) *= norm;

  /* surface properties */

  if (iof->N || iof->Ng) {			/* apply a normal */
    copyV3(param->hp,iob->N);
    Vz(iob->N) = 0.;
  }
  if (iof->Ng) copyV3(iob->N,iob->Ng);          /* apply a geom. normal */

  /* parameter space mapping */

  if (iof->v) {
    iob->v=(Vz(param->hp) -  param->o->zmin) /
           (param->o->zmax - param->o->zmin);
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


static PrimProcs CylProgs = {
	CylName,
	CylInfo,		/* The methods */
	CylBound,
	CylIntersect,
	CylSetup
};

/*
 * Rm_GenCylinder: Instanciates a new cylinder with given parameters.
 */
Prim *
Rm_GenCylinder(radius,zmin,zmax,thetamax)
RtFloat radius;
RtFloat zmin,zmax;
RtFloat thetamax;
{
	Cyl *o;
	Prim *p;
	
	o = (Cyl *)RmMalloc(sizeof(Cyl));	/* Platz machen f"ur Cylind. */
	p = (Prim *)RmMalloc(sizeof(Prim));	/* und Objektstruktur */
	
	o->radius2=(o->radius=radius)*radius;   /* Werte eintragen */
	o->zmin=zmin;
	o->zmax=zmax;
	o->thetamax=DegToRad(thetamax);
	
	p->type=PRIMITIVE;			/* Und in die Objektstruktur */
	p->data=(char *)o;		        /* einbinden */
	p->procs= &CylProgs;
	return p;
}

/*\end{verbatim}*/
