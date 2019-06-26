/* \subsection{ShaderTools.c} \label{ShaderTools.c}
Toolbox Funktionen der {\em RenderMan Shading Language}.
\begin{verbatim} */

#include <varargs.h>

#include "MathVector.h"
#include "Math.h"
#include "Misc.h"
#include "Shader.h"
#include "RiError.h"
#include "AdminGlobals.h"

/*
 * Mathematische Funktionen.
 */


/*
 * Rm_sin: Berechnet den Sinus eines Winkels.
 *	Input: Winkel im Bogenma"s.
 *	Output: Sinus des Winkels.
 */
RtFloat 
Rm_sin(x)
RtFloat x;
{
  return sin(x);
}

/*
 * Rm_cos: Berechnet den Cosinus eines Winkels.
 *      Input: Winkel im Bogenma"s.
 *      Output: Cosinus des Winkels.
 */
RtFloat
Rm_cos(x)
RtFloat x;
{
  return cos(x);
}

/*
 * Rm_tan: Berechnet den Tangens eines WInkels.
 *	Input: Winkel im Bogenma"s.
 *	Output: Tangens des Winkels.
 */
RtFloat
Rm_tan(x)
RtFloat x;
{
  return tan(x);
}

/*
 * Rm_radians: Konvertiert von Gradma"s nach Bogenma"s.
 *	Input: Winkel im Bogenma"s.
 *	Output: Winkel im Gradma"s.
 */
RtFloat
Rm_radians(x)
RtFloat x;
{
  return DegToRad(x);
}

/*
 * Rm_degrees: Konvertiert von Bogenma"s nach Gradma"s.
 *	Input: Winkel im Gradma"s.
 *	Output: Winkel im Bogenma"s.
 */
RtFloat
Rm_degrees(x)
RtFloat x;
{
  return RadToDeg(x);
}

/*
 * Rm_sqrt: Berechnet die Quadratwurzel des Arguments.
 *	Input: Wert von dem die Wurzel zu ziehen ist.
 *	Output: Quadratwurzel des Wertes.
 */
RtFloat
Rm_sqrt(x)
RtFloat x;
{
  if (x<0.) {
    warning(1,"[Shader.sqrt] domain error");
    return 0.;
  }
  return sqrt(x);
}

/*
 * Rm_pow: Berechnet x^y von x und y.
 *	Input: x und y.
 *	Output: x^y.
 */
RtFloat
Rm_pow(x,y)
RtFloat x,y;
{
  return pow(x,y);
}

/*
 * Rm_mod: Berechnet a modulo b. (auch mit reelen Zahlen !)
 *	Input: a und b.
 *	Output: a modulo b.
 */
RtFloat
Rm_mod(a,b)
RtFloat a,b;
{
   return(a-floor(a/b)*b);
}

/*
 * Rm_abs: Berechnet den Absolutbetrag von x.
 *	Input: x.
 *	Output: |x|.
 */
RtFloat
Rm_abs(x)
RtFloat x;
{
  return fabs(x);
}

/*
 * Rm_min: Ermittelt das Minimum zweier Zahlen x und y.
 *	Input: x und y.
 *	Output: Das Minimum von x und y.
 */
RtFloat
Rm_min(x,y)
RtFloat x,y;
{
   if (x<y) return x;
   else     return y;
}

/*
 * Rm_max: Ermittelt das Maximum zweier Zahlen x und y.
 *      Input: x und y.
 *      Output: Das Maximum von x und y.
 */
RtFloat
Rm_max(x,y)
RtFloat x,y;
{
   if (x>y) return x;
   else     return y;
}

/*
 * Rm_floor: Ermittelt die kleinste ganze Zahl des reellen Arguments x.
 *	Input: x.
 *	Output: [x].
 */
RtFloat
Rm_floor(x)
RtFloat x;
{
   return floor(x);
}

/*
 * Rm_step: Stellt fest ob eine Zahl unter oder "uber eine Grenze liegt.
 *	Input: Grenze und Zahl.
 *	Output: 0 wenn die Zahl unter der Grenze liegt, sonst 1.
 */
RtFloat
Rm_step(bound,val)
RtFloat bound,val;
{
  RtFloat t;

  if (val<bound) return 0;
  else           return 1.;
}

/*
 * Rm_smoothstep: F"uhrt eine Hermitsche Interpolation zw. 0 und 1 durch.
 *	Input: Untere und Obere Grenze sowie Argument f"ur die Interpolations-
 *		funktion.
 *	Output: 0 wenn das Argument kleiner als die Untere bzw. 1 wenn es 
 *		gr"o"ser als die obere Grenze ist. Ansonst eine weiche
 *		Interpolation zw. 0 und 1.
 */
RtFloat
Rm_smoothstep(lower,upper,val)
RtFloat lower,upper,val;
{
  RtFloat t;

  if (val<lower) return 0;
  if (val>=upper) return 1.;
  t=(val-lower) / (upper-lower);
  return (-2*t*t*t+3*t*t);
}

/*
 * Shading, Coloring and Lightning Functions.
 */

/*
 * Rm_ambient: Summiert die Helligkeit aller ambienten Lichtquellen.
 *	Input: Zeiger auf einen Vektor aller aktiven Lichtquellenshader.
 *	Output: Summe aller ambienten Lichtquellen.
 */
void
Rm_ambient(lightset,sum)
LightSet *lightset;	/* Input */
RtColor sum;		/* Output */
{
  RtPoint dummy;
  RtColor CI;

  setV3(0.,0.,0.,sum); /* Initialisieren */

  /* Abarbeiten der Liste mit den Lichtquellenshader. Aufruf der
   * Objekt-Methode "illuminate" und testen des R"uckgabewerts.
   * Nur wenn der R"uckgabewert 0 ist handelt es sich um eine
   * ambiente Lichtquelle. Dann wird der erhaltene Beleuchtungswert 'CI'
   * in die Summe aufgenommen. */

  while(lightset) {
   if (!(*lightset->light->procs->illuminate)(lightset->light,dummy,dummy,CI)){
     sum[0] += CI[0];
     sum[1] += CI[1];
     sum[2] += CI[2];
   }
   lightset=lightset->next;
  }
}

/*
 * Rm_diffuse: Berechnet den diffusen Reflektionsanteil f"ur eine gegebene
 *		Oberfl"achennormale.
 *	Input: Zeiger auf einen Vektor aller aktiven Lichtquellenshader.
 *		Punkt auf der Oberfl"ache, f"ur den die Funktion aufgrufen
 *		wird. Normale in dem Punkt.
 *	Output: Der diffuse Reflektionsanteil.
 */
void
Rm_diffuse(lightset,P,norm,C)
LightSet *lightset;		/* Input */
RtPoint P;			/* Input */
RtPoint norm;			/* Input */
RtColor C;			/* Output */
{
  RtPoint unitnorm;
  RtPoint L;
  RtColor CI;

/* Definition der Funktion wie sie in Shading Language beschrieben wird:
 *
 * color C = 0;
 * point unitnorm = normalize(norm);
 * illuminance(P,unitnorm,PI/2)
 *   C += CI * normalize(L) . unitnorm;
 * return C;
 */


  setV3(0.,0.,0.,C);	 /* Initialisieren */
  unitV3(norm,unitnorm); /* Normieren der Normalen */

  /* In einer Schleife werden alle Lichtquellen abgearbeitet und
   * der jeweils ermittelte Lichteinfall durch den Cosinus des
   * Winkels zw. Lichtquelle und Normale gewichtet. Der gewichtete
   * Wert wird dann aufsummiert. */

  while(lightset) {
   if ((*lightset->light->procs->illuminate)(lightset->light,P,L,CI)){ 
      if (smulV3(unitnorm,L) > 0.) {
        RtPoint Lnorm;
        RtFloat t;

        unitV3(L,Lnorm);
        t=smulV3(Lnorm,unitnorm);
        C[0]+=CI[0]*t;
        C[1]+=CI[1]*t;
        C[2]+=CI[2]*t;
      }
   }
   lightset=lightset->next;
  }
}

/*
 * Rm_specular: Es wird der Anteil der spiegelenden Reflektion berechnet.
 *      Input: Zeiger auf einen Vektor aller aktiven Lichtquellenshader.
 *              Punkt auf der Oberfl"ache, f"ur den die Funktion aufgrufen
 *              wird. Normale in dem Punkt. Richtung des Betrachters. 
 *		Rauhigkeit der Oberfl"ache.
 *      Output: Der spiegelende Reflektionsanteil.
 */
void
Rm_specular(lightset,P,norm,eye,roughness,C)
LightSet *lightset;	/* Input */
RtPoint P;		/* Input */
RtPoint norm,eye;	/* Input */
RtFloat roughness;	/* Input */
RtColor C;		/* Output */
{
  RtPoint unitnorm,uniteye;
  RtPoint L;
  RtColor CI;

  setV3(0.,0.,0.,C);		/* Initialisierung */
  unitV3(norm,unitnorm);	/* Normieren der Normalen */
  unitV3(eye,uniteye);		/* Normieren des Vektors zum Betrachter */

  /* In einer Schleife werden alle Lichtquellen abgearbeitet und
   * der jeweils ermittelte Lichteinfall, durch t gwichtet, aufsummiert.
   * t wird nach folgender Formel ermittelt:
   *	t = (norm.H / |norm| * |H|)^(1/roughness)
   * Hierbei steht '.' f"ur das Skalarprodukt. H ist der Vektor der in der
   * Mitte zw. der Richtung der Lichtquelle und des Betrachters liegt. */
 
  while(lightset) {
   if ((*lightset->light->procs->illuminate)(lightset->light,P,L,CI)){
      if (smulV3(unitnorm,L) > 0.) {
        RtPoint H,Lnorm;
        RtFloat t;

        unitV3(L,Lnorm);
        addV3(Lnorm,uniteye,H);
        unitV3(H,H);
        t=pow(smulV3(unitnorm,H),1./roughness);
        C[0]+=CI[0]*t;
        C[1]+=CI[1]*t;
        C[2]+=CI[2]*t;
      }
   }
   lightset=lightset->next;
 }
}

/*
 * Rm_trace: Ermittelt den Lichteinfall aus einer bestimmten Richtung.
 *	Input: Aktuelle Rekusionstiefe. Punkt von dem aus der Strahl
 *		zur"uck verfolgt werden soll. Richtung in der der Lichteinfall
 *		ermittelt werden soll.
 *	Output: Lichteinfall.
 */
RtVoid
Rm_trace(level,P,D,C)
int level;
RtPoint P,D;
RtColor C;
{
  Ray new_ray;

  /* Solange die Maximale Rekusionstiefe nicht "uberschritten wird, wird
   * der RayTracer Rekursiv aufgerufen und das Ergebniss zur"uckgeliefert.
   * Ansonst wird schwarz zur"uckgeliefert. */

  if (level+1<RmGlobals.MaxLevel) {  /* we're not to deep then recurse */
    SetRay(P,D,new_ray);
    RmTrace(level+1,&new_ray,C);
  }else{
    setV3(0.,0.,0.,C);
  }
}


/*
 * Geometrische Funktionen.
 */

/*
 * Rm_xcomp: Liefert die x Komponente des Punktes P.
 *	Input: P.
 *	Output: x Komponente.
 */ 
RtFloat
Rm_xcomp(P)
RtPoint P;
{
  return(P[0]);
}

/*
 * Rm_ycomp: Liefert die y Komponente des Punktes P.
 *      Input: P.
 *      Output: y Komponente.
 */
RtFloat
Rm_ycomp(P)
RtPoint P;
{
  return(P[1]);
}

/*
 * Rm_zcomp: Liefert die z Komponente des Punktes P.
 *      Input: P.
 *      Output: z Komponente.
 */
RtFloat
Rm_zcomp(P)
RtPoint P;
{
  return(P[2]);
}

/*
 * Rm_setxcomp: Belegt die x Komponente von P mit dem Wert val.
 *	Input: P und val.
 *	Output: neues P.
 */
RtVoid
Rm_setxcomp(P,val)
RtPoint P;
RtFloat val;
{
  Vx(P)=val;
}

/*
 * Rm_setycomp: Belegt die y Komponente von P mit dem Wert val.
 *      Input: P und val.
 *      Output: neues P.
 */
RtVoid
Rm_setycomp(P,val)
RtPoint P;
RtFloat val;
{
  Vy(P)=val;
}

/*
 * Rm_setzcomp: Belegt die z Komponente von P mit dem Wert val.
 *      Input: P und val.
 *      Output: neues P.
 */
RtVoid
Rm_setzcomp(P,val)
RtPoint P;
RtFloat val;
{
  Vz(P)=val;
}

/*
 * Rm_length: Berechnet die euklidsche L"ange des Vektors V.
 *	Input: V.
 *	Output: Die L"ange.
 */
RtFloat
Rm_length(V)
RtPoint V;
{
  return lenV3(V);
}

/*
 * Rm_distance: Berechnet den Abstand zwischen zwei Punkten P1 und P2.
 *      Input: P1,P2.
 *      Output: Der Abstand.
 */
RtFloat
Rm_distance(P1,P2)
RtPoint P1,P2;
{
  RtPoint P;

  subV3(P1,P2,P);
  return lenV3(P);
}

/* Rm_normalize: Normalisiert den Vektor V.
 *	Input: V.
 *	Output: Normalisierter Vektor R.
 */
void
Rm_normalize(V,R)
RtPoint V,R;
{
  unitV3(V,R);
}

/*
 * Rm_faceforward: Negiert den Vektor V wenn er aus der Richtung R gesehen
 *		zum Betrachter zeigt. (wenn V und R einen spitzen Winkel
 *		bilden)
 *	Input: V,R.
 *	Output: Korrigierter Vektor.
 */
void
Rm_faceforward(V,R,V_new)
RtPoint V,R,V_new;
{
  if (smulV3(R,V)>0.) mulV3(V,-1.,V_new);
  else copyV3(V,V_new);
}

/* 
 * Rm_reflect: Berechnet den Reflektionsvektor bei einfallendem Strahl I
 *		und der normalen N.
 *	Input: I,N.
 *	Output: Reflektionsvektor R.
 */ 
RtVoid
Rm_reflect(I,N,R)
RtPoint I,N,R;
{
   RtFloat vn,n2,j ;
   RtPoint ntmp;

   vn = smulV3(I,N) ;
   n2 = smulV3(N,N) ;
   j = 2.0*(vn/n2) ;

   mulV3(N,j,ntmp) ;
   subV3(I,ntmp,R) ;
}

/*
 * Farbfunktionen.
 */

/*
 * Rm_mix: Mischt zwei Farben c1,c2 nach der Formel (1-a)*c1+a*c2.
 *	Input: c1,c2,a.
 *	Output: Ergebniss der Mischung in c3.
 */
RtVoid
Rm_mix(c1,c2,a,c3)
RtColor c1,c2;
RtFloat a;
RtColor c3;
{
  RtFloat not_a=(1.-a);

  Cr(c3)=not_a*Cr(c1)+a*Cr(c2); 
  Cg(c3)=not_a*Cg(c1)+a*Cg(c2); 
  Cb(c3)=not_a*Cb(c1)+a*Cb(c2); 
}

RtVoid
Rm_printf(str,va_alist)
char *str;
va_dcl
{
  va_list params;
  char b[80],b2[80];
  int i,j;
  RtFloat *p;
  RtFloat f;
  char *s;

  va_start(params);

  j=0;
  while(1) {
    i=0;
    while (str[j] && str[j] != '%') {
      b[i] = str[j];
      i++;j++;
    }
    if (i) {
      b[i]='\0';
      fprintf(stderr,b);
    }
    if (!str[j]) {va_end(params);return;}
    i=0;
    while (str[j] &&
          ((str[j] >= '0' && str[j] <= '9') || 
           str[j] == '.' || str[j] == '%')) {
      b[i] = str[j];
      i++;j++;
    }
    switch(str[j]) {
      case 'p' : b[i++] = 'g'; b[i] = '\0';
                 p = va_arg(params,RtFloat *);
                 sprintf(b2,"<%s,%s,%s>",b,b,b);
                 fprintf(stderr,b2,p[0],p[1],p[2]);
                 break;
      case 'f' : b[i++] = 'g'; b[i] = '\0';
                 f = va_arg(params,RtFloat);
                 fprintf(stderr,b,f);
                 break;
      case 's' : b[i++] = 's'; b[i] = '\0';
                 s = va_arg(params,char *);
                 fprintf(stderr,b,s);
      default  : sprintf(b2,"?\\%s?",b);
                 fprintf(stderr,b2);
                 break;
    }
    if (!str[++j]) {va_end(params);return;}
  }
}

/*\end{verbatim}*/
