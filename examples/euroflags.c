#include <ri.h>
#include "colors.h"

static RtPoint plane[4] = { {-1.,-1.,0.},
                            {-1.,1.,0.},
                            {1.,1.,0.},
                            {1.,-1.,0.},
                         };
static RtPoint vert[4] = {  {0.,0.,0.},
                            {0.,2.,0.},
                            {1.,2.,0.},
                            {1.,0.,0.},
                         };
static RtPoint hori[4] = {  {0.,0.,0.},
                            {0.,2./3.,0.},
                            {3.,2./3.,0.},
                            {3.,0.,0.},
                         };

static RtPoint from = {0,0,-2};
static RtPoint to = {0,0,2};


vertical(a,b,c)
RtColor a,b,c;
{
  RiTransformBegin();
    RiColor(c);
    RiPolygon(4,RI_P,(RtPointer)hori,RI_NULL);
    RiTranslate(0.,2./3.,0.);
    RiColor(b);
    RiPolygon(4,RI_P,(RtPointer)hori,RI_NULL);
    RiTranslate(0.,2./3.,0.);
    RiColor(a);
    RiPolygon(4,RI_P,(RtPointer)hori,RI_NULL);
  RiTransformEnd();
}

horizontal(a,b,c)
RtColor a,b,c;
{
  RiTransformBegin();
    RiColor(a);
    RiPolygon(4,RI_P,(RtPointer)vert,RI_NULL);
    RiTranslate(1.,0.,0.);
    RiColor(b);
    RiPolygon(4,RI_P,(RtPointer)vert,RI_NULL);
    RiTranslate(1.,0.,0.);
    RiColor(c);
    RiPolygon(4,RI_P,(RtPointer)vert,RI_NULL);
  RiTransformEnd();
}


main()
{

  RiBegin(RI_NULL);

    RiCropWindow(0.,1.001,.30,.65);
    RiScreenWindow(-10.,10.,-10.,10.);
    RiFormat(300,300,1.);
    RiPixelSamples(2.,2.);
    RiPixelFilter(RiGaussianFilter,2.,2.);
    RiDisplay("flags.rle",RI_FILE,RI_RGB,RI_NULL);

    RiLightSource("distantlight","from",from,"to",to,RI_NULL);

    RiTranslate(-9.5,0.,1.);

    RiWorldBegin();

     RiTransformBegin();
       RiTranslate(0.,0.,1.);
       RiScale(20.,20.,1.);
       RiColor(SkyBlue);
       RiPolygon(4,RI_P,(RtPointer)plane,RI_NULL);
     RiTransformEnd();

     RiTransformBegin();
       vertical(black,red,yellow);   /* deutschland */
       RiTranslate(4.,0.,0.);
       horizontal(black,yellow,red); /* belgien */
       RiTranslate(4.,0.,0.);
       vertical(red,white,blue);     /* niederlande */
       RiTranslate(4.,0.,0.);
       horizontal(green,white,red);  /* italien */
       RiTranslate(4.,0.,0.);
       vertical(red,white,red);      /* oestereich */
     RiTransformEnd();

     RiTranslate(0.,-3.,0.);

     RiTransformBegin();
       horizontal(green,white,red);  /* italien */
       RiTranslate(4.,0.,0.);
       vertical(red,white,red);      /* oestereich */
       RiTranslate(4.,0.,0.);
       vertical(black,red,yellow);   /* deutschland */
       RiTranslate(4.,0.,0.);
       horizontal(black,yellow,red); /* belgien */
       RiTranslate(4.,0.,0.);
       vertical(red,white,blue);     /* niederlande */
     RiTransformEnd();

    RiWorldEnd();

  RiEnd();
}
