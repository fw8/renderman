#include "ri.h"

RtColor red = {1.,0.,0.};
RtColor green = {0.,1.,0.};
RtColor blue = {0.,0.,1.};
RtColor white = {1.,1.,1.};

static RtPoint CamPos = {0.,5.,-5.};
static RtPoint CamDir = {0.,-5.,5.}; 
static RtFloat CamRoll = 0.;

static RtPoint vertices[4] = {	{1.,-1.,0.},
				{-1.,-1.,0.},
				{-1.,1.,0.},
				{1.,1.,0.}
                             };

static RtPoint from = {0,2,0};
static RtPoint to = {0,-2,0};

main()
{
  RtFloat freq = 5.;
  RtFloat fov = 3.1415/6.;
  RtFloat Ka,Ks,Kd,roughness;
  RtFloat intens=40.;
  RtFloat scale;

  RiBegin(RI_NULL);

  RiFormat(500,500,1.);
  RiPixelSamples(2.,2.);
  RiPixelFilter(RiBoxFilter,1.,1.);

   PlaceCamera(CamPos,CamDir,CamRoll);
   FrameCamera(2.,1.,1.);

    RiDisplay("wood_screen.rle",RI_FILE,RI_RGB,RI_NULL);

    intens = .9;
    RiLightSource("ambientlight","intensity",&intens,RI_NULL);

    RiWorldBegin();

      RiLightSource("distantlight","from",from,"to",to,RI_NULL);
      Ks=.8;Ka=.5;Kd=1.2;scale=.3;roughness=.01;
/*      RiSurface("plastic","Ka",&Ka,"Kd",&Kd,"Ks",&Ks,RI_NULL); */
      RiSurface("wood","Ka",&Ka,"Kd",&Kd,"Ks",&Ks,"roughness",&roughness,RI_NULL);
/*     RiSurface("bumptrace","scale",&scale,"Ka",&Ka,"Kd",&Kd,"Ks",&Ks,RI_NULL);*/

      RiColor(white);
      RiTransformBegin();
        RiTranslate(0.,-.2,0.);
        RiScale(1.5,1.,1.5);
        RiRotate(90.,1.,0.,0.);
        RiPolygon(4,RI_P,(RtPointer)vertices,RI_NULL);
      RiTransformEnd();

      Ks=.8;Ka=.2;Kd=.5;roughness=.01;
      RiSurface("screen","Ka",&Ka,"Kd",&Kd,"Ks",&Ks,"roughness",&roughness,RI_NULL);
      RiTranslate(0.,.9,0.);
      RiSphere(.7,-.7,.7,360.,RI_NULL); 

    RiWorldEnd();

  RiEnd();
}

