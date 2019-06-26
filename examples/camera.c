/* \subsection{camera.c} \label{camera.c}
 * Einige Routinen von uns zur besseren Kamerapositionierung
\begin{verbatim}*/
#include <ri.h>

/*
 * PlaceCamera(): establish a viewpoint, viewing direction and
 *  orientation for a scene. 
 *  This routine must be called before RiWorldBegin().
 *    position: a point giving the camera position
 *    direction: a point giving the camera direction relative
 *               to position.
 *    roll: an optional rotation of the camera about its
 *          direction axis
 */

PlaceCamera(position,direction,roll)
RtPoint position,direction;
float roll;
{
	RiIdentity();	/* Initialize the camera transformation */
	RiRotate(-roll,0.,0.,1.);
	AimZ(direction);
	RiTranslate(-position[0],-position[1],-position[2]);
}

/*
 * AimZ(): rotate the world so the directionvector points in
 *    positive z by rotating about the y axis, then x.
 *    The cosine of each rotation is given by components 
 *    of the normalized direction vector. 
 *    Before the y rotation the direction vector
 *    might be in negative z, but not afterward.
 */

#define PI 3.14159265359
#include <math.h>
#define sqr(x) ((x)*(x))

AimZ(direction)
RtPoint direction;
{
	double xzlen,yzlen,yrot,xrot;

	if(direction[0]==0. && direction[1]==0. && direction[2]==0.)
		return;
	/*
	 * The initial rotation about the y axis is given by the 
         * projection of the direction vector 
 	 * onto the x,z plane:
         * the x and z components of the direction.
	 */
	xzlen=sqrt(sqr(direction[0])+sqr(direction[2]));
	if (xzlen == 0.)
		yrot=(direction[1]<0.) ? 180. : 0.;
	else
		yrot=180.*acos(direction[2]/xzlen)/PI;

	/*
	 * The second rotation, about the x axis, is given by the
         * projection on the y,z plane of the 
	 * y-rotated direction vector: the original y
	 * component, and the rotated x,z vector from above.
	 */
	yzlen=sqrt(sqr(direction[1])+sqr(xzlen));
	xrot=180.*acos(xzlen/yzlen)/PI;
		/* yzlen should never be 0 */

	if (direction[1]>0.)
		RiRotate(xrot,1.,0.,0.);
	else
		RiRotate(-xrot,1.,0.,0.);
	/* The last rotation declared gets performed first */
	if (direction[0]>0.)
		RiRotate(-yrot,0.,1.,0.);
	else
		RiRotate(yrot,0.,1.,0.);
}

#define min(a,b)        ((a>b) ? b : a)

/* FrameCamera(): give physical parameters for a camera.
 * Parameters:
 *	focallength: the "camera's" focal length.
 *	framewidth : the width of the film frame.
 *	framehight : the height of the film frame.
 */

_FrameCamera(focallength,framewidth,frameheight)
float focallength,framewidth,frameheight;
{
  RtFloat normwidth,normheight;

  /* Focal length of 0 is taken to be an orthographic 
     projection */
  if (focallength != 0.0) {
    RiProjection(RI_PERSPECTIVE,RI_NULL);
    normwidth=(framewidth*.5)/focallength;
    normheight=(frameheight*.5)/focallength;
  }else{
    RiProjection(RI_ORTHOGRAPHIC,RI_NULL);
    normwidth=framewidth*.5;
    normheight=frameheight*.5;
  }
  RiScreenWindow(-normwidth,normwidth,-normheight,normheight);
}

FrameCamera(focallength,framewidth,frameheight)
float focallength,framewidth,frameheight;
{
  RtFloat fov;
  /* A nonzero focal length is taken to be a "normal" lens */
  if (focallength != 0.0) {
    fov = 2 * atan((min(framewidth,frameheight)*.5)
          /focallength);
    RiProjection(RI_PERSPECTIVE,RI_FOV,(RtPointer)&fov,
                 RI_NULL);
  }else{
    RiProjection(RI_ORTHOGRAPHIC,RI_NULL);
  }
  RiFrameAspectRatio((RtFloat)(framewidth/frameheight));
}
/*\end{verbatim}*/
