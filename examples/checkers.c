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


main()
{
  RtFloat freq = 10.;

  RiBegin(RI_NULL);

  RiFormat(100,100,1.);
  RiPixelSamples(2.,2.);
  RiPixelFilter(RiBoxFilter,1.,1.);

  RiTranslate(0.,0.,1.2);
  RiRotate(45.,1.,0.,0.);

    RiWorldBegin();
      RiDisplay("checker.pix",RI_FILE,RI_RGB,RI_NULL);

      RiSurface("checker","frequency",&freq,RI_NULL);
      RiColor(white);

      RiSphere(.9,-.4,.9,270.,RI_NULL);
    RiWorldEnd();

  RiEnd();
}

