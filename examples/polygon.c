/* \subsection{polygon.c}
   Ein kleines Demoprogramm zur variablen Eingabe folgender Parameter:
   Samplingrate, Filterrate, Aufl"osung, Name des Ausgabefiles und
   Art der verwendeten Filterfunktion.
\begin{verbatim}*/
#include <ri.h>
#include "MathVector.h"

RtColor white = {1.,1.,1.};
RtColor grey  = {.5,.5,.5};

static RtPoint CamPos = {0.,0.,-4.};
static RtPoint CamDir = {0.,0.,4.};
static RtFloat CamRoll = 0.;

static RtPoint from = {0.,0.,-4.0};
static RtPoint to = {0.,0.,4.};

static RtPoint pol[4] = { {-.75,-.5,0.0},{-.75,.5,0.0},{0.75,.5,0.0},{0.75,-.5,0.0}};

main() {
  float Fil,samp;
  int aufl,art;
  char name[20];

   RiBegin(RI_NULL);

      printf("\nAufloesung : ");
      scanf("%d",&aufl);
      RiFormat(aufl,aufl,1.);
      printf("\nAusgabefile : ");
      scanf("%s",name);
      RiDisplay(name,RI_FILE,RI_RGB,RI_NULL);
      printf("\nSamplingrate : ");
      scanf("%f",&samp);
      RiPixelSamples(samp,samp);
      printf("\nWelcher Filter ( 1 = Box / 2 = Gaussian): ");
      scanf("%d",&art);
      printf("\nFilteringparameter : ");
      scanf("%f",&Fil);
      if (art == 1)
        RiPixelFilter(RiBoxFilter,Fil,Fil);
      else
        RiPixelFilter(RiGaussianFilter,Fil,Fil);

      PlaceCamera(CamPos,CamDir,CamRoll);
      FrameCamera(2.,1.,1.);

      RiLightSource("ambientlight",RI_NULL);

      RiWorldBegin();
	RiLightSource("distantlight","from",from,"to",to,RI_NULL);

        RiSurface("easysurface",RI_NULL);

	RiRotate(10.0,0.0,0.0,1.0); 

	RiColor(grey);
	RiPolygon(4,RI_P,(RtPointer)pol,RI_NULL);

      RiWorldEnd();

   RiEnd();
}

Axes()
{
  static RtPoint xachse[4] = { {0.0,-.05,0.0},{0.0,.05,0.0},{200.0,.05,0.0},{200.0,-.05,0.0}};
  static RtPoint yachse[4] = { {-.05,0.0,0.0},{-.05,200.0,0.0},{.05,200.0,0.0},{.05,0.0,0.0}};
  static RtPoint zachse[4] = { {-.05,0.0,0.0},{-.05,0.0,200.0},{.05,0.0,200.0},{.05,0.0,0.0}};

  RiPolygon(4,RI_P,(RtPointer)xachse,RI_NULL);
  RiPolygon(4,RI_P,(RtPointer)yachse,RI_NULL);
  RiPolygon(4,RI_P,(RtPointer)zachse,RI_NULL);
}
/*\end{verbatim}*/
