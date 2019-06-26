/*
  Dieses File enth"alt alle Routinen, die zur Verwaltung und Berechnung
  des Objekts Polygonnetz n"otig sind!
*/

#include <varargs.h>

#include "RiTokens.h"
#include "RiError.h"
#include "Prim.h"
#include "Math.h"
#include "Misc.h"

#define EQ(a,b) (!strcmp((a),(b)))
#define NEQ(a,b) (!EQ(a,b))

#define PT(i) p[v[(i)]]		/* a macro for easy access to vertices via */
                                /* indices. (v and p should point to the   */ 
                                /* right places !)                         */

typedef unsigned short Index;   /* index type for indices to points[]      */

typedef struct {
        RtFloat u, v;                    /* 2D point */
} V2;

#define HIGH 1
#define LOW 0

typedef RtFloat Bounds[2][3];

typedef struct list {
        Index pn;
        long int counter;
        Bounds bounds;
        struct list *next;
} List;

typedef struct {
        short xsize, ysize, zsize;      /* # of voxels along each axis */
        Bounds bounds;                  /* bounding box */
        double voxsize[3];              /* size of a voxel */
        List ****cells;       /* Voxels */
} Grid;

enum dom {XNORMAL,YNORMAL,ZNORMAL};

typedef struct {
	Index nverts;		/* number of vertices in polygon */
	Index *verts;		/* list of indices to points[] */
        RtPoint geom_norm;	/* geometric (true) normal of this polygon */
        enum dom dnorm;		/* the dominant axe */
        RtFloat d; 		/* Plane Constant */
} Poly;	

typedef struct {
	int npolys;		/* number of polygons */
	int npoints;		/* number of shared vertices */
	RtPoint *points;	/* Liste der unterschiedlichen ecken */
        RtPoint *normals;	/* Liste der an Ecken gebundenen Normalen */  
        RtColor	*colors;	/* Liste der an Ecken gebundenen Farben */
	RtColor *opacities;	/* Liste der an Ecken gebundenen Transparenz */
	Poly    *poly;          /* Liste der inneren Polygonstrukturen */
        Grid    *grid;
} PolyNet;

typedef struct {		/* Kleine Hilfsstruktur f"ur sp"ateren Schnitt*/
	RtPoint hp;
        Index   pn;
} Param;


/*
 * PolyNetName: Gibt Namen des Objekts zur"uck.
 */
static char *
PolyNetName()
{
	return "polygonnetz";
}

/*
 * PolyNetInfo: Gibt Informationen "uber dieses Objektes zur"uck.
 */
static void
PolyNetInfo(prim,stream)
  Prim  *prim;
  FILE  *stream;
{
	PolyNet	*o;
	int	i,j;
        RtFloat *p;
        Index   v;

	o = (PolyNet *)prim->data;
	fprintf(stream,"polygonnetz: %d polygons:\n",o->npolys);
	for (i=0;i<o->npolys;i++) {
	  fprintf(stream,"  poly %d: %d vertices\n",i,o->poly[i].nverts);
          for (j=0;j<o->poly[i].nverts;j++) {
            v = o->poly[i].verts[j];
            p = o->points[v];
            fprintf(stream,"    #%d\t: %d -> <%g,%g,%g>\n",
                    j,v,p[0],p[1],p[2]);
          }
	}
}

/*
 * PolyNetBounds: give bounds of object (in object coordinates).
 */
static void
PolyNetBounds(prim,bounds)
Prim *prim;
Bounds bounds;
{
  int  i;
  PolyNet *o = (PolyNet *)prim->data;
  RtFloat xmin,xmax,ymin,ymax,zmin,zmax;

  xmin=o->points[0][0];
  xmax=o->points[0][0];
  ymin=o->points[0][1];
  ymax=o->points[0][1];
  zmin=o->points[0][2];
  zmax=o->points[0][2];

  for (i=1;i<o->npoints;i++)  {
    if (o->points[i][0] < xmin) xmin = o->points[i][0];
    if (o->points[i][0] > xmax) xmax = o->points[i][0];
    if (o->points[i][1] < ymin) ymin = o->points[i][1];
    if (o->points[i][1] > ymax) ymax = o->points[i][1];
    if (o->points[i][2] < zmin) zmin = o->points[i][2];
    if (o->points[i][2] > zmax) zmax = o->points[i][2];
  }

  if (xmin-xmax<RI_EPSILON) {
    bounds[LOW][X] = xmin-RI_EPSILON;
    bounds[HIGH][X] = xmax+RI_EPSILON;
  }else{
    bounds[LOW][X] = xmin;
    bounds[HIGH][X] = xmax;
  }

  if (ymin-ymax<RI_EPSILON) {
    bounds[LOW][Y] = ymin-RI_EPSILON;
    bounds[HIGH][Y] = ymax+RI_EPSILON;
  }else{
    bounds[LOW][Y] = ymin;
    bounds[HIGH][Y] = ymax;
  }

  if (zmin-zmax<RI_EPSILON) {
    bounds[LOW][Z] = zmin-RI_EPSILON;
    bounds[HIGH][Z] = zmax+RI_EPSILON;
  }else{
    bounds[LOW][Z] = zmin;
    bounds[HIGH][Z] = zmax;
  }

}

/*
 * Quadrants are defined as:
 *        |
 *   1    |   0
 *        |
 * -------c--------
 *        |
 *   2    |   3
 *        |
 */
#define quadrant(p, c) ((p.u<c.u) ? ((p.v<c.v) ? 2 : 1) : ((p.v<c.v) ? 3 : 0))

/*
 * Project a point in 3-space to the plane whose normal is indicated by "i."
 */
#define project(r, p, i)        {switch(i) { \
                                case XNORMAL: \
                                        r.u = p[Y]; \
                                        r.v = p[Z]; \
                                        break; \
                                case YNORMAL: \
                                        r.u = p[X]; \
                                        r.v = p[Z]; \
                                        break; \
                                case ZNORMAL: \
                                        r.u = p[X]; \
                                        r.v = p[Y]; \
                                        break; \
                                } }
/*
 * Perform ray-polygon intersection test.
 */
static RtFloat
PolyIntersect(ray,poly,p)
Ray *ray;
Poly *poly;
RtPoint *p;
{
  register Index *v = poly->verts;
  register int winding, i;
  int quad, lastquad;
  double dist, left, right;
  V2 center, cur, last;


 /*
  * First, find where ray hits polygon plane, projecting
  * along the polygon's dominant normal component.
  */

  dist = smulV3(poly->geom_norm,ray->D);
  if(dist == 0.)
           /*
            * No intersection with polygon plane.
            */
           return 0.;

  dist = (poly->d - smulV3(poly->geom_norm,ray->P)) / dist;
  if(dist < RI_EPSILON)
           /*
            * The intersection point is behind the ray origin.
            */
           return 0.;

 /*
  * Compute the point of intersection, projected appropriately.
  */
  if       (poly->dnorm == XNORMAL) {
    center.u = ray->P[Y] + dist * ray->D[Y];
    center.v = ray->P[Z] + dist * ray->D[Z];
  }else if (poly->dnorm == YNORMAL) {
    center.v = ray->P[Z] + dist * ray->D[Z];
    center.u = ray->P[X] + dist * ray->D[X];
  }else{
    center.u = ray->P[X] + dist * ray->D[X];
    center.v = ray->P[Y] + dist * ray->D[Y];
  }
 /*
  * Is the point inside the polygon?
  *
  * Compute the winding number by finding the quadrant each
  * polygon point lies in with respect to the the point in
  * question, and computing a "delta" (winding number).  If we
  * end up going around in a complete circle around
  * the point (winding number is non-zero at the end), then
  * we're inside.  Otherwise, the point is outside.
  *
  * Note that we can turn this into a 2D problem by projecting
  * all the points along the axis defined by poly->index, which
  * is the "dominant" part of the polygon's normal vector.
  */
  winding = 0;
  project(last, PT(poly->nverts-1),poly->dnorm);
  lastquad = quadrant(last, center);
  for(i = 0;i < poly->nverts; i++) {
          project(cur, PT(i), poly->dnorm);
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
  return (winding == 0 ? 0. : dist);
}


/*
 * PolyNetIntersect: intersects ray with polygon prim and put intersections in
 * hit list. Return number of hit points.
 */
static int
PolyNetIntersect(ray,prim,hit)
  Ray	*ray;
  Prim	*prim;
  Isect *hit;
{
  PolyNet   *o = (PolyNet *)prim->data;
  Isect     *thit = hit;
  int        i,nhits = 0;

  for(i=0;i<o->npolys;i++) {	/* Hit all inner Polygons ! */
    RtFloat dist = PolyIntersect(ray,&o->poly[i],o->points);
    if (dist) {
      Param *param = (Param *)RmGetJunk(sizeof(Param));
      RayPoint(ray,dist,param->hp);
      param->pn = i;
      IsectAdd(hit,dist,prim,param);
      hit++; nhits ++;
    }
  }
  if (nhits > 1)
    RmSortHitList(thit,nhits); /* Schnittpunkte sortieren ! */
  return nhits; 
}

/*
 * PolyNetSetup: F"ullt den I/O-Block mit allem, was der Shader braucht
 *
 *              INPUT : hit = Zeiger auf Hitstruktur
 *                      iof = Zeiger auf Input/Output Flags
 *                      iob = Zeiger auf Input/Output Block
 */
static void
PolyNetSetup(hit,iof,iob)
 Isect	*hit;
 IOF    *iof;
 IOB	*iob;
{
  Prim *prim   = hit->prim;
  PolyNet *o   = (PolyNet *)prim->data;
  Param *param = (Param *)hit->data;
  Poly *poly;
  Index *v;
  RtPoint *p;
  float	*dist;
  int	i;

  poly = &o->poly[param->pn];
  v = poly->verts; /* setups for the */
  p = o->points;   /* PT(i) macro */

  dist = (float *)RmGetJunk(poly->nverts * sizeof(float));

  if (iof->Cs) {     /* Shader needs the Color */
    if (o->colors) { /* There are vertic-colors to interpol.*/
      RtPoint tmp;
      RtColor color;
      float maxdist = 0.0;
      float sum = 0.0;

     /* Ermittlung der Lage im Verh"altnis zu den Ecken! */

      color[0] = color[1] = color[2] = 0.0;
      for (i=0;i<poly->nverts;i++) {
	subV3(param->hp,PT(i),tmp);
	if ( (dist[i]=lenV3(tmp)) > maxdist ) 
           maxdist = dist[i];
      }
      for (i=0;i<poly->nverts;i++) {
	sum += dist[i] = maxdist / dist[i];
	/* Je nach Abstand Farbanteil dazu. */
	color[0] += dist[i]*o->colors[v[i]][0];
	color[1] += dist[i]*o->colors[v[i]][1];
	color[2] += dist[i]*o->colors[v[i]][2];
      }
      color[0] /= sum;	/* Jetzt noch normalisieren. */
      color[1] /= sum;
      color[2] /= sum;
      copyV3(color,iob->Cs);
    } else {

      copyV3(prim->attrib.Cs,iob->Cs);      /* Else PolyNet-Color */

    }
  }

  if (iof->Os) {      /* Shader needs the Opacity */
      copyV3(prim->attrib.Os,iob->Os);      /* Else PolyNet-Opacity */
  }

  if (iof->N || iof->Ng) {	/* Shader needs the Normal */
      copyV3(poly->geom_norm,iob->N);	/* Else PolyNet-Normal */
  }
  
  if (iof->Ng) {
    copyV3(iob->N,iob->Ng);
  }

  if (iof->u)	{		/* Needs Surface Space */
    iob->u = param->hp[0];	/* x */
    iob->v = param->hp[1];	/* y */
  }

  if (iof->s)   {		/* Need Texture */
    iob->s = param->hp[0];
    iob->t = param->hp[1];
  }

}

static PrimProcs PolyNetProgs = {
	PolyNetName,
	PolyNetInfo,		/* Die Routinen f"ur dieses Objekt! */
	PolyNetBounds,
	PolyNetIntersect,
	PolyNetSetup
};




/*
 * Convert from voxel number along X/Y/Z to corresponding coordinate.
 */
#define voxel2x(g,x)            ((x) * g->voxsize[0]+ g->bounds[0][0])
#define voxel2y(g,y)            ((y) * g->voxsize[1] + g->bounds[0][1])
#define voxel2z(g,z)            ((z) * g->voxsize[2] + g->bounds[0][2])
/*
 * And vice-versa.
 */
#define x2voxel(g,x)            (((x) - g->bounds[0][0]) / g->voxsize[0])
#define y2voxel(g,y)            (((y) - g->bounds[0][1]) / g->voxsize[1])
#define z2voxel(g,z)            (((z) - g->bounds[0][2]) / g->voxsize[2])
/*
 * Is the point "p" outisde of the bounding box "b"?
 */
#define OutOfBounds(p,b) (p[X] < b[0][0] || p[X] > b[1][0] ||\
                          p[Y] < b[0][1] || p[Y] > b[1][1] ||\
                          p[Z] < b[0][2] || p[Z] > b[1][2])

/*
 * Ray-bounding box intersection test.
 */
RtFloat
IntersectBounds(ray, bounds)
Ray *ray;
Bounds bounds;
{
  double t, tmin, tmax, bmin, bmax;
  double dir, pos;
  /* extern unsigned long BVTests;
REMEBER THIS !!
        BVTests++; */


        tmin = 0.;
        tmax = RI_INFINITY;

        dir = ray->D[X];
        pos = ray->P[X];

        if (dir < 0) {
                bmin = bounds[HIGH][X];
                bmax = bounds[LOW][X];
        } else {
                bmax = bounds[HIGH][X];
                bmin = bounds[LOW][X];
        }

        if (dir != 0.) {                /* check x-faces */
                t = (bmax - pos) / dir;
                if (t < 0.)
                        return 0.;
                if (t <= tmax)
                        tmax = t;
                t = (bmin - pos) / dir;
                if (t >= 0.) {
                        if (t > tmax)
                                return 0.;
                        tmin = t;
                }
        } else if (pos < bmin || pos > bmax)
                return 0.;

        dir = ray->D[Y];
        pos = ray->P[Y];

        if (dir < 0) {
                bmin = bounds[HIGH][Y];
                bmax = bounds[LOW][Y];
        } else {
                bmax = bounds[HIGH][Y];
                bmin = bounds[LOW][Y];
        }

        if (dir != 0.) {                /* check y-faces */
                t = (bmax - pos) / dir;
                if (t < 0.)
                        return 0.;
                if (t <= tmax) {
                        if (t < tmin)
                                return 0.;
                        tmax = t;
                }
                t = (bmin - pos) / dir;
                if (t >= tmin) {
                        if (t > tmax)
                                return 0.;
                        tmin = t;
                }
        } else if (pos < bmin || pos > bmax)
                return 0.;

        dir = ray->D[Z];
        pos = ray->P[Z];

        if (dir < 0) {
                bmin = bounds[HIGH][Z];
                bmax = bounds[LOW][Z];
        } else {
                bmax = bounds[HIGH][Z];
                bmin = bounds[LOW][Z];
        }

        if (dir != 0.) {                /* check z-faces */
                t = (bmax - pos) / dir;
                if (t < 0.)
                        return 0.;
                if (t <= tmax) {
                        if (t < tmin)
                                return 0.;
                        tmax = t;
                }
                t = (bmin - pos) / dir;
                if (t >= tmin) {
                        if (t > tmax)
                                return 0.;
                        tmin = t;
                }
        } else if (pos < bmin || pos > bmax)
                return 0.;

        return tmin;
}


#define SetIfLess(a, b)         (a = (a) < (b) ? (a) : (b))
#define SetIfGreater(a, b)      (a = (a) > (b) ? (a) : (b))

FindBounds(poly,p,bounds)
Poly *poly;
RtPoint *p;
Bounds bounds;
{
  Index *v = poly->verts;
  int i;

  copyV3(PT(0),bounds[LOW]);
  copyV3(bounds[LOW],bounds[HIGH]);

  for (i=1;i<poly->nverts;i++) {
    RtFloat *pti = PT(i);
    SetIfLess(bounds[LOW][X],pti[X]);
    SetIfLess(bounds[LOW][Y],pti[Y]);
    SetIfLess(bounds[LOW][Z],pti[Z]);
    SetIfGreater(bounds[HIGH][X],pti[X]);
    SetIfGreater(bounds[HIGH][Y],pti[Y]);
    SetIfGreater(bounds[HIGH][Z],pti[Z]);
  }
}


/*
 * Convert 3D point to index into grid's voxels.
 */
pos2grid(grid, pos, index)
Grid *grid;
RtFloat pos[3];
int index[3];
{
        index[X] = (int)(x2voxel(grid, pos[0]));
        index[Y] = (int)(y2voxel(grid, pos[1]));
        index[Z] = (int)(z2voxel(grid, pos[2]));

        if (index[X] == grid->xsize)
                index[X] = grid->xsize -1;
        if (index[Y] == grid->ysize)
                index[Y] = grid->ysize -1;
        if (index[Z] == grid->zsize)
                index[Z] = grid->zsize -1;

        if (index[X] < 0 || index[X] >= grid->xsize ||
            index[Y] < 0 || index[Y] >= grid->ysize ||
            index[Z] < 0 || index[Z] >= grid->zsize)
                return 0;
        return 1;
}


/*
 * Place an polygon in a grid.
 */
engrid(grid,polynet,pn)
Grid *grid;
PolyNet *polynet;
Index pn;
{
  int x, y, z, low[3], high[3];
  List *list;
  Bounds bounds;

  FindBounds(&polynet->poly[pn],polynet->points,bounds);

 /*
  * This routine should *never* be passed an unbounded object, but...
  */
  if (pos2grid(grid, bounds[LOW], low) == 0 ||
    pos2grid(grid, bounds[HIGH], high) == 0 ) {
    internal(1,"[PrimPolyNet.engrid] out of grid");
  }

 /*
  * For each voxel that intersects the object's bounding
  * box, add pointer to this object to voxel's linked list.
  */
  for (x = low[X]; x <= high[X]; x++) {
    for (y = low[Y]; y <= high[Y]; y++) {
      for (z = low[Z]; z <= high[Z]; z++) {
        list = (List *)RmMalloc(sizeof(List));
        list->pn = pn;
        RmMemCpy(bounds,list->bounds,sizeof(Bounds));
        list->counter = 0;
        list->next = grid->cells[x][y][z];
        grid->cells[x][y][z] = list;
      }
    }
  }
}


/*
 * Initialize the voxel grid.
 */
void
InitGrid(prim,xsize,ysize,zsize)
Prim *prim;
int xsize, ysize, zsize;
{
  PolyNet *polynet=(PolyNet *)prim->data;
  Grid *grid;
  int x, y, i;
  Bounds bounds;

  grid = (Grid *)RmMalloc(sizeof(Grid));

 /*
  * Find bounding box of bounded polygons.
  */
  PolyNetBounds(prim,bounds);

  grid->xsize = xsize; grid->ysize = ysize; grid->zsize = zsize;

  for (i = 0; i < 3; i++) {
    bounds[LOW][i]  -= 2. * RI_EPSILON;
    bounds[HIGH][i] += 2. * RI_EPSILON;
    grid->bounds[LOW][i] =  bounds[LOW][i];
    grid->bounds[HIGH][i] = bounds[HIGH][i];
  }
  grid->voxsize[X] = (grid->bounds[HIGH][X]-grid->bounds[LOW][X])/xsize;
  grid->voxsize[Y] = (grid->bounds[HIGH][Y]-grid->bounds[LOW][Y])/ysize;
  grid->voxsize[Z] = (grid->bounds[HIGH][Z]-grid->bounds[LOW][Z])/zsize;

 /*
  * Allocate voxels.
  */
  grid->cells = (List ****)RmMalloc(xsize * sizeof(List ***));
  for (x = 0; x < xsize; x++) {
    grid->cells[x] = (List ***)RmMalloc(ysize*sizeof(List **));
    for (y = 0; y < ysize; y++) {
      grid->cells[x][y] = (List **)RmMalloc(zsize,sizeof(List *));
      RmMemClr(grid->cells[x][y],sizeof(List *));
    }
  }

  for (i = 0; i < polynet->npolys; i++) {
    engrid(grid,polynet,i);
  }
  polynet->grid = grid;
} 


unsigned long   raynumber = 1;          /* Current "ray number". */
                                        /* (should be "grid number") */

/*
 * Intersect ray with objects in grid cell.  Note that there are a many ways
 * to speed up this routine, all of which uglify the code to a large extent.
 */
static int
CheckVoxel(list,prim,ray,raybounds,hit,counter)
List *list;
Prim *prim;
Ray *ray;
double *raybounds[2][3];
Isect *hit;
unsigned long counter;
{
  double dist, lx, hx, ly, hy, lz, hz;
  PolyNet *o = (PolyNet *)prim->data;
  int nhits = 0;

  lx = *raybounds[LOW][X];
  hx = *raybounds[HIGH][X];
  ly = *raybounds[LOW][Y];
  hy = *raybounds[HIGH][Y];
  lz = *raybounds[LOW][Z];
  hz = *raybounds[HIGH][Z];

  for (; list; list = list->next) {
   /*
    * If object's counter is greater than or equal to the
    * number associated with the current grid,
    * don't bother checking again.  In addition, if the
    * bounding box of the ray's extent in the voxel does
    * not intersect the bounding box of the object, don't bother.
    */
    if (list->counter >= counter ||
      list->bounds[LOW][X] > hx ||
      list->bounds[HIGH][X] < lx ||
      list->bounds[LOW][Y] > hy ||
      list->bounds[HIGH][Y] < ly ||
      list->bounds[LOW][Z] > hz ||
      list->bounds[HIGH][Z] < lz) continue;
    list->counter = counter;

    dist = PolyIntersect(ray,o->poly[list->pn],o->points);
    if (dist) {
      Param *param = (Param *)RmGetJunk(sizeof(Param));
      RayPoint(ray,dist,param->hp);
      param->pn = list->pn;
      IsectAdd(hit,dist,prim,param);
      hit++; nhits ++;
    }
  }
  return nhits;
}


/*
 * Intersect ray with grid, returning distance from "pos" to
 * nearest intersection with an object in the grid.  Returns 0.
 * if no intersection.
 */
static int
GridIntersect(ray,prim,grid,hit)
Ray *ray;
Prim *prim;
Grid *grid;
Isect *hit;
{
  List *list;
  double dist, mindist, offset, tMaxX, tMaxY, tMaxZ;
  double tDeltaX, tDeltaY, tDeltaZ, *raybounds[2][3];
  int stepX, stepY, stepZ, outX, outY, outZ, x, y, z;
  RtPoint curpos, nXp, nYp, nZp, np, pDeltaX, pDeltaY, pDeltaZ;
  unsigned long counter;
  int nhits,totalhits;

  mindist = RI_INFINITY;

 /*
  * The "raynumber" scheme prevents us from performing unnecessary
  * ray/object intersection tests. Since an object may span more than
  * one voxel, it is possible that such an object will be tested more
  * than once as we move from voxel-to-voxel. The idea here is to
  * increment "raynumber" once when we begin a ray/grid intersection
  * test. Whenever we check for intersection with an object in
  * the grid, we assign that object's "counter" field the current value
  * of "raynumber".  On subsequent calls to CheckVoxel, if the object's
  * "counter" is greater than or equal to the value "raynumber" had
  * when we started tracing the current grid, we know that we've already
  * checked that object.  This isn't as straight-forward as one
  * might think due to instantiation -- one may have several
  * instances of the same object in a grid.  By incrementing raynumber
  * whenever we being a new ray/grid int. test, we guarantee that
  * we will check for intersection with the objects in the grid
  * at most once.
  */

 /*
  * If outside of the bounding box, check to see if we
  * hit it.
  */
  if (OutOfBounds(ray->P,grid->bounds)) {
    offset = IntersectBounds(ray,grid->bounds);
    if (offset <= 0.) return 0; /* Ray never hit grid space.  */
    RayPoint(ray,offset,curpos);
  }else{
    offset = 0.;
    copyV3(ray->P,curpos);
  }

  counter = raynumber++;
 /*
  * Kludge:  Voxel space is mapped as [start, end),
  * and we want it to be [start, end].
  */
  x = x2voxel(grid, curpos[X]);
  if (x == grid->xsize) x = grid->xsize -1;

  y = y2voxel(grid, curpos[Y]);
  if (y == grid->ysize) y = grid->ysize -1;

  z = z2voxel(grid, curpos[Z]);
  if (z == grid->zsize) z = grid->zsize -1;

 /*
  * tMaxX is the absolute distance from the ray origin we must move
  *              to get to the next voxel in the X
  *              direction.  It is incrementally updated
  *              by DDA as we move from voxel-to-voxel.
  * tDeltaX is the relative amount along the ray we move to
  *              get to the next voxel in the X direction. Thus,
  *              when we decide to move in the X direction,
  *              we increment tMaxX by tDeltaX.
  */
  if (ray->D[X] < 0.) {
    stepX = outX = -1;
    tMaxX = (voxel2x(grid, x) - curpos[X]) / ray->D[X];
    tDeltaX = grid->voxsize[X] / - ray->D[X];
    raybounds[LOW][X] = &np[X];
    raybounds[HIGH][X] = &curpos[X];
  } else if (ray->D[X] > 0.) {
    stepX = 1;
    outX = grid->xsize;
    tMaxX = (voxel2x(grid, x+1) - curpos[X]) / ray->D[X];
    tDeltaX = grid->voxsize[X] / ray->D[X];
    raybounds[LOW][X] = &curpos[X];
    raybounds[HIGH][X] = &np[X];
  } else {
    tMaxX = RI_INFINITY;
    raybounds[LOW][X] = &curpos[X];
    raybounds[HIGH][X] = &np[X];
    tDeltaX = 0.;
  }

  if (ray->D[Y] < 0.) {
    stepY = outY = -1;
    tMaxY = (voxel2y(grid, y) - curpos[Y]) / ray->D[Y];
    tDeltaY = grid->voxsize[Y] / - ray->D[Y];
    raybounds[LOW][Y] = &np[Y];
    raybounds[HIGH][Y] = &curpos[Y];
  } else if (ray->D[Y] > 0.) {
    stepY = 1;
    outY = grid->ysize;
    tMaxY = (voxel2y(grid, y+1) - curpos[Y]) / ray->D[Y];
    tDeltaY = grid->voxsize[Y] / ray->D[Y];
    raybounds[LOW][Y] = &curpos[Y];
    raybounds[HIGH][Y] = &np[Y];
  } else {
    tMaxY = RI_INFINITY;
    raybounds[LOW][Y] = &curpos[Y];
    raybounds[HIGH][Y] = &np[Y];
    tDeltaY = 0.;
  }

  if (ray->D[Z] < 0.) {
    stepZ = outZ = -1;
    tMaxZ = (voxel2z(grid, z) - curpos[Z]) / ray->D[Z];
    tDeltaZ = grid->voxsize[Z] / - ray->D[Z];
    raybounds[LOW][Z] = &np[Z];
    raybounds[HIGH][Z] = &curpos[Z];
  } else if (ray->D[Z] > 0.) {
    stepZ = 1;
    outZ = grid->zsize;
    tMaxZ = (voxel2z(grid, z+1) - curpos[Z]) / ray->D[Z];
    tDeltaZ = grid->voxsize[Z] / ray->D[Z];
    raybounds[LOW][Z] = &curpos[Z];
    raybounds[HIGH][Z] = &np[Z];
  } else {
    tMaxZ = RI_INFINITY;
    raybounds[LOW][Z] = &curpos[Z];
    raybounds[HIGH][Z] = &np[Z];
    tDeltaZ = 0.;
  }

 /*
  * We've already moved from the ray origin along the ray to "hitpoint"
  * by "offset" units in order to reach the grid.
  */
  tMaxX += offset;
  tMaxY += offset;
  tMaxZ += offset;

  pDeltaX[X] = tDeltaX * ray->D[X];
  pDeltaX[Y] = tDeltaX * ray->D[Y];
  pDeltaX[Z] = tDeltaX * ray->D[Z];
  pDeltaY[X] = tDeltaY * ray->D[X];
  pDeltaY[Y] = tDeltaY * ray->D[Y];
  pDeltaY[Z] = tDeltaY * ray->D[Z];
  pDeltaZ[X] = tDeltaZ * ray->D[X];
  pDeltaZ[Y] = tDeltaZ * ray->D[Y];
  pDeltaZ[Z] = tDeltaZ * ray->D[Z];

  nXp[X] = ray->P[X] + tMaxX * ray->D[X];
  nXp[Y] = ray->P[Y] + tMaxX * ray->D[Y];
  nXp[Z] = ray->P[Z] + tMaxX * ray->D[Z];
  nYp[X] = ray->P[X] + tMaxY * ray->D[X];
  nYp[Y] = ray->P[Y] + tMaxY * ray->D[Y];
  nYp[Z] = ray->P[Z] + tMaxY * ray->D[Z];
  nZp[X] = ray->P[X] + tMaxZ * ray->D[X];
  nZp[Y] = ray->P[Y] + tMaxZ * ray->D[Y];
  nZp[Z] = ray->P[Z] + tMaxZ * ray->D[Z];

  totalhits = 0;

  while (1) {
    if (tMaxX < tMaxY) {
      if (tMaxX < tMaxZ) {
        copyV3(nXp,np);
        if ((list = grid->cells[x][y][z])) {
          totalhits += nhits 
          = CheckVoxel(list,prim,ray,raybounds,hit,counter);
          hit += nhits;
        }
         
        x += stepX;
        if (x == outX) break;
        tMaxX += tDeltaX;
        copyV3(nXp,curpos);
        nXp[X] += pDeltaX[X];
        nXp[Y] += pDeltaX[Y];
        nXp[Z] += pDeltaX[Z];
      }else{
        copyV3(nZp,np);
        if ((list = grid->cells[x][y][z])) {
          totalhits += nhits 
          = CheckVoxel(list,prim,ray,raybounds,hit,counter);
          hit += nhits;
        }
        z += stepZ;
        if (z == outZ) break;
        tMaxZ += tDeltaZ;
        copyV3(nZp,curpos);
        nZp[X] += pDeltaZ[X];
        nZp[Y] += pDeltaZ[Y];
        nZp[Z] += pDeltaZ[Z];
       }
    }else{
        if (tMaxY < tMaxZ) {
          copyV3(nYp,np);
          if ((list = grid->cells[x][y][z])) {
            totalhits += nhits 
            = CheckVoxel(list,prim,ray,raybounds,hit,counter);
            hit += nhits;
          }
          y += stepY;
          if (y == outY) break;
          tMaxY += tDeltaY;
          copyV3(nYp,curpos);
          nYp[X] += pDeltaY[X];
          nYp[Y] += pDeltaY[Y];
          nYp[Z] += pDeltaY[Z];
        }else{
          copyV3(nZp,np);
          if ((list = grid->cells[x][y][z])) {
            totalhits += nhits 
            = CheckVoxel(list,prim,ray,raybounds,hit,counter);
            hit += nhits;
          }
          z += stepZ;
          if (z == outZ) break;
          tMaxZ += tDeltaZ;
          copyV3(nZp,curpos);
          nZp[X] += pDeltaZ[X];
          nZp[Y] += pDeltaZ[Y];
          nZp[Z] += pDeltaZ[Z];
      }
    }
  }
  return totalhits;
}
/*
 * Rm_GenPolyNet: Generiert aus den "ubergebenen Parametern eine neue
 *                Polygonnetzstruktur!
 */
Prim *
Rm_GenPolyNet(npolys,nverts,verts,argp)
  RtInt npolys,nverts[],verts[];
  va_list       argp;
{
  PolyNet *o = (PolyNet *)RmMalloc(sizeof(PolyNet));
  Prim *prim = (Prim *)RmMalloc(sizeof(Prim));
  RtToken	token;
  RtPoint       *next;
  int		i,j,k;

  o->npolys = npolys;
  o->poly = (Poly *)RmMalloc(npolys*sizeof(Poly));

  o->npoints = -1; k = 0;
  for(i=0;i<npolys;i++) {
    o->poly[i].nverts = nverts[i];
    o->poly[i].verts = (Index *)RmMalloc(nverts[i]*sizeof(Index));
    for (j=0;j<nverts[i];j++) {
      if (verts[k] > o->npoints) o->npoints = verts[k];
      o->poly[i].verts[j] = verts[k++];
    }
  }
  o->npoints++;

  o->points = 0;	/* Defaultwerte setzen */
  o->colors = 0;
  o->opacities = 0;
  o->normals = 0;

  token = va_arg(argp,RtToken); /* Und in die Parameterliste */

  while (token != RI_NULL) {

    if (EQ(token,RI_P)) {
      if (o->points) 
        warning(1,"[PointsPolygon] more than one RI_P token; using last one");
      else 
        o->points = (RtPoint *)RmMalloc(o->npoints*sizeof(RtPoint));

      next = (RtPoint *)va_arg(argp,RtPointer);
      for (i=0;i<o->npoints;i++) copyV3(next[i],o->points[i]);

    }else if (EQ(token,RI_N)) { /* Kommen da Normale ? */
      if (o->normals)
        warning(1,"[PointsPolygon] more than one RI_N token; using last one");
      else
        o->normals = (RtPoint *)RmMalloc(o->npoints*sizeof(RtPoint));

      next = (RtPoint *)va_arg(argp,RtPointer);
      for (i=0;i<o->npoints;i++) copyV3(next[i],o->normals[i]);

    }else if (EQ(token,RI_CS)) { /* Kommen da an Ecken gebundene Farben ? */
      if (o->colors)
        warning(1,"[PointsPolygon] more than one RI_CS token; using last one");
      else
        o->colors = (RtColor *)RmMalloc(o->npoints*sizeof(RtColor));

      next = (RtPoint *)va_arg(argp,RtPointer);
      for (i=0;i<o->npoints;i++) copyV3(next[i],o->colors[i]);

    }else if (EQ(token,RI_OS)) { /* Oder etwa Transparenzen / */
      if (o->opacities)
        warning(1,"[PointsPolygon] more than one RI_OS token; using last one");
      else
        o->opacities = (RtPoint *)RmMalloc(o->npoints*sizeof(RtColor));

      next = (RtColor *)va_arg(argp,RtPointer);
      for (i=0;i<o->npoints;i++) copyV3(next[i],o->opacities[i]);

    }else {
      sprintf(RmMsg,"[RiPrimPolyNet.GenPoly] unknown or wrong token RI_%s",
              token);
      error(1,RmMsg);
    }

    token=va_arg(argp,RtToken); /* fetch next token */
  }

  if (!o->points) {
    error(1,"[PointsPolygon] no points specified");
    return 0;
  }


  /* process all polygons and do some prework! */

  for (i=0;i<o->npolys;i++) {
   /*
    * Find normal to polygon.  Check all edges before giving
    * up, just to be relatively nice about things.
    */
    Poly    *pi = &o->poly[i];
    Index   *v  = pi->verts;
    RtPoint *p  = o->points;
    RtPoint point1,point2;

    subV3(PT(1),PT(0),point1);
    for (j=1;j<pi->nverts;j++) {
      if (smulV3(point1,point1) == 0.0) {
         warning(1,"degenerate polygon");
         return 0;
      }
      subV3(PT((j+1)%pi->nverts),PT(j),point2);
      vmulV3(point1,point2,pi->geom_norm);
      if (lenV3(pi->geom_norm) != 0.) break;
      copyV3(point2,point1);
    }

    if (j >= pi->nverts) {
       /*
        * If we walked all the way through the list,
        * then we didn't find a valid normal vector -- we
        * must have a degenerate polygon of some sort.
        */
        warning(1,"degenerate polygon");
        return 0;
    }

   /*
    * Compute and store the plane constant.
    */
    pi->d = smulV3(pi->geom_norm,PT(0));

   /*
    * Find which part of the normal vector is "dominant."  This
    * is used to turn the point-in-polygon test into a 2D problem.
    */
    { RtFloat anorm_x = fabs(pi->geom_norm[0]);
      RtFloat anorm_y = fabs(pi->geom_norm[1]);
      RtFloat anorm_z = fabs(pi->geom_norm[2]);
      RtFloat indexval;

      indexval = max(anorm_y, anorm_z);
      indexval = max(anorm_x, indexval);

      if(indexval == anorm_x)
        pi->dnorm = XNORMAL;
      else if(indexval == anorm_y)
        pi->dnorm = YNORMAL;
      else
        pi->dnorm = ZNORMAL;
    }
  }

  prim->type = PRIMITIVE;
  prim->data = (char *)o;	/* Polygonnetz in Objekt eintragen */
  prim->procs = &PolyNetProgs;  /* Und die zugeh"origen Funktionen eintragen */

  InitGrid(prim,30,30,30);

  return prim;
}
