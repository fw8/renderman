#include <ri.h>

SolidSphere(r)
RtFloat r;
{
  RiSolidBegin(RI_PRIMITIVE);
    RiSphere(r,-r,r,360.,RI_NULL);
  RiSolidEnd();
}


SolidUnitCube()
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
