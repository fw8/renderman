#include <stdio.h>


#include <ri.h>
#include "math.h"


SolidSphere(radius,zmin,zmax)
RtFloat radius,zmin,zmax;
{
	RiSolidBegin(RI_PRIMITIVE);
		RiSphere(radius,zmin,zmax,360.0,RI_NULL);

		if (fabs(zmax) < radius)
		  RiDisk(zmax,sqrt(radius*radius - zmax*zmax),360.0,RI_NULL);

                if (fabs(zmin) < radius)
                  RiDisk(zmin,sqrt(radius*radius - zmin*zmin),360.0,RI_NULL);
	RiSolidEnd();
}

SolidCylinder(radius,zmin,zmax)
RtFloat radius,zmin,zmax;
{
        RiSolidBegin(RI_PRIMITIVE);
		RiCylinder(radius,zmin,zmax,360.0,RI_NULL);

		RiDisk(zmax,radius,360.0,RI_NULL);
		RiDisk(zmin,radius,360.0,RI_NULL);
	RiSolidEnd();
}


SolidHemisphere(radius,zmin,zmax)
RtFloat radius,zmin,zmax;
{
	RiSolidBegin(RI_INTERSECTION);
		SolidSphere(radius,zmin,zmax);
		RiRotate(90.0,1.0,0.0,0.0);
		SolidCylinder(radius,0.0,radius);
	RiSolidEnd();
}

SolidWedge(radius,zmin,zmax,thetamax)
RtFloat radius,zmin,zmax,thetamax;
{
	printf("%f %f %f %f\n",radius,zmin,zmax,thetamax);
	if (thetamax == 180.0) {
		SolidHemisphere(radius,zmin,zmax);
	}else if (thetamax == 360.0) { printf("xxxxxxxx");
		SolidSphere(radius,zmin,zmax);
	}else if (thetamax < 180.0) {
		RiSolidBegin(RI_INTERSECTION);
			SolidHemisphere(radius,zmin,zmax);
			RiRotate(thetamax-180.0,0.0,0.0,1.0);
			SolidHemisphere(radius,zmin,zmax);
		RiSolidEnd();
	}else if (thetamax < 360.0) {
		RiSolidBegin(RI_UNION);
			SolidHemisphere(radius,zmin,zmax);
			RiRotate(thetamax,0.0,0.0,1.0);
			SolidHemisphere(radius,zmin,zmax);
		RiSolidEnd();
	}
}
