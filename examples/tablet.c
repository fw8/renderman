#include <ri.h>

RtColor red = {1.,0.,0.};
RtColor green = {0.,1.,0.};
RtColor blue = {0.,0.,1.};
RtColor white = {1.,1.,1.};

static RtPoint CamPos = {0.,5.,-5.};
static RtPoint CamDir = {0.,-5.,5.}; 
static RtFloat CamRoll = 0.;

static RtPoint vertices[4] = {	{.5,-.5,0.},
				{-.5,-.5,0.},
				{-.5,.5,0.},
				{.5,.5,0.}
                             };

static RtPoint from = {0,2,0};
static RtPoint to = {0,-2,0};

main()
{
  RtFloat freq = 5.;
  RtFloat fov = 3.1415/6.;
  RtFloat Ka,Ks,Kd,roughness,turbulence;
  RtFloat intens=40.;
  RtFloat scale;

  RiBegin(RI_NULL);

  RiFormat(200,200,1.);
  RiPixelSamples(1.,1.);
  RiPixelFilter(RiBoxFilter,1.,1.);
  RiDisplay("tablet.rle",RI_FILE,RI_RGB,RI_NULL);

   PlaceCamera(CamPos,CamDir,CamRoll);
   FrameCamera(2.,1.,1.);

/*    RiLightSource("ambientlight",RI_NULL); */

    RiWorldBegin();

      RiColor(white);
      RiLightSource("distantlight","from",from,"to",to,RI_NULL);
      Ks=0.;Ka=.0;Kd=1.0;scale=.01;
      RiSurface("bozo","Ka",&Ka,"Kd",&Kd,"Ks",&Ks,"roughness",&roughness,
                "turbulence",&turbulence,RI_NULL);
      RiSurface("txtplastic","Kd",&Kd,"Ks",&Ks,
                "mapname","karte.rle",RI_NULL);

      RiTransformBegin();
        RiScale(4.,1.,4.);
        RiRotate(90.,1.,0.,0.);
        RiPolygon(4,RI_P,(RtPointer)vertices,RI_NULL);
      RiTransformEnd();

      Ks=.8;Ka=.2;Kd=.5;roughness=.01;turbulence=.8;
      RiSurface("bozo","Ka",&Ka,"Kd",&Kd,"Ks",&Ks,"roughness",&roughness,
                "turbulence",&turbulence,RI_NULL);

      RiSurface("txtplastic","Ka",&Ka,"Kd",&Kd,"Ks",&Ks,
                "mapname","2iklein.rle",RI_NULL);

      RiTranslate(0.,.7,0.);
      RiColor(green);
/*      SolidWedge(1.,-.5,.2,270.);
      RiSphere(.7,-.7,.7,360.,RI_NULL); 
      RiColor(green); */
      RiSphere(.7,-.7,.7,360.,RI_NULL); 
/*
      RiCylinder(.5,-2.5,1.5,270.,RI_NULL); */

    RiWorldEnd();

  RiEnd();
}

