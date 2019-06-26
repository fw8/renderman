/* \subsection{Screen.c} \label{Screen.c}
Das Screen-Modul!

Hier wird die ganze Bildschirmverwaltung "ubernommen, unter anderem
auch die Generierung des Bildes.
\begin{verbatim} */

#include "Admin.h"
#include "Misc.h"
#include "Screen.h"
#include "Math.h"
#include "Stat.h"

  ScreenGlobals	RmScreenGlobals;

#define SG RmScreenGlobals

/* \end{verbatim} \label{Screen.c:RiGaussianFilter} \begin{verbatim}
 * Wie jede Filterfunktion liefert RiGaussian die Wichtigkeit des Samples
   an der Stelle x,y zur"uck.
 *
 *		INPUT : x = x-Entfernung vom MIttelpunkt
 *			y = y-Entfernung vom MIttelpunkt
 *			xwidth = Ausdehnung des Filters in x
 *			ywidth = Ausdehnung des Filters in y
 *		OUTPUT: Wertigkeit des Samples
*/ 
RtFloat
RiGaussianFilter(x,y,xwidth,ywidth)
	RtFloat x,y;
	RtFloat xwidth,ywidth;
{
  if ((x >= -(xwidth/2)) && (x <= (xwidth/2)) && 
      (y >= -(ywidth/2)) && (y <= (ywidth/2))) 
        return(exp(-(2*sqr( sqrt(sqr(x)+sqr(y)) ))));
  else
	return(0.);
}
/* \end{verbatim} \label{Screen.c:RiBoxFilter} \begin{verbatim}
 * Wie jede Filterfunktion liefert BoxFilter die Wichtigkeit des Samples
   an der Stelle x,y zur"uck.
 *
 *              INPUT : x = x-Entfernung vom MIttelpunkt
 *                      y = y-Entfernung vom MIttelpunkt
 *                      xwidth = Ausdehnung des Filters in x
 *                      ywidth = Ausdehnung des Filters in y
 *              OUTPUT: Wertigkeit des Samples
*/
RtFloat
RiBoxFilter(x,y,xwidth,ywidth)
        RtFloat x,y;
        RtFloat xwidth,ywidth;
{
  if ((x >= -(xwidth/2)) && (x <= (xwidth/2)) &&
      (y >= -(ywidth/2)) && (y <= (ywidth/2)))
        return(1.);
  else
        return(0.);
}

/* \end{verbatim} \label{Screen.c:_init_ScreenGlobals} \begin{verbatim}
 * _init_ScreenGlobals: Initialisierung aller globalen Bildvariablen.
 */
void
_init_ScreenGlobals()
{
  RmScreenGlobals.xsamples = 2;
  RmScreenGlobals.ysamples = 2;
  RmScreenGlobals.filterfunction = RiGaussianFilter;
  RmScreenGlobals.xwidth = 1;
  RmScreenGlobals.ywidth = 1;

  RmScreenGlobals.z_clip_near = RI_EPSILON;
  RmScreenGlobals.z_clip_far  = RI_INFINITY;

  RmScreenGlobals.scr_left   = -1.;
  RmScreenGlobals.scr_right  =  1.;
  RmScreenGlobals.scr_bottom = -1.;
  RmScreenGlobals.scr_top    =  1.;
  RmScreenGlobals.field_of_view = PId2;
  RmScreenGlobals.scr_default = RI_TRUE;

  RmScreenGlobals.res_x = 640;
  RmScreenGlobals.res_y = 480;
  RmScreenGlobals.org_x = 0;
  RmScreenGlobals.org_y = 0;

  RmScreenGlobals.PAR   = 1.;
  RmScreenGlobals.FAR   = 0.;

  RmScreenGlobals.crop_left   = 0.;
  RmScreenGlobals.crop_right  = 1.;
  RmScreenGlobals.crop_top    = 0.;
  RmScreenGlobals.crop_bottom = 1.;

}
/* \end{verbatim} \label{Screen.c:_begin_Screen} \begin{verbatim}
 * _begin_Screen: Berechnung aller anderen globalen Bildvariablen.
 */
void
_begin_Screen()
{
  RtFloat help;
  RtFloat IAR;

  if (RmScreenGlobals.scr_default) {
    if (RmScreenGlobals.FAR == 0.) 
      RmScreenGlobals.FAR = ((RtFloat)RmScreenGlobals.res_x * 
                      RmScreenGlobals.PAR) / (RtFloat)RmScreenGlobals.res_y;
    if (RmScreenGlobals.FAR >= 1.) {
      RmScreenGlobals.scr_left *= RmScreenGlobals.FAR;
      RmScreenGlobals.scr_right *= RmScreenGlobals.FAR;
    }else{
      RmScreenGlobals.scr_bottom *= 1./RmScreenGlobals.FAR;
      RmScreenGlobals.scr_top *= 1./RmScreenGlobals.FAR;
    }
  }else{
    if (RmScreenGlobals.FAR == 0.)
      RmScreenGlobals.FAR = ((RtFloat)RmScreenGlobals.res_x * 
                      RmScreenGlobals.PAR) / (RtFloat)RmScreenGlobals.res_y;
  /*    RmScreenGlobals.FAR = (fabs(RmScreenGlobals.scr_left)+
            fabs(RmScreenGlobals.scr_right)) / 
            (fabs(RmScreenGlobals.scr_bottom)+fabs(RmScreenGlobals.scr_top)); 
  */
  }
  IAR = RmScreenGlobals.FAR/RmScreenGlobals.PAR;
  if (RmScreenGlobals.res_y > RmScreenGlobals.res_x) {
    if (IAR < 1.) {
      RmScreenGlobals.res_x = (RtInt) ((RtFloat)RmScreenGlobals.res_y * IAR);
    }else{
      RmScreenGlobals.res_y = (RtInt) ((RtFloat)RmScreenGlobals.res_x * 1./IAR);
    }
  }else{
    if (IAR < 1.) {
      RmScreenGlobals.res_y = (RtInt) ((RtFloat)RmScreenGlobals.res_x * IAR);
    }else{
      RmScreenGlobals.res_x = (RtInt) ((RtFloat)RmScreenGlobals.res_y * 1./IAR);
    }
  }

#ifdef DEBUG
  fprintf(stderr,"\npicture resolution [%d,%d]\n",RmScreenGlobals.res_x,
         RmScreenGlobals.res_y);
  fprintf(stderr,"screen window [(%g,%g);(%g,%g)]\n",RmScreenGlobals.scr_left,
  RmScreenGlobals.scr_top,RmScreenGlobals.scr_right,RmScreenGlobals.scr_bottom);
  fprintf(stderr,"frame aspect ratio : %g\n",RmScreenGlobals.FAR);
  fprintf(stderr,"pixel aspect ratio : %g\n",RmScreenGlobals.PAR);
  fprintf(stderr,"field of view      : %g\n",RmScreenGlobals.field_of_view);
#endif

  RmScreenGlobals.pstep_x = (RmScreenGlobals.scr_right - 
			RmScreenGlobals.scr_left) / RmScreenGlobals.res_x;
  RmScreenGlobals.pstep_y = (RmScreenGlobals.scr_bottom - 
			RmScreenGlobals.scr_top) / RmScreenGlobals.res_y;
  RmScreenGlobals.sstep_x = RmScreenGlobals.pstep_x / RmScreenGlobals.xsamples;
  RmScreenGlobals.sstep_y = RmScreenGlobals.pstep_y / RmScreenGlobals.ysamples;

  RmScreenGlobals.saminx = (int) (RmScreenGlobals.res_x * 
			RmScreenGlobals.xsamples);
  help = RmScreenGlobals.ywidth*RmScreenGlobals.ysamples;
  if (help != (int)help) help+=1.0;
  RmScreenGlobals.saminy = (int) help;
  if ( (RmScreenGlobals.saminy % 2) == 0 ) RmScreenGlobals.saminy += 1;
  RmScreenGlobals.xoffset= ((RmScreenGlobals.xwidth-1.)*
				RmScreenGlobals.pstep_x)/2.;
  RmScreenGlobals.yoffset= ((RmScreenGlobals.ywidth-1.)*
				fabs(RmScreenGlobals.pstep_y)/2);


  help = RmScreenGlobals.xoffset/RmScreenGlobals.sstep_x;
  if (help != (int)help) help+=1.0;
  RmScreenGlobals.xsamoffset = (RtInt) help;
  help = fabs(RmScreenGlobals.yoffset/RmScreenGlobals.sstep_y);
  if (help != (int)help) help+=1.0;
  RmScreenGlobals.ysamoffset = (RtInt) help;


  /* Wieviele Sample brauchen wir denn nun als Buffer ?  */ 

  RmScreenGlobals.anzsamples = (RmScreenGlobals.saminx + 
	(RmScreenGlobals.xsamoffset*2) ) * RmScreenGlobals.saminy; 
  RmScreenGlobals.samplebuffer = (Sample *)RmMalloc(RmScreenGlobals.anzsamples*
					sizeof(Sample));
}

void
_end_Screen()
{
  RmFree(RmScreenGlobals.samplebuffer);
}
/* \end{verbatim} \label{Screen.c:RmGenRayOrthographic} \begin{verbatim}
 * RmGenRayOrthographic: Strahlgenerierung bei orthogonaler Sicht.
 *
 *	INPUT :	x,y = Ursprung des Starhls.
 *		ray = Zeiger auf Strahl
 */
void
RmGenRayOrthographic(x,y,ray)
float x,y;
Ray    *ray;
{
  setV3(x,y,0.,ray->P); /* put ray-origin at (x,y,0) */
  setV3(0.,0.,1.,ray->D);   /* and ray-direction at (0,0,1) */
  RmTransformRay(ray,RmGlobals.CtoW,ray);
}
/* \end{verbatim} \label{Screen.c:RmGenRayPerspective} \begin{verbatim}
 * RmGenRayPerspective: Strahlgenerierung bei perspektivischer Sicht.
 *
 *      INPUT : x,y = Ursprung des Starhls.
 *              ray = Zeiger auf Strahl
 */
void
RmGenRayPerspective(x,y,ray)
float x,y;
Ray    *ray;
{
  setV3(0.,0.,0.,ray->P); /* put ray-origin at (0,0,0) */
  setV3(x,y,1.,ray->D);   /* and ray-direction at (x,y,1) */
  unitV3(ray->D,ray->D);  /* unitize ray-direction */
  RmTransformRay(ray,RmGlobals.CtoW,ray);
}


/* \end{verbatim} \label{Screen.c:RmGeneratePic} \begin{verbatim}
 * RmGeneratePic:
 *		Hier wird das Bild erzeugt, indem in einer entsprechenden
 *		Schleife alle Sample die n"otig sind durchlaufen werden.
 *		Aus ihren Werten, werden je nacvh Filtering dann die Werte
 *		f"ur die Pixel ermittelt, und an die Ausgabe "ubergeben.
 */
void
RmGeneratePic()
{
  RtFloat start_sx;
  RtFloat cur_sx,cur_sy;	/* Momentane x,y Screenkoordinaten */
  RtFloat rx,ry;		/* Zufallswerte */
  Sample  *cur_sample;		/* Zeiger auf momentane Position 
				   im Samplebuffer */
  Ray	  ray;			/* Strahl */
  int   i,k,l;			/* Hilfsvariablen */
  RtFloat cur_px,cur_py;	/* Current Pixelcoordinates */
  RtColor col,retcol;		/* Farbvariablen */
  int   cur_pnumber;		/* Momentane Pixelnummer */
  RtFloat cur_snumber;		/* Momentane Samplenummer */
  int   cur_pos,helppos;	/* Hilfsvaribalen */
  RtFloat samplewidth;		/* Die Samplerate */
  int   sperline;		/* Samples pro Zeile */
  float dx,dy;			/* Abweichungen vom mittelwert */
  float wertigkeit;		/* Wertigkeit eines Samples */
  float gesamtwert;		/* Summe aller Wertigkeiten einer Filtermask */
  int offset,ynumber,y2number;	/* Hilfsvariablen */
  Sample *sample1ptr,*sample2ptr; /* Hilfszeiger */
  RtFloat ysamplecount; 	/* die korrkete momentane y-Samplezahl */
  int   ysamplepos; 		/* y-Position der ersten Zeile im Buffer */
  RtFloat pcropxmin,pcropxmax,pcropymin,pcropymax; /* CropWindow */
  int   cropflag; 		/* 0 = Kein CropWindow aktiv */

  int  LastBoundinRegionTests; /* used to show bbox performance */

  /* Ermittlung der Begrenzungen des Cropwindows, falls dieses vorhanden ist */

  pcropxmin = RmScreenGlobals.scr_left + ((RmScreenGlobals.scr_right 
              - RmScreenGlobals.scr_left) * RmScreenGlobals.crop_left);
  pcropxmax = RmScreenGlobals.scr_left + ((RmScreenGlobals.scr_right 
              - RmScreenGlobals.scr_left) * RmScreenGlobals.crop_right);
  pcropymin = RmScreenGlobals.scr_top - ((RmScreenGlobals.scr_top 
              - RmScreenGlobals.scr_bottom) * RmScreenGlobals.crop_bottom);
  pcropymax = RmScreenGlobals.scr_top - ((RmScreenGlobals.scr_top 
              - RmScreenGlobals.scr_bottom) * RmScreenGlobals.crop_top);
  cropflag = 0;
  if ((RmScreenGlobals.crop_left!=0.)||(RmScreenGlobals.crop_right!=1.0)
       ||(RmScreenGlobals.crop_bottom!=1.)||(RmScreenGlobals.crop_top!=0.)) 
     cropflag = 1;
       

  /* Berechnung einiger ben"otigter Variablen */

  ysamplecount = ysamplepos = - RmScreenGlobals.ysamoffset;
  sperline = RmScreenGlobals.saminx+(2*RmScreenGlobals.xsamoffset);
  cur_sx = start_sx = RmScreenGlobals.scr_left - (RmScreenGlobals.xsamoffset 
                      * RmScreenGlobals.sstep_x) + RmScreenGlobals.sstep_x/2;
  cur_sy = RmScreenGlobals.scr_top + (RmScreenGlobals.ysamoffset 
             * fabs(RmScreenGlobals.sstep_y)) + RmScreenGlobals.sstep_y/2;
  cur_sample = RmScreenGlobals.samplebuffer;

  /* Initialisation of samplebuffer with starting values */

  for (i=0;i<RmScreenGlobals.saminy;i++) { /* Alle Samplelines, die f"ur
					    einen Durchlauf ben"otigt werden */
    for (k=0;k<sperline;k++) {  /* Die komplette Zeile */
      if ( (!cropflag) ||	/* Falls im aktiven Fensterbereich */
          ((pcropxmin <= (cur_sx-(RmScreenGlobals.xwidth*
            RmScreenGlobals.xsamples*RmScreenGlobals.sstep_x/2))) &&
           (pcropxmax >= (cur_sx+(RmScreenGlobals.xwidth*
            RmScreenGlobals.xsamples*RmScreenGlobals.sstep_x/2))) &&
           (pcropymin <= (cur_sy-(RmScreenGlobals.ywidth*
            RmScreenGlobals.ysamples*RmScreenGlobals.sstep_y/2))) &&
           (pcropymax >= (cur_sy+(RmScreenGlobals.ywidth*
            RmScreenGlobals.ysamples*RmScreenGlobals.sstep_y/2))))) { 

#ifdef JITTER	/* es soll ein zuf"alliger Punkt im Sample als Bezugspunkt f"ur
		   die Berechnung erzeugt werden. */
        rx = RmRnd(RmScreenGlobals.sstep_x); 
        ry = RmRnd(RmScreenGlobals.sstep_y);
 	
	/* x,y-Wert nach diesen Zufallswerten setzen. */

        cur_sample->x = cur_sx - (RmScreenGlobals.sstep_x/2) + rx;
        cur_sample->y = cur_sy - (RmScreenGlobals.sstep_y/2) + ry;
#else
	rx = 0; 
        ry = 0;

	/* Na sonst nehmen wir genau den Mittelpunkt des Samples */

        cur_sample->x = cur_sx; 
        cur_sample->y = cur_sy;
#endif
	/* Nun Strahl entsprechend erzeugen und Tracen ! */

        (*RmGlobals.GenRay)(cur_sample->x,cur_sample->y,&ray);

	LastBoundinRegionTests=RmStat.BoundinRegionTests;
        RmTrace(0,&ray,cur_sample->color);
#ifdef SHOWBOUNDS
	{ float scale = 
               (float) (RmStat.BoundinRegionTests-LastBoundinRegionTests);
               if (scale) {
			scale = 1. - 1./scale;
			cur_sample->color[0]*=scale;
			cur_sample->color[1]*=scale;
			cur_sample->color[2]*=scale;
		}
	}
#endif
      }
      cur_sx += RmScreenGlobals.sstep_x;	/* N"achstes Sample der Zeile */
      cur_sample++;	/* Bufferzeiger erh"ohen */
    }
    cur_sx = start_sx;	/* x wieder auf Anfang einer Zeile setzen */
    cur_sy += RmScreenGlobals.sstep_y; /* Und n"achste Samplezeile */
  }

  /* Und nun die Ermittlung aller Pixelwerte, die sich aus unserem minimalen
     Samplebuffer berechnen lassen.

     Anfangswerte berechnen:
  */

  cur_px = RmScreenGlobals.scr_left + RmScreenGlobals.pstep_x/2;
  cur_py = RmScreenGlobals.scr_top + RmScreenGlobals.pstep_y/2;
  cur_pnumber = 0;
  cur_snumber = 0.0;
  samplewidth = RmScreenGlobals.xsamples * RmScreenGlobals.xwidth;
  cur_sample = RmScreenGlobals.samplebuffer;


  for (l=0;l<RmScreenGlobals.res_y;l++) { /* All Pixels in y-Richtung */
 
    fprintf(stderr,"\r     \r<%d>",l);

    for (i=0;i<RmScreenGlobals.res_x;i++) { /* All Pixels in this line */

      col[0]=col[1]=col[2]=0.;	/* Farbe auf schwarz (Hintergrund) setzen */
      gesamtwert = 0.;
  
      for(k=0;k<RmScreenGlobals.saminy;k++) {	 /* All SampleLines */
  
        cur_pos = helppos = (RtInt) (cur_snumber); /* Buffer positionieren */
  
        while (cur_pos < (cur_snumber + samplewidth)) { /* Solange noch 
					relevante Samples da sind */

	  if ( (!cropflag) ||	/* und es sich im aktiven Fenster befindet */
	      ((pcropxmin <= (cur_px-(RmScreenGlobals.xwidth*
                RmScreenGlobals.pstep_x/2))) &&
               (pcropxmax >= (cur_px+(RmScreenGlobals.xwidth*
                RmScreenGlobals.pstep_x/2))) &&
               (pcropymin <= (cur_py-(RmScreenGlobals.ywidth*
                RmScreenGlobals.pstep_y/2))) &&
               (pcropymax >= (cur_py+(RmScreenGlobals.ywidth*
                RmScreenGlobals.pstep_y/2))))) {

		/* Abweichung vom Mittelpunkt berechnen */
 
            dx = cur_px-cur_sample[cur_pos-helppos].x;
	    dx /= RmScreenGlobals.pstep_x; /* im Verh"altnis zur Pixelweite */
            dy = cur_py-cur_sample[cur_pos-helppos].y;
            dy /= RmScreenGlobals.pstep_y;

		/* Wertigkeit des Samples "uber Filterfunktion berechnen */

  	    wertigkeit = (* RmScreenGlobals.filterfunction)(dx,dy,
                            RmScreenGlobals.xwidth,RmScreenGlobals.ywidth);

            if (wertigkeit) {	/* Falls "uberhaupt an Farbgebung beteiligt */
				/* Farbe anteilsm"a"sig dazu */
              col[0] += wertigkeit*cur_sample[cur_pos-helppos].color[0];
  	      col[1] += wertigkeit*cur_sample[cur_pos-helppos].color[1];
  	      col[2] += wertigkeit*cur_sample[cur_pos-helppos].color[2];
	      gesamtwert += wertigkeit;
            }
	  }
          cur_pos++;
        }  	
	(int) cur_sample += sperline*sizeof(Sample); /* Next SampleLine */
      }

      if (gesamtwert) {	/* Farbe normieren */
        col[0] /= gesamtwert;
        col[1] /= gesamtwert;
        col[2] /= gesamtwert;
      }
 
	/* Und den Pixelwert ausgeben */

      RmWritePic(col);

	/* Variablen aktualisieren */

      cur_pnumber++;
      cur_px += RmScreenGlobals.pstep_x;
      cur_snumber += RmScreenGlobals.xsamples;
      cur_sample = RmScreenGlobals.samplebuffer;
      offset = (int) cur_snumber;
      offset *= sizeof(Sample);
      (int) cur_sample += offset;

    }
    
    /* Und nun den Samplebuffer aktuaslisieren !!! */

    cur_pnumber = 0;
    cur_snumber = 0.0;
    cur_px = RmScreenGlobals.scr_left + RmScreenGlobals.pstep_x/2;
    cur_py += RmScreenGlobals.pstep_y;
    cur_sample = RmScreenGlobals.samplebuffer;
    ysamplecount += RmScreenGlobals.ysamples;

    /* Wieviele Zeilen k"onnen vom alten durch Scrolling "ubernommen werden ? */

    ynumber = abs((int)(ysamplecount) - ysamplepos); 
    y2number = ynumber;
    y2number *= (sperline * sizeof(Sample));
    ysamplepos = (int) ysamplecount;
    sample1ptr = cur_sample;
    sample2ptr = cur_sample;
    (int)sample2ptr += y2number;
    (int) cur_sample += RmScreenGlobals.anzsamples * sizeof(Sample);

	/* Samples die wieder gebracuht werden nach "oben" kopieren */

    while (sample2ptr < cur_sample) {
      *sample1ptr++ = *sample2ptr++;
    } 

    for (i=0;i<ynumber;i++) { /* Anzahl der Zeilen die noch n"otig sind neu
				 zu berechnen */
      for (k=0;k<sperline;k++) { /* Samples pro Zeile */
        if ( (!cropflag) || /* falls im aktiven Bereich des Fensters */
            ((pcropxmin <= (cur_sx-(RmScreenGlobals.xwidth*
              RmScreenGlobals.xsamples*RmScreenGlobals.sstep_x/2))) &&
             (pcropxmax >= (cur_sx+(RmScreenGlobals.xwidth*
              RmScreenGlobals.xsamples*RmScreenGlobals.sstep_x/2))) &&
             (pcropymin <= (cur_sy-(RmScreenGlobals.ywidth*
              RmScreenGlobals.ysamples*RmScreenGlobals.sstep_y/2))) &&
             (pcropymax >= (cur_sy+(RmScreenGlobals.ywidth*
              RmScreenGlobals.ysamples*RmScreenGlobals.sstep_y/2)))) ) {
#ifdef JITTER
	/* es soll ein zuf"alliger Punkt im Sample als Bezugspunkt f"ur
                   die Berechnung erzeugt werden. */
        rx = RmRnd(RmScreenGlobals.sstep_x);
        ry = RmRnd(RmScreenGlobals.sstep_y);
        sample1ptr->x = cur_sx - (RmScreenGlobals.sstep_x/2) + rx;
        sample1ptr->y = cur_sy - (RmScreenGlobals.sstep_y/2) + ry;
#else
        rx = 0;	/* Wir wollen den Mittelpunkt haben */
        ry = 0;
        sample1ptr->x = cur_sx;
        sample1ptr->y = cur_sy;
#endif

	/* Strahl generieren und Tracen !! */

          (* RmGlobals.GenRay)(sample1ptr->x,sample1ptr->y,&ray);
          LastBoundinRegionTests=RmStat.BoundinRegionTests;
          RmTrace(0,&ray,sample1ptr->color);
#ifdef SHOWBOUNDS
        { float scale =
               (float) (RmStat.BoundinRegionTests-LastBoundinRegionTests);
               if (scale) {
                        scale = 1. - 1./scale;
                        sample1ptr->color[0]*=scale;
                        sample1ptr->color[1]*=scale;
                        sample1ptr->color[2]*=scale;
                }
        }
#endif


        }
        cur_sx += RmScreenGlobals.sstep_x; /* N"achstes Sample */
        sample1ptr++;
      }
      cur_sx = start_sx;	/* Zeilenbeginn */
      cur_sy += RmScreenGlobals.sstep_y; /* N"achste Zeile */
      sample1ptr++;
    }
    
    cur_sample = RmScreenGlobals.samplebuffer; /* Zeiger auf Buffer 
						  aktualisieren */

  }	/* Und der ganze Spa"s auf`s Neue !! */
}
/*\end{verbatim}*/
