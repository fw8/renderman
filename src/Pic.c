/* \subsection{Pic.c} \label{Pic.c}
 * Die Routinen f"ur die Ausgabe des Bildes im RLE-Format
\begin{verbatim}*/
#include <stdio.h>
#include <rle.h>
#include "RiTypes.h"
#include "AdminGlobals.h"
#include "AdminMem.h"
#include "Misc.h"
#include "RiError.h"

static struct rle_hdr pic_hdr;
static unsigned char *buffer[3];       /* Output buffer */

/*  \end{verbatim} \label{Pic.c:correct} \begin{verbatim}
 * Convert floating-point to unsigned char (0-255).
 * This could easily be a macro, but the old SGI compilers dump core
 * on it for some reason.
 */
static unsigned char
correct(x)
int x;
{
        if (x < 0)
                return 0;
        if (x > 255)
                return 255;
        return (unsigned char)x;
}

static unsigned char
clamp(fvalue) 
double fvalue;
{
   int ivalue;

   ivalue = (int)(fvalue*RmGlobals.Quant_one +
                  RmGlobals.Quant_ditheramplitude * (RmRnd(2.)-1.) + .5
                 );
   if (ivalue < RmGlobals.Quant_min) ivalue = RmGlobals.Quant_min;
   if (ivalue > RmGlobals.Quant_max) ivalue = RmGlobals.Quant_max;

   return (unsigned char)ivalue;
}


static FILE *outfile;
static int lc,xres;
/*  \end{verbatim} \label{Pic.c:RmOpenPic} \begin{verbatim}
*/
void
RmOpenPic(fn,width,height,x_off,y_off)
char *fn;
int  width,height,x_off,y_off;
{
   if (*fn=='\0') {

#ifdef DEBUG
      fprintf(stderr,"writing to stdout");
#endif

      outfile=stdout;
   }else if (!(outfile=fopen(fn,"wb"))) {
      sprintf(RmMsg,"cannot open '%s'",fn);
      fatal(0,RmMsg);
   }

   pic_hdr.rle_file = outfile;
   pic_hdr.xmax = width-1;
   pic_hdr.ymax = height-1;
   pic_hdr.xmin = pic_hdr.ymin = 0;
   pic_hdr.ncolors = 3;
   pic_hdr.alpha = 0;
   RLE_SET_BIT(pic_hdr,RLE_RED);
   RLE_SET_BIT(pic_hdr,RLE_GREEN);
   RLE_SET_BIT(pic_hdr,RLE_BLUE);
   pic_hdr.dispatch = RUN_DISPATCH;
   
   rle_put_setup(&pic_hdr);

   /*Flush the header. */
   fflush(pic_hdr.rle_file);
   buffer[0]=(unsigned char *)RmMalloc(sizeof(unsigned char)*(unsigned)width);
   buffer[1]=(unsigned char *)RmMalloc(sizeof(unsigned char)*(unsigned)width);
   buffer[2]=(unsigned char *)RmMalloc(sizeof(unsigned char)*(unsigned)width);
   lc=0; /* reset line counter */
   xres=width;
}
/*  \end{verbatim} \label{Pic.c:RmWritePic} \begin{verbatim}
*/
void
RmWritePic(in)
RtColor in;
{
   RtColor out;

   out[0] = pow(in[0]*RmGlobals.Exposure_gain,1./RmGlobals.Exposure_gamma);
   out[1] = pow(in[1]*RmGlobals.Exposure_gain,1./RmGlobals.Exposure_gamma);
   out[2] = pow(in[2]*RmGlobals.Exposure_gain,1./RmGlobals.Exposure_gamma);

   buffer[0][lc] = clamp(out[0]);
   buffer[1][lc] = clamp(out[1]);
   buffer[2][lc] = clamp(out[2]);
/*
   buffer[0][lc]=correct(r);
   buffer[1][lc]=correct(g);
   buffer[2][lc]=correct(b);
*/
   if (++lc==xres) {
      rle_putrow(buffer, xres, &pic_hdr);
      lc=0;
   }
} 


void
_end_Pic()
{
   rle_puteof(&pic_hdr);
}
/*\end{verbatim}*/
