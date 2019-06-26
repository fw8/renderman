/* \subsection{wuerfel.c}
  Hier ein kleines Beispiel f"ur CSG-Objekte!
\begin{verbatim} */
#include <ri.h>

RtColor red = {1.,0.,0.};
RtColor green = {0.,1.,0.};
RtColor blue = {0.,0.,1.};
RtColor white = {1.,1.,1.};



Sphere(r)
RtFloat r;
{
  RiSolidBegin(RI_PRIMITIVE);
    RiSphere(r,-r-.1,r+.1,360.,RI_NULL);
  RiSolidEnd();
}


UnitCube()
{

  static RtPoint square[4] = { {-.5,-.5,-.5},{-.5,.5,-.5},{.5,.5,-.5},{.5,-.5,-.5}};

 RiSolidBegin(RI_PRIMITIVE);
  RiTransformBegin();
    RiPolygon(4,RI_P,(RtPointer)square,RI_NULL);
    RiRotate(90.,0.0,1.0,0.0);
    RiPolygon(4,RI_P,(RtPointer)square,RI_NULL);
    RiRotate(90.,0.0,1.0,0.0);
    RiPolygon(4,RI_P,(RtPointer)square,RI_NULL);
    RiRotate(90.,0.0,1.0,0.0);
    RiPolygon(4,RI_P,(RtPointer)square,RI_NULL);
  RiTransformEnd();
  RiTransformBegin();
    RiRotate(90.,1.0,0.0,0.0);
    RiPolygon(4,RI_P,(RtPointer)square,RI_NULL);
  RiTransformEnd();
  RiTransformBegin();
    RiRotate(-90.,1.0,0.0,0.0);
    RiPolygon(4,RI_P,(RtPointer)square,RI_NULL);
  RiTransformEnd();
 RiSolidEnd();
}

PutSphere(x,y,z,r)
RtFloat x,y,z,r;
{
  RiTransformBegin();
    RiTranslate(x,y,z);
    RiSphere(r,-r-.1,r+.1,360.,RI_NULL);
  RiTransformEnd();
}

RtPoint from = {0.,0.,-2.};
RtPoint to = {-1.,1.,0.};
RtFloat intens = 2.;
 
main()
{
  RtFloat freq = 5.;

  RiBegin(RI_NULL);

  RiFormat(100,100,1.);
  RiPixelSamples(1.,1.);
  RiPixelFilter(RiBoxFilter,1.,1.);

  RiTranslate(0.,0.,1.2);
  RiRotate(45.,0.,1.,0.);
  RiRotate(45.,0.,0.,1.);

  RiDisplay("dice.rle",RI_FILE,RI_RGB,RI_NULL);
  RiLightSource("distantlight","from",from,"to",to,"intensity",&intens,RI_NULL);

    RiWorldBegin();

      RiSolidBegin(RI_DIFFERENCE);
	RiSolidBegin(RI_INTERSECTION);
          RiSurface("wood",RI_NULL);
          UnitCube();
          Sphere(.7);
        RiSolidEnd();

        RiSurface("plastic",RI_NULL);
        RiColor(white);

	RiSolidBegin(RI_PRIMITIVE);
	  PutSphere(0.,0.,-.5,.1);
	  PutSphere(.25,.25,-.5,.1);
	  PutSphere(-.25,-.25,-.5,.1);
        RiSolidEnd();


	RiSolidBegin(RI_PRIMITIVE);
          PutSphere(.5,.25,-.25,.1);
          PutSphere(.5,.0,-.25,.1);
          PutSphere(.5,-.25,-.25,.1);
          PutSphere(.5,.25,.25,.1);
          PutSphere(.5,.0,.25,.1);
          PutSphere(.5,-.25,.25,.1);
        RiSolidEnd();

	RiSolidBegin(RI_PRIMITIVE);
          PutSphere(.25,-.5,.25,.1);
          PutSphere(-.25,-.5,-.25,.1);
        RiSolidEnd();

      RiSolidEnd();

    RiWorldEnd();

  RiEnd();
}

/* \end{verbatim} */
