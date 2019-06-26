/* \subsection{MathTrafo.c} \label{MathTrafo.c}
 *	MathTrafo: Routinen zur Manipulation von 4x4 Matritzen.
\begin{verbatim}*/

/*
 *	Alle Transformationen erfolgen in einem rechtsh"andigen
 *	Koordinatensystem.
 *	Die Indizierung der Matritzen erfolgt nach dem Schema :
 *
 *		/ m[0][0] ... m[0][3] \
 *		|   .    .      .     |
 *		|   .      .    .     |
 *              |   .        .  .     |
 *		\ m[3][0] ... m[3][3] /
 */

#include "RiTypes.h"
#include "Misc.h"
#include "MathVector.h"
#include "Math.h"
#include "RiError.h"


/*
 * \end{verbatim} \label{MathTrafo.c:RmCpyM44} \begin{verbatim}
 *	RmCpyM44: Kopiert eine Matrix src nach dest.
 */
void
RmCpyM44(src,dest)
	RtMatrix src,dest;
{
	RmMemCpy(src,dest,sizeof(RtMatrix));
}

/*
 * \end{verbatim} \label{MathTrafo.c:RmIdentityM44} \begin{verbatim}
 *	RmIdentityM44: Erzeugen einer Einheits-Matrix.
 */ 
void
RmIdentityM44(m)
	RtMatrix m;
{
	m[0][0]=1.; m[0][1]=0.; m[0][2]=0.; m[0][3]=0.;
	m[1][0]=0.; m[1][1]=1.; m[1][2]=0.; m[1][3]=0.;
	m[2][0]=0.; m[2][1]=0.; m[2][2]=1.; m[2][3]=0.;
	m[3][0]=0.; m[3][1]=0.; m[3][2]=0.; m[3][3]=1.;
}

/*
 * \end{verbatim} \label{MathTrafo.c:RmMulM44} \begin{verbatim}
 *	RmMulM44: Multiplikation zweier Matritzen a und b. Das
 *		Ergebniss wird in dest abgelegt.
 */
void
RmMulM44(a,b,dest)
	RtMatrix a,b,dest;
{
	RtMatrix d;

	d[0][0]=a[0][0]*b[0][0]+a[0][1]*b[1][0]+a[0][2]*b[2][0]+a[0][3]*b[3][0];
	d[1][0]=a[1][0]*b[0][0]+a[1][1]*b[1][0]+a[1][2]*b[2][0]+a[1][3]*b[3][0];
	d[2][0]=a[2][0]*b[0][0]+a[2][1]*b[1][0]+a[2][2]*b[2][0]+a[2][3]*b[3][0];
	d[3][0]=a[3][0]*b[0][0]+a[3][1]*b[1][0]+a[3][2]*b[2][0]+a[3][3]*b[3][0];

	d[0][1]=a[0][0]*b[0][1]+a[0][1]*b[1][1]+a[0][2]*b[2][1]+a[0][3]*b[3][1];
	d[1][1]=a[1][0]*b[0][1]+a[1][1]*b[1][1]+a[1][2]*b[2][1]+a[1][3]*b[3][1];
	d[2][1]=a[2][0]*b[0][1]+a[2][1]*b[1][1]+a[2][2]*b[2][1]+a[2][3]*b[3][1];
	d[3][1]=a[3][0]*b[0][1]+a[3][1]*b[1][1]+a[3][2]*b[2][1]+a[3][3]*b[3][1];

	d[0][2]=a[0][0]*b[0][2]+a[0][1]*b[1][2]+a[0][2]*b[2][2]+a[0][3]*b[3][2];
	d[1][2]=a[1][0]*b[0][2]+a[1][1]*b[1][2]+a[1][2]*b[2][2]+a[1][3]*b[3][2];
	d[2][2]=a[2][0]*b[0][2]+a[2][1]*b[1][2]+a[2][2]*b[2][2]+a[2][3]*b[3][2];
	d[3][2]=a[3][0]*b[0][2]+a[3][1]*b[1][2]+a[3][2]*b[2][2]+a[3][3]*b[3][2];

	d[0][3]=a[0][0]*b[0][3]+a[0][1]*b[1][3]+a[0][2]*b[2][3]+a[0][3]*b[3][3];
	d[1][3]=a[1][0]*b[0][3]+a[1][1]*b[1][3]+a[1][2]*b[2][3]+a[1][3]*b[3][3];
	d[2][3]=a[2][0]*b[0][3]+a[2][1]*b[1][3]+a[2][2]*b[2][3]+a[2][3]*b[3][3];
	d[3][3]=a[3][0]*b[0][3]+a[3][1]*b[1][3]+a[3][2]*b[2][3]+a[3][3]*b[3][3];

	RmCpyM44(d,dest);
}

/*
 * \end{verbatim} \label{MathTrafo.c:RmTransM44} \begin{verbatim}
 *	RmTransM44: Translation einer Matrix m um den Vektor (dx,dy,dz).
 */
void
RmTransM44(m,dx,dy,dz)	
	RtMatrix m;
	RtFloat dx,dy,dz;
{
	RtMatrix trans_mat;

	RmIdentityM44(trans_mat);
	trans_mat[0][3]	= dx;
	trans_mat[1][3] = dy;
	trans_mat[2][3] = dz;

	RmMulM44(m,trans_mat,m);
}

/*
 * \end{verbatim} \label{MathTrafo.c:RmScalM44} \begin{verbatim}
 *	RmScalM44: Skalierung einer Matrix m um (sx,sy,sz).
 */
void
RmScalM44(m,sx,sy,sz)
	RtMatrix m;
        RtFloat sx,sy,sz;
{
	RtMatrix scal_mat;

	RmIdentityM44(scal_mat);
        scal_mat[0][0] = sx;
        scal_mat[1][1] = sy;
        scal_mat[2][2] = sz;

        RmMulM44(m,scal_mat,m);
}

/*
 * \end{verbatim} \label{MathTrafo.c:rotx} \begin{verbatim}
 *	rotx: Rotation einer Matrix m um die x-Achse.
 */
static void
rotx(m,phi)
	RtMatrix m;
	RtFloat  phi;
{
	RtMatrix rot_mat;

	RmIdentityM44(rot_mat);
	rot_mat[1][1] = cos(phi);
	rot_mat[1][2] = -sin(phi);
	rot_mat[2][1] = sin(phi);
	rot_mat[2][2] = cos(phi);

	RmMulM44(m,rot_mat,m);
}

/*
 * \end{verbatim} \label{MathTrafo.c:roty} \begin{verbatim}
 *	roty: Rotation einer Matrix m um die y-Achse.
 */
static void
roty(m,phi)
        RtMatrix m;
        RtFloat  phi;
{
        RtMatrix rot_mat;

        RmIdentityM44(rot_mat);
        rot_mat[0][0] = cos(phi);
        rot_mat[0][2] = sin(phi);
        rot_mat[2][0] = -sin(phi);
        rot_mat[2][2] = cos(phi);

        RmMulM44(m,rot_mat,m);
}

/*
 * \end{verbatim} \label{MathTrafo.c:rotz} \begin{verbatim}
 *	rotz: Rotation einer Matrix m um die z-Achse.
 */
static void
rotz(m,phi)
        RtMatrix m;
        RtFloat  phi;
{
        RtMatrix rot_mat;

        RmIdentityM44(rot_mat);
        rot_mat[0][0] = cos(phi);
        rot_mat[0][1] = -sin(phi);
        rot_mat[1][0] = sin(phi);
        rot_mat[1][1] = cos(phi);

        RmMulM44(m,rot_mat,m);
}

/*
 * \end{verbatim} \label{MathTrafo.c:RmRotM44} \begin{verbatim}
 *	RmRotM44: Rotation einer Matrix m um eine beliebige 
 *		Achse [x,y,z].
 */
void
RmRotM44(m,x,y,z,phi)
        RtMatrix m;
        RtFloat  x,y,z;
	RtFloat  phi;
{
	RtMatrix rot_mat;
	RtFloat  alpha,beta;

	/* phi=-phi;  $$$ damits richtig rum dreht !! */

	RmIdentityM44(rot_mat);

	if (x >= y && x >= z) {
		alpha= atan(z/x); 
		beta = atan(y/sqrt(sqr(x)+sqr(z)));
		roty(rot_mat,alpha);
		rotz(rot_mat,-beta);
		rotx(rot_mat,phi);
		rotz(rot_mat,beta);
		roty(rot_mat,-alpha);
	} else if (y >= x && y >= z) {
                alpha= atan(x/y);
                beta = atan(z/sqrt(sqr(y)+sqr(x)));
                rotz(rot_mat,alpha);
                rotx(rot_mat,-beta);
                roty(rot_mat,phi);
                rotx(rot_mat,beta);
                rotz(rot_mat,-alpha);
        } else if (z >= x && z >= y) {
                alpha= atan(y/z);
                beta = atan(x/sqrt(sqr(z)+sqr(y)));
                rotx(rot_mat,alpha);
                roty(rot_mat,-beta);
                rotz(rot_mat,phi);
                roty(rot_mat,beta);
                rotx(rot_mat,-alpha);
        } else {
		internal(1,"[MathTrafo.RmRotM44] trouble");
        }

        RmMulM44(m,rot_mat,m);
}

/*
 * \end{verbatim} \label{MathTrafo.c:RmMulM44V3} \begin{verbatim}
 *	RmMulM44V3: Multiplikation einer Matrix m mit einem Vektor v1,
 * 		Ergebniss in v2.
 */
void
RmMulM44V3(m,v1,v2)
	RtMatrix m;
	RtPoint  v1,v2;
{
	RtFloat a,b,c,d;

	a = m[0][0]*v1[0] + m[1][0]*v1[1] + m[2][0]*v1[2] + m[3][0];
	b = m[0][1]*v1[0] + m[1][1]*v1[1] + m[2][1]*v1[2] + m[3][1];
	c = m[0][2]*v1[0] + m[1][2]*v1[1] + m[2][2]*v1[2] + m[3][2];
	d = m[0][3]*v1[0] + m[1][3]*v1[1] + m[2][3]*v1[2] + m[3][3];

	if (d != 1.0) {
		v2[0] = a/d;
		v2[1] = b/d;
		v2[2] = c/d;
	}else{	/* Optimierung durch Vermeidung unn"otiger Divisionen */
		v2[0] = a;
		v2[1] = b;
		v2[2] = c;
	}
}

/* \end{verbatim} \label{MathTrafo.c:RmTransformVector} \begin{verbatim}
 *	RmTransformVector: Transformation eines Vektors (d.h. keine 
 *		Translation).
 */
void
RmTransformVector(src,m,dest)
        RtPoint src;
        RtMatrix m;
        RtPoint dest;
{
        RtFloat a,b,c,d;

        a = m[0][0]*src[0] + m[0][1]*src[1] + m[0][2]*src[2];
        b = m[1][0]*src[0] + m[1][1]*src[1] + m[1][2]*src[2];
        c = m[2][0]*src[0] + m[2][1]*src[1] + m[2][2]*src[2];
#ifdef DEBUG
        d = m[3][0]*src[0] + m[3][1]*src[1] + m[3][2]*src[2];
        if (d != 0.) {
          sprintf(RmMsg,
                  "[MathTrafo.RmTransformVector] serious problem : d = %g",d);
          internal(1,RmMsg);
        }
#endif DEBUG
	dest[0] = a;
	dest[1] = b;
	dest[2] = c;
}

/* \end{verbatim} \label{MathTrafo.c:RmTransformPoint} \begin{verbatim}
 *	RmTransformPoint: Transformation eines Punktes.
 */
void
RmTransformPoint(src,m,dest)
        RtPoint src;
        RtMatrix m;
        RtPoint dest;
{
        RtFloat a,b,c,d;

        a = m[0][0]*src[0] + m[0][1]*src[1] + m[0][2]*src[2] + m[0][3];
        b = m[1][0]*src[0] + m[1][1]*src[1] + m[1][2]*src[2] + m[1][3];
        c = m[2][0]*src[0] + m[2][1]*src[1] + m[2][2]*src[2] + m[2][3];
        d = m[3][0]*src[0] + m[3][1]*src[1] + m[3][2]*src[2] + m[3][3];

        if (d != 1.0) {
                dest[0] = a/d;
                dest[1] = b/d;
                dest[2] = c/d;
        }else{  /* Optimierung durch Vermeidung unn"otiger Divisionen */
                dest[0] = a;
                dest[1] = b;
                dest[2] = c;
        }
}

/*
 * \end{verbatim} \label{MathTrafo.c:RmTransformRay} \begin{verbatim}
 *	RmTransformRay: Transformierung eines Strahls.
 */
RmTransformRay(src,m,dest)
        Ray *src;
        RtMatrix m;
        Ray *dest;
{
	RmTransformPoint(src->P,m,dest->P);
	RmTransformVector(src->D,m,dest->D);
}

/*
 * \end{verbatim} \label{MathTrafo.c:printM44} \begin{verbatim}
 *	printM44: Ausgabe eine 4x4 Matrix (zu Testzwecken).
 */
#ifdef DEBUG
void
printM44(m)
	RtMatrix m;

{
   printf("\n /%6.2lf  %6.2lf  %6.2lf  %6.2lf \\",
               m[0][0],m[0][1],m[0][2],m[0][3]) ;
   printf("\n |%6.2lf  %6.2lf  %6.2lf  %6.2lf |",
               m[1][0],m[1][1],m[1][2],m[1][3]) ;
   printf("\n |%6.2lf  %6.2lf  %6.2lf  %6.2lf |",
               m[2][0],m[2][1],m[2][2],m[2][3]) ;
   printf("\n \\%6.2lf  %6.2lf  %6.2lf  %6.2lf /",
               m[3][0],m[3][1],m[3][2],m[3][3]) ;

}
#endif DEBUG

/*
 * \end{verbatim} \label{MathTrafo.c:RmInvertM44} \begin{verbatim}
 *	RmInvertM44: Berechnet die Inverse einer Matrix 
 *		nach Gauss-Jordan. Bei nicht invertierbaren Matritzen
 *		wird 1, sonst 0 zur"uckgegeben.
 */
int
RmInvertM44(a,b)
        RtMatrix a,b;
{
        float  tmp[4][8];
        float l;
        int i,zeile0=0,zeile1=1,zeile2=2,zeile3=3;

        tmp[0][0]=a[0][0];tmp[0][1]=a[0][1];tmp[0][2]=a[0][2];tmp[0][3]=a[0][3];
        tmp[1][0]=a[1][0];tmp[1][1]=a[1][1];tmp[1][2]=a[1][2];tmp[1][3]=a[1][3];
        tmp[2][0]=a[2][0];tmp[2][1]=a[2][1];tmp[2][2]=a[2][2];tmp[2][3]=a[2][3];
        tmp[3][0]=a[3][0];tmp[3][1]=a[3][1];tmp[3][2]=a[3][2];tmp[3][3]=a[3][3];
        tmp[0][4]=1.;tmp[0][5]=0.;tmp[0][6]=0.;tmp[0][7]=0.;
        tmp[1][4]=0.;tmp[1][5]=1.;tmp[1][6]=0.;tmp[1][7]=0.;
        tmp[2][4]=0.;tmp[2][5]=0.;tmp[2][6]=1.;tmp[2][7]=0.;
        tmp[3][4]=0.;tmp[3][5]=0.;tmp[3][6]=0.;tmp[3][7]=1.;

        /* 1.Schritt nach Gauss-Jordan */

        if (tmp[0][0] == 0) {
                if (tmp[1][0] != 0) { zeile0 = 1; zeile1 = 0;}
                else {
                        if (tmp[2][0] != 0) { zeile0 = 2; zeile2 = 0;}
                        else {
                                if (tmp[3][0] != 0) { zeile0 = 3; zeile3 = 0;}
                                else {
                                        return(1);      /* Null Determinante */
                                }
                        }
                }
        }
        l = -(tmp[zeile1][0]/tmp[zeile0][0]);
        for(i=0;i<8;i++) tmp[zeile1][i] += tmp[zeile0][i]*l;
        l = -(tmp[zeile2][0]/tmp[zeile0][0]);
        for(i=0;i<8;i++) tmp[zeile2][i] += tmp[zeile0][i]*l;
        l = -(tmp[zeile3][0]/tmp[zeile0][0]);
        for(i=0;i<8;i++) tmp[zeile3][i] += tmp[zeile0][i]*l;
        l = tmp[zeile0][0];
        for(i=0;i<8;i++) tmp[zeile0][i]/=l;


        /* 2.Schritt nach Gauss-Jordan */

        if (tmp[zeile1][1] == 0) {
                if (tmp[zeile2][1] != 0) { zeile1 = 2; zeile2 = 1;}
                else {
                        if (tmp[zeile3][1] != 0) { zeile1 = 3; zeile3 = 1;}
                        else {
                                return(1);      /* Null Determinante */
                        }
                }
        }
        l = -(tmp[zeile0][1]/tmp[zeile1][1]);
        for(i=1;i<8;i++) tmp[zeile0][i] += tmp[zeile1][i]*l;
        l = -(tmp[zeile2][1]/tmp[zeile1][1]);
        for(i=1;i<8;i++) tmp[zeile2][i] += tmp[zeile1][i]*l;
        l = -(tmp[zeile3][1]/tmp[zeile1][1]);
        for(i=1;i<8;i++) tmp[zeile3][i] += tmp[zeile1][i]*l;
        l = tmp[zeile1][1];
        for(i=1;i<8;i++) tmp[zeile1][i]/=l;


	 /* 3.Schritt nach Gauss-Jordan */

        if (tmp[zeile2][2] == 0) {
                if (tmp[zeile3][2] != 0) { zeile2 = 3; zeile3 = 2;}
                else {
                        return(1);      /* Null Determinante */
                }
        }
        l = -(tmp[zeile0][2]/tmp[zeile2][2]);
        for(i=2;i<8;i++) tmp[zeile0][i] += tmp[zeile2][i]*l;
        l = -(tmp[zeile1][2]/tmp[zeile2][2]);
        for(i=2;i<8;i++) tmp[zeile1][i] += tmp[zeile2][i]*l;
        l = -(tmp[zeile3][2]/tmp[zeile2][2]);
        for(i=2;i<8;i++) tmp[zeile3][i] += tmp[zeile2][i]*l;
        l = tmp[zeile2][2];
        for(i=2;i<8;i++) tmp[zeile2][i]/=l;


        /* 4.Schritt nach Gauss-Jordan */

        if (tmp[zeile3][3] == 0) {
                return(1);      /* Null Determinante */
        }
        l = -(tmp[zeile0][3]/tmp[zeile3][3]);
        for(i=3;i<8;i++) tmp[zeile0][i] += tmp[zeile3][i]*l;
        l = -(tmp[zeile1][3]/tmp[zeile3][3]);
        for(i=3;i<8;i++) tmp[zeile1][i] += tmp[zeile3][i]*l;
        l = -(tmp[zeile2][3]/tmp[zeile3][3]);
        for(i=3;i<8;i++) tmp[zeile2][i] += tmp[zeile3][i]*l;
        l = tmp[zeile3][3];
        for(i=3;i<8;i++) tmp[zeile3][i]/=l;

        b[0][0]=tmp[0][4];b[0][1]=tmp[0][5];b[0][2]=tmp[0][6];b[0][3]=tmp[0][7];
        b[1][0]=tmp[1][4];b[1][1]=tmp[1][5];b[1][2]=tmp[1][6];b[1][3]=tmp[1][7];
        b[2][0]=tmp[2][4];b[2][1]=tmp[2][5];b[2][2]=tmp[2][6];b[2][3]=tmp[2][7];
        b[3][0]=tmp[3][4];b[3][1]=tmp[3][5];b[3][2]=tmp[3][6];b[3][3]=tmp[3][7];

        return(0);
}
/*\end{verbatim}*/

