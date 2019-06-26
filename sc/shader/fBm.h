/* \subsection{fBm.h} \label{fBm.h}
Dieverse Verwirbelungs-Funktionen.
\begin{verbatim} */

#define EPSILON 0.0001
#define FBM(v,omega,lamda,octaves) \
		o=1; l=1; fBm=0;\
		for (i=0;i<octaves;i=i+1) {\
			fBm=fBm+o*noise(l*v);\
			l=l*lambda; o=o*omega;\
		}

#define VFBM(v,omega,lamda,octaves) \
		o=1; l=1; VfBm=0;\
		for (i=0;i<octaves;i=i+1) {\
			VfBm=VfBm+o*point noise(l*v);\
			l=l*lambda; o=o*omega;\
			if (o<EPSILON) break;\
		}
#define CHAOS(v,octaves)\
		o=1; l=1; chaos=0;\
		for (i=0;i<octaves;i=i+1) {\
			chaos=chaos+o*noise(l*v);\
			l=l*2; o=o*.5;\
		}
#define MARBLE(v)\
		CHAOS(v,6);\
		marble=sin(8*chaos+7*zcomp(v))+1;\
		marble=marble*.5;\
		marble=pow(marble,.77);
/*\end{verbatim}*/
