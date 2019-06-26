/* \subsection{gshade.c}
  Hier ein kleines Beispiel f"ur die Erzeugung von Farbverl"aufen auf einem W"urfel.
\begin{verbatim} */
#include <ri.h>

RtColor red = {1.,0.,0.};
RtColor green = {0.,1.,0.};
RtColor blue  = {0.,0.,1.};
RtColor yellow = {1.,1.,0.};
RtColor pink = {1.,0.,1.};
RtColor grey = {0.5,0.5,0.5};


static RtPoint CamPos = {0.,0.,-4.};
static RtPoint CamDir = {0.,0.,4.};
static RtFloat CamRoll = 0.;

static RtPoint from = {0.,5.,-3.};
static RtPoint to = {0.,0.,0.};

main() {
   int auf;
   int sur;

   RiBegin(RI_NULL);

      printf("\nAufloesung : ");
      scanf("%d",&auf);
      RiFormat(auf,auf,1.);
      RiPixelSamples(1.,1.);
      RiPixelFilter(RiBoxFilter,1.,1.);
      printf("\noberflaeche (1 = contant sost easysurface : ");
      scanf("%d",&sur);

      PlaceCamera(CamPos,CamDir,CamRoll);
      FrameCamera(2.,1.,1.);
      RiLightSource("ambientlight",RI_NULL);
      RiDisplay("gshade.rle",RI_FILE,RI_RGB,RI_NULL);

      RiWorldBegin();

	RiLightSource("distantlight","from",from,"to",to,RI_NULL);

        if (sur == 1)
	  RiSurface("constant",RI_NULL);
        else
          RiSurface("easysurface",RI_NULL);

	RiRotate(45.0,1.0,1.0,1.0);

	RiColor(red);
/*	Axes(); */
        RiColor(green);
	UnitCube();

      RiWorldEnd();

   RiEnd();
}

Axes()
{
  static RtPoint xachse[4] = { {0.0,-.05,0.0},{0.0,.05,0.0},
                          {200.0,.05,0.0},{200.0,-.05,0.0}};
  static RtPoint yachse[4] = { {-.05,0.0,0.0},{-.05,200.0,0.0},
                          {.05,200.0,0.0},{.05,0.0,0.0}};
  static RtPoint zachse[4] = { {-.05,0.0,0.0},{-.05,0.0,200.0},
                             {.05,0.0,200.0},{.05,0.0,0.0}};

  RiPolygon(4,RI_P,(RtPointer)xachse,RI_NULL);
  RiPolygon(4,RI_P,(RtPointer)yachse,RI_NULL);
  RiPolygon(4,RI_P,(RtPointer)zachse,RI_NULL);
}

  static int nverts[6] = { 4,4,4,4,4,4};
  static int verts[6*4] = { 0,1,2,3,7,6,2,3,4,5,6,7,4,5,1,0,
                            5,1,2,6,4,0,3,7};

UnitCube()
{

  static RtPoint square[8] = { {-.5,-.5,.5},{-.5,.5,.5},
                  {.5,.5,.5},{.5,-.5,.5}, {-.5,-.5,-.5},
                  {-.5,.5,-.5},{.5,.5,-.5},{.5,-.5,-.5} };
  static RtColor col[8] = { {1.,1.,0.},{0.,1.,1.},
                 {0.,1.,0.},{1.,0.,0.},{0.,0.,1.},
                 {0.,1.,0.},{1.,0.,1.},{1.,1.,0.} };

    RiPointsPolygon(6,nverts,verts,RI_P,(RtPointer)square,
                    RI_CS,(RtPointer)col,RI_NULL);
}
/*\end{verbatim}*/
