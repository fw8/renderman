/*\subsection{Screen.h} \label{Screen.h}
 * HeaderFile f"ur das Screen - Modul , Definition der ScreenGlobals
\begin{verbatim}*/

#ifndef _Screen_
#define _Screen_

#include "RiTypes.h"

typedef struct {
	RtFloat		x,y;		/* x/y-Diff. from center of pixel */
	RtColor		color;		/* Color of this sample */
} Sample;

typedef struct {
	RtFloat		xsamples;	/* Sampleratio in x-direction */
	RtFloat		ysamples;	/* Sampleratio in y-direction */
	RtFloatFunc 	filterfunction; /* Current filter function */
	RtFloat		xwidth;		/* Current xwidth of f.function */
	RtFloat         ywidth;         /* Current ywidth of f.function */
	Sample		*samplebuffer;	/* Buffer for filtering pixels */	
	RtFloat		pstep_x;	/* xLength of an pixel */
	RtFloat		pstep_y;	/* yLength of an pixel */
	RtFloat		sstep_x;	/* xLength of a sample */
	RtFloat		sstep_y;	/* yLength of a sample */
        RtInt		saminx;		/* Number of Samples per Line in */
	RtInt		saminy;	/* Number of SampleRows in samplebuffer */
        RtFloat		xoffset; /* Border of Sampling caused by filter */
	RtFloat		yoffset; /* Same in y-direction */
	RtInt		xsamoffset; /* Number of Bordersamples e.g. */
	RtInt		ysamoffset; /* Same at the y-Border */
	RtInt		anzsamples; /* Number of samples in buffer */
	RtFloat 	z_clip_near,z_clip_far;
	RtFloat		scr_left,scr_right,scr_bottom,scr_top;
	RtFloat		field_of_view;
	RtBoolean	scr_default;
	RtInt		res_x,res_y,org_x,org_y;
	RtFloat		PAR,FAR; /* pixel aspect ratio, frame aspect ratio */
	RtFloat		crop_left,crop_right,crop_top,crop_bottom;

} ScreenGlobals;

extern ScreenGlobals RmScreenGlobals;

extern void RmGenRayOrthographic();
extern void RmGenRayPerspective();
extern void RmGeneratePic();

#endif _Screen_

/*\end{verbatim} */
