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
    SetIfLess(pti[X],bounds[LOW][X]);
    SetIfLess(pti[Y],bounds[LOW][Y]);
    SetIfLess(pti[Z],bounds[LOW][Z]);
    SetIfGreater(pti[X],bounds[HIGH][X]);
    SetIfGreater(pti[Y],bounds[HIGH][Y]);
    SetIfGreater(pti[Z],bounds[HIGH][Z]);
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
