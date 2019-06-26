/* \subsection{geom.c}
   Ein kleiner Lader f"ur die Geometrien aus der Geom-Database.
\begin{verbatim}*/
#include <stdio.h>
#include <ri.h>

#include "colors.h"

static RtPoint CamPos = {0,1,-4.2};
static RtPoint CamDir = {0,-1,4};
static RtFloat CamRoll = 0.;

static RtPoint from = {0,1,-1};
static RtPoint to = {0,-1,1};

main(argc,argv)
int argc;
char *argv[];
{
  RtFloat Ka,Ks,Kd;
  FILE *in, *fopen();
  char fn[80];

  if (argc > 1) {
    in = fopen(argv[1],"r");
    if (in == NULL) {
      fprintf(stderr,"can't read input file %s\n",argv[1]);
    }
  }else in = stdin;

  RiBegin(RI_NULL);

  RiFormat(50,50,1.);
  RiPixelSamples(1.,1.);
  RiPixelFilter(RiBoxFilter,1.,1.);

    PlaceCamera(CamPos,CamDir,CamRoll);
    FrameCamera(2.,1.,1.);

   if (argc > 1) {
     sprintf(fn,"%s.rle",argv[1]);
     RiDisplay(fn,RI_FILE,RI_RGB,RI_NULL);
   }else RiDisplay("geom.rle",RI_FILE,RI_RGB,RI_NULL);

    RiWorldBegin();

      RiLightSource("distantlight","from",from,"to",to,RI_NULL);
      RiLightSource("ambientlight",RI_NULL);

      Ka=.2; Kd=.5; Ks=.5;
      RiSurface("bozo","Ka",&Ka,"Kd",&Kd,"Ks",&Ks,RI_NULL);
      RiColor(yellow);

      RiRotate(45.,0.,1.,0.);
      ReadGeom(in);

    RiWorldEnd();
  RiEnd();
}


ReadGeom(stream)
FILE *stream;
{
   RtInt i,j,k,npt,npoly,nedge;
   RtFloat x,y,z;
   RtPoint pt[2028];
   int     nv[2000];
   int     v[5000];

   fscanf(stream,"%d %d %d",&npt,&npoly,&nedge);
   for (i = 0; i < npt; i++) {
     fscanf(stream,"%lf %lf %lf",&x,&y,&z);
     pt[i][0] = x;
     pt[i][1] = y;
     pt[i][2] = z;
   }

   k=0;
   for (i = 0; i < npoly; i++) {
     fscanf(stream,"%d",&nv[i]);
     for (j = 0; j < nv[i]; j++) {
       fscanf(stream,"%d",&v[k]);
       v[k++]--;
     }
   }
   RiPointsPolygon(npoly,nv,v,RI_P,pt,RI_NULL);
}
/*\end{verbatim}*/
