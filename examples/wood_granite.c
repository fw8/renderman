#include <ri.h>

RtColor red = {1.,0.,0.};
RtColor green = {0.,1.,0.};
RtColor blue = {0.,0.,1.};
RtColor white = {1.,1.,1.};

static RtPoint CamPos = {0.,4.,-4.};
static RtPoint CamDir = {0.,-4.,4.}; 
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
  RtFloat Ka,Ks,Kd;
  RtFloat intens=40.;

  RiBegin(RI_NULL);

  RiFormat(100,100,1.);
  RiPixelSamples(1.,1.);
  RiPixelFilter(RiBoxFilter,1.,1.);

   PlaceCamera(CamPos,CamDir,CamRoll);
   FrameCamera(2.,1.,1.);

    RiLightSource("ambientlight",RI_NULL);

    RiWorldBegin();

      RiLightSource("distantlight","from",from,"to",to,RI_NULL);
      Ks=.2;Ka=.5;Kd=.9;
/*      RiSurface("plastic","Ka",&Ka,"Kd",&Kd,"Ks",&Ks,RI_NULL); */
/*      RiSurface("wood","Kd",&Kd,RI_NULL); */
/*
      RiSurface("checker","frequency",&freq,RI_NULL); 
      RiColor(white);
      RiTransformBegin();
        RiRotate(90.,1.,0.,0.);
        RiPolygon(4,RI_P,(RtPointer)vertices,RI_NULL);
      RiTransformEnd();
*/
      Ks=.2;Ka=.5;Kd=.2;
      RiSurface("granite","Kd",&Kd,RI_NULL);
      RiColor(white);
      RiTranslate(0.,.7,0.);
      RiSphere(.5,-.5,.5,360.,RI_NULL);

    RiWorldEnd();

  RiEnd();
}

