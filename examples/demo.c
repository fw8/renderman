/* \subsection{demoprog.c}
   Ein kleines Demoprogramm zur variablen Eingabe folgender Parameter:
   Samplingrate, Filterrate, Aufl"osung, Name des Ausgabefiles,
   Art der verwendeten Filterfunktion, Surfaceshader und Farbe der Objekte.
\begin{verbatim}*/
#include <ri.h>
#include "MathVector.h"

static RtPoint CamPos = {4.,5.,-10.};
static RtPoint CamDir = {0.,-4.,14.};
static RtFloat CamRoll = 0.;

static RtPoint from = {0.,20.,3.};
static RtPoint to = {0.,4.,4.};

static RtPoint vertices[4] = {  {1.,-1.,0.},
                                {-1.,-1.,0.},
                                {-1.,1.,0.},
                                {1.,1.,0.}
                             };

static char *strshaders[] = {"bump","bumptrace","clouds","constant",
                             "easysurface","marble","matte","metal",
                             "plastic","screen","stripes","trace",
                             "wood","wood2","shiny"};

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
  float filx,fily,sampx,sampy;
  int auflx,aufly,art;
  char name[20];
  float rcol1,gcol1,bcol1,rcol2,gcol2,bcol2;
  RtColor col1,col2;
  int sur1,sur2;
  char *surface1,*surface2;

   RiBegin(RI_NULL);

      printf("\n\n\n\nDEMOPROGRAMM ZUM UMGANG MIT DER IMPLEMENTATION DES");
      printf("\n           RENDERMAN-INTERFACE");
      printf("\n\n von Hans-Juergen Braendle und Florian Wolpert.");
      printf("\n\n\nSie koennen das entstandene Bild nach Ende des Programms");
      printf("\nmit getX11 bildname ansehen!");
      printf("\n\n\nGeben Sie folgende Parameter ein:");
      printf("\n\nAufloesung in X   (empfohlen 100): ");
      scanf("%d",&auflx);
      printf("Aufloesung in Y   (empfohlen 100): ");
      scanf("%d",&aufly);
      RiFormat(auflx,aufly,1.);
      printf("Ausgabefile                      : ");
      scanf("%s",name);
      RiDisplay(name,RI_FILE,RI_RGB,RI_NULL);
      printf("Sampling in X      (empfohlen 1.): ");
      scanf("%f",&sampx);
      printf("Sampling in Y      (empfohlen 1.): ");
      scanf("%f",&sampy);
      RiPixelSamples(sampx,sampy);
      printf("Welcher Filter ( 1 = Box / 2 = Gaussian): ");
      scanf("%d",&art);
      printf("Filterbreite in X  (empfohlen 1.): ");
      scanf("%f",&filx);
      printf("Filterbreite in Y  (empfohlen 1.): ");
      scanf("%f",&fily);
      if (art == 1)
        RiPixelFilter(RiBoxFilter,filx,fily);
      else
        RiPixelFilter(RiGaussianFilter,filx,fily);
      printf("\nFolgende Shader stehen zur Auswahl:  \
\n    1  = bump \
\n    2  = bumptrace \
\n    3  = clouds \
\n    4  = constant \
\n    5  = easysurface \
\n    6  = marble \
\n    7  = matte \
\n    8  = metal \
\n    9  = plastic \
\n    10 = screen \
\n    11 = stripes \
\n    12 = trace \
\n    13 = wood \
\n    14 = wood2 \
\n    15 = shiny ");
      printf("\nOberflaeche der Ebene : ");
      scanf("%d",&sur1);
      printf("Oberflaeche der Kugel : ");
      scanf("%d",&sur2);
      surface1 = strshaders[sur1-1];
      surface2 = strshaders[sur2-1];
      printf("\nRotanteil Farbe der Ebene   (von 0.0 - 1.0): ");
      scanf("%f",&rcol1);
      printf("Gruenanteil Farbe der Ebene (von 0.0 - 1.0): ");
      scanf("%f",&gcol1);
      printf("Blauanteil Farbe der Ebene  (von 0.0 - 1.0): ");
      scanf("%f",&bcol1);
      printf("Rotanteil Farbe der Kugel   (von 0.0 - 1.0): ");
      scanf("%f",&rcol2);
      printf("Gruenanteil Farbe der Kugel (von 0.0 - 1.0): ");
      scanf("%f",&gcol2);
      printf("Blauanteil Farbe der Kugel  (von 0.0 - 1.0): ");
      scanf("%f",&bcol2);      
      col1[0]=rcol1;col1[1]=gcol1;col1[2]=bcol1;
      col2[0]=rcol2;col2[1]=gcol2;col2[2]=bcol2;

      PlaceCamera(CamPos,CamDir,CamRoll);
      FrameCamera(2.,1.,1.);

      RiLightSource("ambientlight",RI_NULL);

      RiWorldBegin();
	RiLightSource("distantlight","from",from,"to",to,RI_NULL);

	RiColor(col1);
        RiSurface(surface1,RI_NULL);
        RiTransformBegin();
          RiTranslate(0.,-2.,0.);
          RiScale(50.,1.,50.);
          RiRotate(90.,1.,0.,0.);
          RiPolygon(4,RI_P,(RtPointer)vertices,RI_NULL);
        RiTransformEnd();

        RiSurface(surface2,RI_NULL);
	RiColor(col2);
	kugel(2.0,4.,1.25,6.0);

      RiWorldEnd();

   RiEnd();
}

/*\end{verbatim}*/
