/* \subsection{PrimDisk.c} \label{PrimDisk.c}
  Dieses File enth"alt alle Routinen, die zur Verwaltung und Berechnung
  des Objekts Disk n"otig sind!
\begin{verbatim} */
#include "Prim.h"
#include "Math.h"

typedef struct {	/* DISK (pointed to by prim->data) */
	RtFloat height,radius;
	RtFloat thetamax;	
} Disk;

typedef struct {
	RtFloat u,v; /* Parameterspace */
} Param;

/* \end{verbatim} \label{PrimDisk.c:DiskName} \begin{verbatim}
 * DiskName: Gibt den Namen dieses Objektes zur"uck.
 *
 */
static char *
DiskName()
{
	return "disk" ;
}
/* \end{verbatim} \label{PrimDisk.c:DiskInfo} \begin{verbatim}
 * DiskInfo: Gibt Informationen "uber dieses Objektes zur"uck.
 *
 */
static void
DiskInfo(prim,stream)
Prim *prim;
FILE *stream;
{
        Disk *d;

        d = (Disk *)prim->data;
        fprintf(stream,"disk: height %g, radius %g, thetamax %g\n",
             d->height,d->radius,d->thetamax);
}

/*
 * DiskBound: give bounds of object (in object coordinates).
 */
static void
DiskBound(prim,bound)
Prim *prim;
RtBound bound;
{
  Disk *o = (Disk *)prim->data;

  /* lower edge */
  bound[0] = - o->radius;
  bound[1] = - o->radius;
  bound[2] =   o->height-RI_EPSILON;

  /* higher edge */
  bound[3] =   o->radius;
  bound[4] =   o->radius;
  bound[5] =   o->height+RI_EPSILON;
}

/* \end{verbatim} \label{PrimDisk.c:DiskIntersect} \begin{verbatim}
 * DiskIntersect: intersects ray with disk and put intersections in
 * hit list
 *              INPUT : ray =  Zeiger auf Strahl, der schneiden soll
 *                      prim = Zeiger auf das Primitiv
 *                      hit  = Zeiger auf die Hit-Struktur
 *              OUTPUT: 0 = kein Schnitt gefunden
 *                      sonst Anzahl der Schnitte (hier 1)
 */
static int
DiskIntersect(ray,prim,hit)
Ray   *ray;
Prim  *prim;
Isect *hit;
{
  Disk *d=(Disk *)prim->data;
  RtFloat l,n,m,r,theta;
  Param *param;

  if (Vz(ray->D) == 0.0) return 0;  /* kein Schnitt */

  l = (d->height - Vz(ray->P)) / Vz(ray->D);

  if (l <= RI_EPSILON) return 0;  /* kein Schnitt */

  n = Vx(ray->P) + l * Vx(ray->D);
  m = Vy(ray->P) + l * Vy(ray->D);

  r = sqrt(sqr(n) + sqr(m));

  if (r > d->radius) return 0;  /* kein Schnitt */

  if (n == 0.0) theta = 2 * PI * sgn(m);

  else {

    theta = atan(m/n);

    if (n < 0.0) theta += PI;

  }

  if (theta < 0.0) theta += 2 * PI;

  if (theta > d->thetamax) return 0;   /* kein Schnitt */

  param = (Param *)RmGetJunk(sizeof(Param));

  param->u = theta / d->thetamax; /* (2 * PI); */

  param->v = 1.0 - r / d->radius;

  IsectAdd(hit,l,prim,param);

  return 1;

}


/* \end{verbatim} \label{PrimDisk.c:DiskSetup} \begin{verbatim}
 * DiskSetup: F"ullt den I/O-Block mit allem, was der Shader braucht
 *
 *              INPUT : hit = Zeiger auf Hitstruktur
 *                      iof = Zeiger auf Input/Output Flags
 *                      iob = Zeiger auf Input/Output Block
 */
static void
DiskSetup(hit,iof,iob)
Isect *hit;
IOF *iof;
IOB *iob;
{
  Param *param=(Param *)hit->data;

  if (iof->N || iof->Ng) setV3(0.,0.,1.,iob->N);

  if (iof->Ng) copyV3(iob->N,iob->Ng);

  if (iof->u) iob->u = param->u;

  if (iof->v) iob->v = param->v;

}


static PrimProcs DiskProgs = {
	DiskName,
	DiskInfo,		/* Die Funktionen f"ur dieses Objekt */
	DiskBound,
	DiskIntersect,
	DiskSetup
};
/* \end{verbatim} \label{PrimDisk.c:Rm_GenDisk} \begin{verbatim}
 * Rm_GenDisk: Generiert aus den "ubergebenen Parametern eine neue
 *                Disk!
 *
 *              INPUT : radius = radius der Kugel
 *			zmin   = untere Kappe
			zmax   = obere Kappe
			thetamax = "Offnungswinkel
 *              OUTPUT: Zeiger auf die neue Objektstruktur
 */
Prim *
Rm_GenDisk(height,radius,thetamax)
RtFloat height;
RtFloat radius;
RtFloat thetamax;
{
	Disk *d;
	Prim *p;
	
	d = (Disk *)RmMalloc(sizeof(Disk));	/* Platz machen f"ur Kugel */
	p = (Prim *)RmMalloc(sizeof(Prim));	/* und Objektstruktur */
	
	d->height = height;
	d->radius = radius;
	d->thetamax = DegToRad(thetamax);
	
	p->type = PRIMITIVE;			/* Und in die Objektstruktur */
	p->data = (char *)d;		        /* einbinden */
	p->procs = &DiskProgs;
	return p;
}

/*\end{verbatim}*/
