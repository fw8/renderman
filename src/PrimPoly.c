/* \subsection{PrimPoly.c} \label{PrimPoly.c}
  Dieses File enth"alt alle Routinen, die zur Verwaltung und Berechnung
  des Objekts Polygon n"otig sind!
\begin{verbatim} */

#ifdef sun4
#include <alloca.h>
#endif
#include <varargs.h>

#include "RiTokens.h"
#include "RiError.h"
#include "Prim.h"
#include "MathVector.h"
#include "RiTypes.h"
#include "Math.h"

#define EQ(a,b) (!strcmp((a),(b)))
#define NEQ(a,b) (!EQ(a,b))

typedef struct {
        RtInt nvertics;
        RtPoint *vertics;
        RtPoint *normals;
        RtColor *colors;
        RtColor *opacities;
        RtPoint a,b,n; /* Vektor1,Vektor2,Normale */
	RtFloat d; /* Plane Constant */
        RtInt ukor,vkor; /* Welche Koordinaten werden denn benutzt ? */
        RtFloat umin,umax,vmin,vmax; /* Box for Poly */
} Poly;
/* \end{verbatim} \label{PrimPoly.c:PolyName} \begin{verbatim}
 * PolyName: Gibt den Namen dieses Objektes zur"uck.
 *
 */
static char *
PolyName()
{
	return "polygon";
}
/* \end{verbatim} \label{PrimPoly.c:PolyInfo} \begin{verbatim}
 * PolyInfo: Gibt Informationen "uber dieses Objektes zur"uck.
 *
 */
static void
PolyInfo(prim,stream)
  Prim  *prim;
  FILE  *stream;
{
	Poly	*p;
	int	i;

	p = (Poly *)prim->data;
	fprintf(stream,"polygon: %d vertics:\n",p->nvertics);
	for (i=0;i<p->nvertics;i++) {
	  if (i == p->nvertics-1) 
	    fprintf(stream,"From %g,%g,%g to %g,%g,%g\n",p->vertics[i][0],
                    p->vertics[i][1],p->vertics[i][2],p->vertics[0][0],
                    p->vertics[0][1],p->vertics[0][2]);
          else
	    fprintf(stream,"From %g,%g,%g to %g,%g,%g\n",p->vertics[i][0],
                    p->vertics[i][1],p->vertics[i][2],p->vertics[i+1][0],
                    p->vertics[i+1][1],p->vertics[i+1][2]);
	}
}

/*
 * PolyBound: give bounds of object (in object coordinates).
 */
static void
PolyBound(prim,bound)
Prim *prim;
RtBound bound;
{
  int  i;
  Poly *o = (Poly *)prim->data;
  RtFloat xmin,xmax,ymin,ymax,zmin,zmax;

  xmin=o->vertics[0][0];
  xmax=o->vertics[0][0];
  ymin=o->vertics[0][1];
  ymax=o->vertics[0][1];
  zmin=o->vertics[0][2];
  zmax=o->vertics[0][2];

  for (i=1;i<o->nvertics;i++)  {
    if (o->vertics[i][0] < xmin) xmin = o->vertics[i][0];
    if (o->vertics[i][0] > xmax) xmax = o->vertics[i][0];
    if (o->vertics[i][1] < ymin) ymin = o->vertics[i][1];
    if (o->vertics[i][1] > ymax) ymax = o->vertics[i][1];
    if (o->vertics[i][2] < zmin) zmin = o->vertics[i][2];
    if (o->vertics[i][2] > zmax) zmax = o->vertics[i][2];
  }

  if (xmin-xmax<RI_EPSILON) {
    bound[0] = xmin-RI_EPSILON;
    bound[3] = xmax+RI_EPSILON;
  }else{
    bound[0] = xmin;
    bound[3] = xmax;
  }

  if (ymin-ymax<RI_EPSILON) {
    bound[1] = ymin-RI_EPSILON;
    bound[4] = ymax+RI_EPSILON;
  }else{
    bound[1] = ymin;
    bound[4] = ymax;
  }

  if (zmin-zmax<RI_EPSILON) {
    bound[2] = zmin-RI_EPSILON;
    bound[5] = zmax+RI_EPSILON;
  }else{
    bound[2] = zmin;
    bound[5] = zmax;
  }

}


/* \end{verbatim} \label{PrimPoly.c:PolyIntersect} \begin{verbatim}
 * PolyIntersect: intersects ray with polygon prim and put intersections in
 * hit list
 *		INPUT :	ray =  Zeiger auf Strahl, der schneiden soll
 *			prim = Zeiger auf das Primitiv
 *			hit  = Zeiger auf die Hit-Struktur
 *		OUTPUT: 0 = kein Schnitt gefunden
 *			1 = Schnitt gefunden
 */
static int
PolyIntersect(ray,prim,hit)
  Ray	*ray;
  Prim	*prim;
  Isect *hit;
{
  RtPoint n,m,schnitt;
  RtFloat *tmp;
  double d,l;
  Poly *p;

  p = prim->data;
  copyV3(p->n,n);

  if ((d = smulV3(ray->D,n)) != 0.0) { /* Schnitt existiert */

    /* subV3(p->m,ray->P,m);	
    l = smulV3(m,n);
    l /= d; */

    l = (p->d - smulV3(p->n,ray->P)) / d;
    if (l > RI_EPSILON) {
      RayPoint(ray,l,schnitt);		/* Schnittpunkt festlegen */
      if ( (schnitt[p->ukor]<p->umin)||(schnitt[p->ukor]>p->umax)||
	   (schnitt[p->vkor]<p->vmin)||(schnitt[p->vkor]>p->vmax) )
	return 0;	/* Not in Box of Poly */
      if (RmHitPoly(schnitt,p))  { /* Im Polygon ? */
        tmp = (RtFloat *)RmGetJunk(sizeof(RtPoint));
        copyV3(schnitt,tmp); /* Save intersect tempor. */
	IsectAdd(hit,l,prim,tmp);
        return 1;
      } else
	return 0; 
    }
  } else 
      return 0;
  return 0; 
}

/* \end{verbatim} \label{PrimPoly.c:PolySetup} \begin{verbatim}
 * PolySetup: F"ullt den I/O-Block mit allem, was der Shader braucht
 *		
 *		INPUT :	hit = Zeiger auf Hitstruktur
 *			iof = Zeiger auf Input/Output Flags
 *			iob = Zeiger auf Input/Output Block
 */
static void
PolySetup(hit,iof,iob)
 Isect	*hit;
 IOF    *iof;
 IOB	*iob;
{
  Prim *prim;
  Poly *p;
  RtPoint schnitt;
  float	*abstand;
  float maxabstand,abssumme;
  RtPoint normale,tmp;
  RtColor color,opac;
  int	i;

  prim = hit->prim;
  p = (Poly *)prim->data;
  copyV3(((RtFloat *)hit->data),schnitt);

  /* Etwas Platz f"ur die Interpolationen */
  abstand = (float *)alloca(p->nvertics*sizeof(float));

  if (iof->Cs) {     /* Shader needs the Color */
    if (p->colors != (void *)0) { /* There are vertic-colors to interpol.*/
      maxabstand = 0.0;
      abssumme = 0.0;		/* Ermittlung der Lage im Verh"altnis zu den
				   Ecken! */
      color[0] = color[1] = color[2] = 0.0;
      for (i=0;i<p->nvertics;i++) {
	subV3(schnitt,p->vertics[i],tmp);
	if ( (abstand[i]=lenV3(tmp)) > maxabstand ) maxabstand = abstand[i];
      }
      for (i=0;i<p->nvertics;i++) {
	abssumme += abstand[i] = maxabstand / abstand[i];
	color[0] += abstand[i]*p->colors[i][0]; /* Je nach Abstand Farbanteil
						   dazu. */
	color[1] += abstand[i]*p->colors[i][1];
	color[2] += abstand[i]*p->colors[i][2];
      }
      color[0] /= abssumme; /* Jetzt noch normalisieren. */
      color[1] /= abssumme;
      color[2] /= abssumme;
      copyV3(color,iob->Cs);
    } else {

      copyV3(prim->attrib.Cs,iob->Cs);      /* Else Poly-Color */

    }
  }

  if (iof->Os) {      /* Shader needs the Opacity */
    if (p->opacities != (void *)0) {/* There are vertic-opacities to interpol.*/
      maxabstand = 0.0;
      abssumme = 0.0;
      opac[0] = opac[1] = opac[2] = 0.0;
      for (i=0;i<p->nvertics;i++) {		/* Gleiches Spiel wie mit den
						   Farben. */
        subV3(schnitt,p->vertics[i],tmp);
        if ( (abstand[i]=lenV3(tmp)) > maxabstand ) maxabstand = abstand[i];
      }
      for (i=0;i<p->nvertics;i++) {
        abssumme += abstand[i] = maxabstand / abstand[i];
        opac[0] += abstand[i]*p->opacities[i][0];
        opac[1] += abstand[i]*p->opacities[i][1];
        opac[2] += abstand[i]*p->opacities[i][2];
      }
      opac[0] /= abssumme;
      opac[1] /= abssumme;
      opac[2] /= abssumme;
      copyV3(opac,iob->Os);
    } else {
      copyV3(prim->attrib.Os,iob->Os);      /* Else Poly-Opacity */
    }
  }

  if (iof->N || iof->Ng) {	/* Shader needs the Normal */
    if (p->normals != (void *)0) { /* There are vertic-normals to interpol.*/
      maxabstand = 0.0;
      abssumme = 0.0;
      normale[0] = normale[1] = normale[2] = 0.0;
      for (i=0;i<p->nvertics;i++) {		/* Gleiches Spiel wie mit den
                                                   Farben. */
        subV3(schnitt,p->vertics[i],tmp);
        if ( (abstand[i]=lenV3(tmp)) > maxabstand ) maxabstand = abstand[i];
      }
      for (i=0;i<p->nvertics;i++) {
        abssumme += abstand[i] = maxabstand / abstand[i];
        normale[0] += abstand[i]*p->normals[i][0];
        normale[1] += abstand[i]*p->normals[i][1];
        normale[2] += abstand[i]*p->normals[i][2];
      }
      normale[0] /= abssumme;
      normale[1] /= abssumme;
      normale[2] /= abssumme;
      copyV3(normale,iob->N);
    } else {
      copyV3(p->n,iob->N);	/* Else Poly-Normal */
    }
  }
  
  if (iof->Ng) {
    copyV3(iob->N,iob->Ng);
  }

  if (iof->u)	{		/* Needs Surface Space */
    iob->u = schnitt[0];	/* x */
    iob->v = schnitt[1];	/* y */
  }

  if (iof->s)   {		/* Need Texture */
    iob->s = schnitt[0];
    iob->t = schnitt[1];
  }

}

static PrimProcs PolyProgs = {
	PolyName,
	PolyInfo,		/* Die Routinen f"ur dieses Objekt! */
	PolyBound,
	PolyIntersect,
	PolySetup
};

/* \end{verbatim} \label{PrimPoly.c:Rm_GenPolygon} \begin{verbatim}
 * Rm_GenPolygon: Generiert aus den "ubergebenen Parametern eine neue
 *		  Polygonstruktur!
 *
 *		INPUT : nvertics = Anzahl der Ecken 
 *			argp = Parameterliste.
 *		OUTPUT: Zeiger auf die neue Objektstruktur
 */
Prim *
Rm_GenPolygon(nvertics,argp)
  RtInt nvertics;
  va_list       argp;
{
  RtToken	token;
  RtPoint       *help;
  Poly		*po;
  Prim		*pr;
  int		i;
  RtFloat       xmin,xmax,ymin,ymax,zmin,zmax,dx,dy,dz;

  po = (Poly *)RmMalloc(sizeof(Poly));	/* Erst mal Platz schaffen. */
  pr = (Prim *)RmMalloc(sizeof(Prim));  /* F"ur Polygon und Objektstruktur */

  po->nvertics = nvertics;	/* Defaultwerte setzen */
  po->vertics = (void *)0;
  po->colors = (void *)0;
  po->opacities = (void *)0;
  po->normals = (void *)0;

  token = va_arg(argp,RtToken); /* Parameterliste anzapfen */

  while (token != RI_NULL) {	/* Solange noch was da ist. */

    if (EQ(token,RI_P)) {	/* Kommen da etwa Punkte ? */
      po->vertics = (RtPoint *)RmMalloc(nvertics*sizeof(RtPoint));
      help = (RtPoint *)va_arg(argp,RtPointer); /* Aus Liste einen Punkt
						   rausziehen */
      for (i=0;i<nvertics;i++)  {
        copyV3(help[i],po->vertics[i]); /* Und in Polygonstruktur "ubertragen */
      }
      subV3(po->vertics[1],po->vertics[0],po->a); /* Vektor 1 */
      subV3(po->vertics[nvertics-1],po->vertics[0],po->b); /* Vektor 2 */
      vmulV3(po->a,po->b,po->n); /* Berechnung der Normalen */
      po->d = smulV3(po->n,po->vertics[0]); /* plane Constant */

      xmin=po->vertics[0][0];
      xmax=po->vertics[0][0];
      ymin=po->vertics[0][1];
      ymax=po->vertics[0][1];
      zmin=po->vertics[0][2];
      zmax=po->vertics[0][2];

      for (i=0;i<nvertics;i++)  { /* Nun einen Box f"ur dieses Polygon
				     ermitteln, damit sp"ater leicher
				     geschnitten werden kann. */
	if (po->vertics[i][0] < xmin) 
	  xmin = po->vertics[i][0];
	if (po->vertics[i][0] > xmax) 
	  xmax = po->vertics[i][0];
        if (po->vertics[i][1] < ymin)
          ymin = po->vertics[i][1];
        if (po->vertics[i][1] > ymax)
          ymax = po->vertics[i][1];
        if (po->vertics[i][2] < zmin)
          zmin = po->vertics[i][2];
        if (po->vertics[i][2] > zmax)
          zmax = po->vertics[i][2];
      }
      dx = xmax-xmin;
      dy = ymax-ymin;	/* Sp"ater wird beim Schnitt das Polygon auf eine
			   Ebene projektziert!
			   Hier wird ermittelt auf welche! */
      dz = zmax-zmin;
      if ( (dx<dy) && (dx<dz) ){
	po->ukor = 1;
	po->vkor = 2;	/* y and z Koordinaten sind notwendig */
	po->umin = ymin;
	po->umax = ymax;
	po->vmin = zmin;
	po->vmax = zmax;
      } else
      if ( (dy<dx) && (dy<dz) ) {
	po->ukor = 0;
        po->vkor = 2;   /* x and z Koordinatesn sind notwendig */
        po->umin = xmin;
        po->umax = xmax;
        po->vmin = zmin;
        po->vmax = zmax;
      } else {
	po->ukor = 0;
        po->vkor = 1;   /* x and y Koordinaten sind notwendig */
        po->umin = xmin;
        po->umax = xmax;
        po->vmin = ymin;
        po->vmax = ymax;
      }

    } else if (EQ(token,RI_N)) { /* Kommen da Normale ? */
        po->normals = (RtPoint *)RmMalloc(nvertics*sizeof(RtPoint));
	help = (RtPoint *)va_arg(argp,RtPointer);
        for (i=0;i<nvertics;i++)  {
          copyV3(help[i],po->normals[i]); /* In Polygon eintragen */
        }
    } else if (EQ(token,RI_CS)) { /* Kommen da an Ecken gebundene Farben ? */
        po->colors = (RtColor *)RmMalloc(nvertics*sizeof(RtColor));
        help = (RtPoint *)va_arg(argp,RtPointer);
        for (i=0;i<nvertics;i++)  {
          copyV3(help[i],po->colors[i]); /* In Polygon eintragen */
        }
    } else if (EQ(token,RI_OS)) { /* Oder etwa Transparenzen / */
        po->opacities = (RtPoint *)RmMalloc(nvertics*sizeof(RtColor));
        help = (RtColor *)va_arg(argp,RtPointer);
        for (i=0;i<nvertics;i++)  {
          copyV3(help[i],po->opacities[i]); /* In Polygon eintragen */
        }
    } else {
       sprintf(RmMsg,"[RiPrimPoly.GenPoly] unknown or wrong token RI_%s",token);
        error(1,RmMsg);
    }

    token=va_arg(argp,RtToken); /* fetch next token */
  }

  pr->type = PRIMITIVE;
  pr->data = po;		/* Polygon in Objekt eintragen */
  pr->procs = &PolyProgs;	/* Und die zugeh"origen Funktionen eintragen */

  return pr;
}



/*
 * Die Quadranten sind folgenderma"sen definiert:
 *        |
 *   1    |   0
 *        |
 * -------c--------
 *        |
 *   2    |   3
 *        |
 */
#define quadrant(p, c) ((p.u<c.u) ? ((p.v<c.v) ? 2 : 1) : ((p.v<c.v) ? 3 : 0))

/* \end{verbatim} \label{PrimPoly.c:RmHitPoly} \begin{verbatim}
 * RmHitPoly: Hier also der Test, ob der ermittelte Schnittpunkt zwischen
 *	      Strahl und Polugonebene auch tats"achlich innerhalb der 
 *	      Polygonschranken liegt.
 *
 *              INPUT : point = Ermittelter Schnittpunkt Strahl und Poly-
				gonebene!
 *                      poly =  Zeiger auf Polygonstruktur.
 *              OUTPUT: 0 = nicht innerhalb des Polygons
 *			1 = innerhalb des Polygons.
 */
int
RmHitPoly(point,poly)
 RtPoint point;
 Poly *poly;
{
  	register int winding, i;
        int quad, lastquad;
        double left, right;
        Vec2d center, cur, last;


	center.u = point[poly->ukor];
	center.v = point[poly->vkor];

        winding = 0;
        last.u = poly->vertics[poly->nvertics-1][poly->ukor];
	last.v = poly->vertics[poly->nvertics-1][poly->vkor];
        lastquad = quadrant(last, center);
        for(i = 0;i < poly->nvertics; i++) {
                cur.u = poly->vertics[i][poly->ukor];
		cur.v = poly->vertics[i][poly->vkor];
                quad = quadrant(cur, center);
                if(quad != lastquad) {
                        if(((lastquad + 1) & 3) == quad)
                                winding++;
                        else if(((quad + 1) & 3) == lastquad)
                                winding--;
                        else {
                                /*
                                 * Find where edge crosses
                                 * center's X axis.
                                 */
                                right = last.u - cur.u;
                                left = last.v - cur.v;
                                left *= center.u - last.u;
                                if(left + last.v * right > right * center.v)
                                        winding += 2;
                                else
                                        winding -= 2;
                        }
                        lastquad = quad;
                }
                last = cur;

        }
        return (winding == 0 ? 0 : 1);
}
/*\end{verbatim}*/
