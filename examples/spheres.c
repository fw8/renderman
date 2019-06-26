/* \subsection{spheres.c}
  Hier ein kleines Beispiel f"ur verschiedene Oberfl"achen auf Kugeln.
\begin{verbatim} */
#include <ri.h>
#include "MathVector.h"

RtColor red = {1.,0.,0.};
RtColor green = {0.,1.,0.};
RtColor blue  = {0.,0.,1.};
RtColor yellow = {1.,1.,0.};
RtColor pink = {1.,0.,1.};
RtColor mix1 = {0.,0.7,0.7};
RtColor mix2 = {0.4,0.1,0.2};
RtColor grey = {0.5,0.5,0.5};
RtColor white = {1.,1.,1.};

static RtPoint CamPos = {4.,10.,-20.};
static RtPoint CamDir = {0.,-9.,24.};
static RtFloat CamRoll = 0.;

static RtPoint from = {0.,20.,3.};
static RtPoint to = {0.,4.,4.};

static RtPoint vertices[4] = {  {1.,-1.,0.},
                                {-1.,-1.,0.},
                                {-1.,1.,0.},
                                {1.,1.,0.}
                             };

void
kugel(r,x,y,z)
RtFloat r,x,y,z;
{
    RiTransformBegin();
    RiTranslate(x,y,z);
    RiSphere(r,-r-.1,r+.1,360.,RI_NULL);
    RiTransformEnd();
}

main() {
  float Fil;
  RtFloat freq = 5.;
  RtFloat fov = 3.1415/6.;
  RtFloat Ka,Ks,Kd,roughness;
  RtFloat intens=40.;
  RtFloat scale;
  int auf;

   RiBegin(RI_NULL);

      printf("\nAufloesung : ");
      scanf("%d",&auf);
      RiFormat(auf,auf,1.);
      RiPixelSamples(1.,1.);
      RiPixelFilter(RiBoxFilter,2.0,2.0);


      PlaceCamera(CamPos,CamDir,CamRoll);
      FrameCamera(2.,1.,1.);

      RiLightSource("ambientlight",RI_NULL);

      RiDisplay("spheres.rle",RI_FILE,RI_RGB,RI_NULL);

      RiWorldBegin();
	RiLightSource("distantlight","from",from,"to",to,RI_NULL);

	RiColor(white);
	RiSurface("bump",RI_NULL);
        RiTransformBegin();
	  RiTranslate(0.,-2.,0.);
          RiScale(50.,1.,50.);
          RiRotate(90.,1.,0.,0.);
          RiPolygon(4,RI_P,(RtPointer)vertices,RI_NULL);
        RiTransformEnd();

	Ks = 1.0;
	RiSurface("trace","Ks",&Ks,RI_NULL);
        RiTransformBegin();
          RiTranslate(0.,0.,10.);
          RiScale(50.,50.,1.);
          RiPolygon(4,RI_P,(RtPointer)vertices,RI_NULL);
        RiTransformEnd();

	RiColor(red);
	Ks=.8;Ka=.5;Kd=1.2;scale=.3;roughness=.01;
        RiSurface("plastic","Ka",&Ka,"Kd",&Kd,"Ks",&Ks,RI_NULL); 
	kugel(1.,0.,0.,0.);
	RiColor(yellow);
	RiSurface("bump",RI_NULL);
	kugel(1.,4.,0.,0.);
	RiColor(grey);
	RiSurface("wood","Ka",&Ka,"Kd",&Kd,"Ks",&Ks,"roughness",&roughness,RI_NULL);
	kugel(1.,8.,0.,0.);
	RiColor(white);
	RiSurface("easysurface",RI_NULL);
	kugel(1.,0.,0.,4.);
	Ks = 1.0;
	RiSurface("trace","Ks",&Ks,RI_NULL);
        kugel(1.,4.,0.,4.);
	RiColor(pink);
	RiSurface("screen",RI_NULL);
        kugel(1.,8.,0.,4.);
	RiColor(mix2);
	RiSurface("matte",RI_NULL);
	kugel(1.,0.,0.,8.);
	RiColor(mix1);
	Kd=0.5;
	RiSurface("metal",RI_NULL);
        kugel(1.,4.,0.,8.);
	RiColor(blue);
	RiSurface("marble",RI_NULL);
        kugel(1.,8.,0.,8.);

      RiWorldEnd();

   RiEnd();
}
/* \end{verbatim} */
