/* \subsection{MathVector.h} \label{MathVector.h}
 * Einige defines, die das Arbeiten mit Vektoren erleichtern.
\begin{verbatim}*/
#ifndef _MathVector_
#define _MathVector_
#include <math.h>
#include "RiTypes.h"

/* Definition der Datenstruktur fuer einen Strahl */
typedef struct Ray {    /* A RAY */
        RtPoint P;           /* position (origin) */
        RtPoint D;           /* direction (unitized) */
} Ray;

/* Zugriff auf Farbkomponenten */
#define Cr(C) C[0]
#define Cg(C) C[1]
#define Cb(C) C[2]

/* Zugriff auf Vektorelemente */
#define Vx(A) A[0]
#define Vy(A) A[1]
#define Vz(A) A[2]
#define X 0
#define Y 1
#define Z 2

/*  Vektor A mit Werten x,y,z belegen */
#define setV3(x,y,z,A) A[0]=(x), A[1]=(y), A[2]=(z)

/* Vektor duplizieren R:=A */
#define copyV3(A,R) R[0]=A[0], R[1]=A[1], R[2]=A[2]

/* Vektoraddition */
#define addV3(A,B,R) R[0]=A[0]+B[0], R[1]=A[1]+B[1], R[2]=A[2]+B[2]

/* Vektorsubtraktion */
#define subV3(A,B,R) R[0]=A[0]-B[0], R[1]=A[1]-B[1], R[2]=A[2]-B[2]

/* Betrag von A : |A| */
#define lenV3(A) sqrt(A[0]*A[0]+A[1]*A[1]+A[2]*A[2]) 

/* Normierter Vektor R:=A/|A| gibt |A| zurueck */
extern RtFloat RmUnitV3();

/* Vektor normieren */
#define unitV3(A,R) RmUnitV3((A),(R))
 
/* Skalarprodukt von A und B : A*B */
#define smulV3(A,B) (A[0]*B[0]+A[1]*B[1]+A[2]*B[2])

/* Kreuzprodukt von A und B : R:=AxB */
#define vmulV3(A,B,R) \
	R[0]= A[1]*B[2]-B[1]*A[2],     \
	R[1]= -(A[0]*B[2]-A[2]*B[0]),  \
	R[2]= A[0]*B[1]-A[1]*B[0]
	
/* Multiplikation mit einem Skalar */
#define mulV3(A,t,R) R[0]=(t)*A[0], R[1]=(t)*A[1], R[2]=(t)*A[2]

/* Linearkombination R:=sA+tB */
#define LCombV3(s,A,t,B,R) \
	R[0]=(s)*A[0]+(t)*B[0], \
	R[1]=(s)*A[1]+(t)*B[1], \
	R[2]=(s)*A[2]+(t)*B[2]

#define printV3(str,A,stream) \
	fprintf(stream,"%s(%5.2f,%5.2f,%5.2f)\n",str,A[0],A[1],A[2])

/* determinante von |ABC| */
#define det(A,B,C) (\
	 A[0]*B[1]*C[2]+B[0]*C[1]*A[2]+C[0]*A[1]*B[2] \
	-A[2]*B[1]*C[0]-B[2]*C[1]*A[0]-C[2]*A[1]*B[0])


#define RayPoint(ray,t,point) {\
        point[0]=(ray)->P[0]+(ray)->D[0]*t; \
        point[1]=(ray)->P[1]+(ray)->D[1]*t; \
        point[2]=(ray)->P[2]+(ray)->D[2]*t; }

#define SetRay(Point,Direction,R) {\
        copyV3(Point,R.P); \
        copyV3(Direction,R.D); }

#define CopyRay(A,B) {\
	SetRay((A).P,(A).D,(B)); }

#endif _MathVector_
/*\end{verbatim}*/
