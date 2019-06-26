#include <ri.h>

RtColor red = {1.,0.,0.};
RtColor green = {0.,1.,0.};
RtColor blue = {0.,0.,1.};
RtColor white = {1.,1.,1.};


main()
{
  RtFloat Ka = 2.;

  RiBegin(RI_NULL);

  RiFormat(200,200,1.);
  RiPixelSamples(1.,1.);
  RiPixelFilter(RiBoxFilter,1.,1.);

  RiTranslate(0.,0.,1.2);

    RiWorldBegin();
      RiDisplay("noise_test.pix",RI_FILE,RI_RGB,RI_NULL);

      RiSurface("easyclouds","Ka",&Ka,RI_NULL);
      RiColor(white);

      RiSphere(.9,-.5,.9,270.,RI_NULL);
    RiWorldEnd();

  RiEnd();
}

