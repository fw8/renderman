/* \subsection{V.h} \label{V.h}
 *  Abgesicherte Speicherbelegungs Funktionen.
\begin{verbatim} */

/*  Vektor A mit Werten x,y,z belegen */
#define setV(x,y,z,A) A[0]=(x), A[1]=(y), A[2]=(z)

/* Vektor duplizieren R:=A */
#define copyV(A,R) R[0]=A[0], R[1]=A[1], R[2]=A[2]

/* Vektoraddition */
#define addVV(A,B,R) R[0]=A[0]+B[0], R[1]=A[1]+B[1], R[2]=A[2]+B[2]
#define addVf(A,b,R) R[0]=A[0]+b, R[1]=A[1]+b, R[2]=A[2]+b

/* Vektorsubtraktion */
#define subVV(A,B,R) R[0]=A[0]-B[0], R[1]=A[1]-B[1], R[2]=A[2]-B[2]
#define subVf(A,b,R) R[0]=A[0]-b, R[1]=A[1]-b, R[2]=A[2]-b

#define mulVV(A,B,R) R[0]=A[0]*B[0], R[1]=A[1]*B[1], R[2]=A[2]*B[2]
#define mulVf(A,b,R) R[0]=A[0]*b, R[1]=A[1]*b, R[2]=A[2]*b

#define divVV(A,B,R) R[0]=A[0]/B[0], R[1]=A[1]/B[1], R[2]=A[2]/B[2]
#define divVf(A,b,R) R[0]=A[0]/b, R[1]=A[1]/b, R[2]=A[2]/b
 
/* Skalarprodukt von A und B : A*B */
#define dotVV(A,B) (A[0]*B[0]+A[1]*B[1]+A[2]*B[2])

/* Kreuzprodukt von A und B : R:=AxB */
#define crossVV(A,B,R) \
	R[0]= A[1]*B[2]-B[1]*A[2],     \
	R[1]= -(A[0]*B[2]-A[2]*B[0]),  \
	R[2]= A[0]*B[1]-A[1]*B[0]

/*\end{verbatim}*/
