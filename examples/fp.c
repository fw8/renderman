#include <ri.h>

RtColor sky = {0.,0.,2.};
RtColor grey = {.5,.5,.5};

static RtPoint vertices[4] = {  {.5,-.5,0.},
                                {-.5,-.5,0.},
                                {-.5,.5,0.},
                                {.5,.5,0.}
                             };

static RtPoint from = {0,0,-2};
static RtPoint to = {0,0,2};

main()
{

  RiBegin(RI_NULL);

    RiFormat(100,100,1.);
    RiPixelSamples(1.,1.);
    RiPixelFilter(RiBoxFilter,1.,1.);
    RiDisplay("fractal.pat.rle",RI_FILE,RI_RGB,RI_NULL);

    RiLightSource("distantlight","from",from,"to",to,RI_NULL);

    RiTranslate(0.,0.,.5);

    RiWorldBegin();

      RiSurface("clouds",RI_NULL);
      RiColor(sky);
      RiOpacity(grey);
      RiPolygon(4,RI_P,(RtPointer)vertices,RI_NULL);

    RiWorldEnd();

  RiEnd();
}
