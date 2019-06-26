/* \subsection{Intersect.c} \label{Intersect.c}
 * Intersect : Durchlaufen eines CSG-Baumes und Ermittlung der Gueltigen
 * Schnittpunkte.
\begin{verbatim}*/

#include "Prim.h"
#include "RiError.h"
#include "Stat.h"

/* \end{verbatim} \label{Intersect.c:RmSortHitList} \begin{verbatim}
 * sort_hit_list: sort a hit list with bubblesort.
 */
void
RmSortHitList(hit,m)
Isect *hit;
int m;
{
  RtBoolean flag=RI_TRUE;
  int i,n=m-1;
  Isect tmp;

  while(flag) {
    flag=RI_FALSE;
    for (i=0;i<n;i++) {
      if (hit[i].t>hit[i+1].t) {
        tmp=hit[i]; 
        hit[i]=hit[i+1]; 
        hit[i+1]=tmp;
        flag=RI_TRUE;
      }
    }
    n--;
  }
}

/* \end{verbatim} \label{Intersect.c:IntersectSolid} \begin{verbatim}
 * IntersectSolid: Intersect a ray with a set of surface primitives.
 * Put a sorted list of intersections in hit and return the number of
 * intersections.
 */
static int
IntersectSolid(ray,solid,hit)
Ray *ray;
Solid *solid;
Isect *hit;
{
  int nroots = 0;
  Ray tray;

#ifdef DEBUG
  if (solid) if (solid->type != SOLID) 
   internal(1,"[Intersect.IntersectSolid] wrong object type, SOLID expected");
#endif

  while(solid) {

#ifdef DEBUG
    if (solid->prim->type != PRIMITIVE) 
      internal(1,"[Intersect.IntersectSolid] wrong object type, \
PRIMITIVE expected");
#endif

    RmTransformRay(ray,solid->prim->WtoO,&tray);
    nroots += (*solid->prim->procs->intersect)(&tray,solid->prim,&hit[nroots]);
    solid=solid->next;
  }
  RmSortHitList(hit,nroots);
  return nroots;
}


/* \end{verbatim} \label{Intersect.c:InterscetMerge} \begin{verbatim}
 * IntersectMerge: Merge two hit-lists into one.
 */
#define IN 1
#define OUT 0

static int
IntersectMerge(op,n1,l1,n2,l2,hit)
CompType op;
int      n1;
Isect   *l1;
int      n2;
Isect   *l2;
Isect   *hit;
{
  int i,new_s,fin_s,s[2];
  int fin_n=0;
  Isect *l = hit;

  s[0]=s[1]=fin_s=OUT;

  while ((n1+n2)>0) {
    if (n1 && n2) 
      if (l1[n1-1].t > l2[n2-1].t)
        i=0;
      else
        i=1;
    else if (n1) 
      i=0;
    else i=1;

    s[i] = !s[i];

    switch (op) {

      case UNION        : new_s=(s[0] || s[1]); break;
      case INTERSECTION : new_s=(s[0] && s[1]); break;
      case DIFFERENCE   : new_s=(s[0] && !s[1]); break;
      default           : internal(1,"[Intersect.IntersectMerge] unknown op.");
    }

    if (new_s != fin_s) {
      fin_s = !fin_s;
      if (i)
        *l=l2[--n2];
      else
        *l=l1[--n1];
      fin_n++;
      l++;
    }else if (i) n2--; else n1--;
  }
  RmSortHitList(hit,fin_n); /* $$$ ist immer revers, also sortieren nicht gut */
  return fin_n;
}

/*
Fast Ray-Box Intersection
by Andrew Woo
from "Graphics Gems", Academic Press, 1990
*/
#define RIGHT   0
#define LEFT    1
#define MIDDLE  2

static int
HitBoundingBox(ray,bound)
Ray *ray;
RtBound bound;
{
        int inside = 1;
        char quadrant[3];
        register int i;
        int whichPlane;
        RtFloat maxT[3];
        RtFloat candidatePlane[3];
	RtPoint coord;

        /* Find candidate planes; this loop can be avoided if
        rays cast all from the eye(assume perpsective view) */
        for (i=0; i<3; i++)
                if(ray->P[i] < bound[i]) {
                        quadrant[i] = LEFT;
                        candidatePlane[i] = bound[i];
                        inside = 0;
                }else if (ray->P[i] > bound[i+3]) {
                        quadrant[i] = RIGHT;
                        candidatePlane[i] = bound[i+3];
                        inside = 0;
                }else   {
                        quadrant[i] = MIDDLE;
                }

        /* Ray origin inside bounding box */
        if(inside)      {
                return (1);
        }
        /* Calculate T distances to candidate planes */
        for (i = 0; i < 3; i++)
                if (quadrant[i] != MIDDLE && ray->D[i] !=0.)
                        maxT[i] = (candidatePlane[i]-ray->P[i]) / ray->D[i];
                else
                        maxT[i] = -1.;

        /* Get largest of the maxT's for final choice of intersection */
        whichPlane = 0;
        for (i = 1; i < 3; i++)
                if (maxT[whichPlane] < maxT[i])
                        whichPlane = i;

        /* Check final candidate actually inside box */
        if (maxT[whichPlane] < 0.) return (0);
        for (i = 0; i < 3; i++)
                if (whichPlane != i) {
                        coord[i] = ray->P[i] + maxT[whichPlane] * ray->D[i];
                        if ((quadrant[i] == RIGHT && coord[i] < bound[i]) ||
                           (quadrant[i] == LEFT && coord[i] > bound[i+3]))
                           return (0);      /* outside box */
                        }else {
                                coord[i] = candidatePlane[i];
                        }
        return (1);                          /* ray hits box */
}



/* \end{verbatim} \label{Intersect.c:RmIntersect} \begin{verbatim}
 * Intersect: Intersect a ray with the solid, which can be either composite
 * or primitive.
 * Put a sorted list of intersections in hit and return the number of
 * intersections.
 * Note that shade expects this routine to set hit[1].medium always.
 * Recursive because of CSG.
 */

int
RmIntersect(ray,obj,hit)
Ray *ray;
Comp *obj;
Isect *hit;
{
	Ray tray;	/* temporary ray, (after world to object trafo.) */
	int nl,nr;	/* # intersections on left and right */
	Isect lhit[ISECTMAX], rhit[ISECTMAX];
			/* intersections lists on left and right */

	if (!obj) return 0;		/* no object, no intersection */

	if (obj->type==BOUNDINGBOX) {
		if (HitBoundingBox(ray,((BBox *)obj)->bound)) {
			obj=((BBox *)obj)->csg_tree;
			RmStat.BoundinRegionTests++;
			nl=RmIntersect(ray,obj,hit);
			if (nl) RmStat.BoundinRegionTestsSucceeded++;
			return(nl);
		}else 
			return 0;
        }

	if (obj->type==COMPOSITE) {	/* composite solid */
		/* recursive on left */
		nl=RmIntersect(ray,obj->left,lhit);
		if ((nl==0) && (obj->op != UNION)) {
			/* optimisation: if l is null then l&r and l-r are
			                 null, so skip r */
			return 0;
		}else{
			/* recurse on right */
			nr=RmIntersect(ray,obj->right,rhit);
			
			/* merge left and right lists */
			return IntersectMerge(obj->op,nl,lhit,nr,rhit,hit);
		}
	}else if (obj->type==SOLID) {	/* set of surface primitives */
		return IntersectSolid(ray,obj,hit);
	}else if (obj->type==PRIMITIVE) {
		RmTransformRay(ray,((Prim *)obj)->WtoO,&tray);
		return (*((Prim *)obj)->procs->intersect)(&tray,obj,hit);
	}else{
		internal(1,"[Intersect.RmIntersect] wrong object type");
	}
}
/*\end{verbatim}*/
