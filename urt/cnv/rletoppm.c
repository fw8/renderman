/*
 * This software is copyrighted as noted below.  It may be freely copied,
 * modified, and redistributed, provided that the copyright notice is
 * preserved on all copies.
 *
 * There is no warranty or other guarantee of fitness for this software,
 * it is provided solely "as is".  Bug reports or fixes may be sent
 * to the author, who may or may not act on them as he desires.
 *
 * You may not include this software in a program or other software product
 * without supplying the source, or without informing the end-user that the
 * source is available for no extra charge.
 *
 * If you modify this software, you should include a notice giving the
 * name of the person performing the modification, the date of modification,
 * and the reason for such modification.
 */
/*
 * rletorla - A conversion program to go from Utah's "rle" image format to
 *            pbmplus ppm true color image format.
 *
 * Author:      Wesley C. Barris
 *              AHPCRC
 *              Minnesota Supercomputer Center, Inc.
 * Date:        Fri July 20 1990
 * Copyright (c) Wesley C. Barris
 */
/*-----------------------------------------------------------------------------
 * System includes.
 */
#include <stdio.h>
#include "rle.h"
#include <ppm.h>

#ifdef USE_STDLIB_H
#include <stdlib.h>
#else

#ifdef USE_STRING_H
#include <string.h>
#else
#include <strings.h>
#define strrchr rindex
#endif

#ifdef VOID_STAR
extern void *malloc();
#else
extern char *malloc();
#endif
extern void free();

#endif /* USE_STDLIB_H */

#define VPRINTF if (verbose || header) fprintf
#define GRAYSCALE   001	/* 8 bits, no colormap */
#define PSEUDOCOLOR 010	/* 8 bits, colormap */
#define TRUECOLOR   011	/* 24 bits, colormap */
#define DIRECTCOLOR 100	/* 24 bits, no colormap */
/*
 * PBMPLUS type declarations.
 */
pixval	maxval	= 255;
/*
 * Utah type declarations.
 */
rle_map		*colormap;
/*
 * Other declarations.
 */
FILE		*fpin;
int		visual, maplen;
int		width, height;
int		verbose = 0, header = 0;
/*-----------------------------------------------------------------------------
 *                                                         Read the rle header.
 */
void read_rle_header()
{
   int	i;
   rle_dflt_hdr.rle_file = fpin;
   rle_get_setup(&rle_dflt_hdr);
   width = rle_dflt_hdr.xmax - rle_dflt_hdr.xmin + 1;
   height = rle_dflt_hdr.ymax - rle_dflt_hdr.ymin + 1;
   VPRINTF(stderr, "Image size: %dx%d\n", width, height);
   if (rle_dflt_hdr.ncolors == 1 && rle_dflt_hdr.ncmap == 3) {
      visual = PSEUDOCOLOR;
      colormap = rle_dflt_hdr.cmap;
      maplen = (1 << rle_dflt_hdr.cmaplen);
      VPRINTF(stderr, "Mapped color image with a map of length %d.\n", maplen);
      }
   else if (rle_dflt_hdr.ncolors == 3 && rle_dflt_hdr.ncmap == 0) {
      visual = DIRECTCOLOR;
      VPRINTF(stderr, "24 bit color image, no colormap.\n");
      }
   else if (rle_dflt_hdr.ncolors == 3 && rle_dflt_hdr.ncmap == 3) {
      visual = TRUECOLOR;
      colormap = rle_dflt_hdr.cmap;
      maplen = (1 << rle_dflt_hdr.cmaplen);
      VPRINTF(stderr, "24 bit color image with color map of length %d\n" ,maplen);
      }
   else if (rle_dflt_hdr.ncolors == 1 && rle_dflt_hdr.ncmap == 0) {
      visual = GRAYSCALE;
      VPRINTF(stderr, "Grayscale image.\n");
      }
   else {
      fprintf(stderr,
              "ncolors = %d, ncmap = %d, I don't know how to handle this!\n",
              rle_dflt_hdr.ncolors, rle_dflt_hdr.ncmap);
      exit(-1);
      }
   if (rle_dflt_hdr.alpha == 0)
      VPRINTF(stderr, "No alpha channel.\n");
   else if (rle_dflt_hdr.alpha == 1)
      VPRINTF(stderr, "Alpha channel exists!\n");
   else {
      fprintf(stderr, "alpha = %d, I don't know how to handle this!\n",
              rle_dflt_hdr.alpha);
      exit(-1);
      }
   switch (rle_dflt_hdr.background) {
      case 0:
         VPRINTF(stderr, "Use all pixels, ignore background color.");
         break;
      case 1:
         VPRINTF(stderr,
                  "Use only non-background pixels, ignore background color.");
         break;
      case 2:
         VPRINTF(stderr,
        "Use only non-background pixels, clear to background color (default).");
         break;
      default:
         VPRINTF(stderr, "Unknown background flag!\n");
         break;
      }
   for (i = 0; i < rle_dflt_hdr.ncolors; i++)
      VPRINTF(stderr, " %d", rle_dflt_hdr.bg_color[i]);
   if (rle_dflt_hdr.ncolors == 1 && rle_dflt_hdr.ncmap == 3) {
      VPRINTF(stderr, " (%d %d %d)\n",
              rle_dflt_hdr.cmap[rle_dflt_hdr.bg_color[0]]>>8,
              rle_dflt_hdr.cmap[rle_dflt_hdr.bg_color[0]+256]>>8,
              rle_dflt_hdr.cmap[rle_dflt_hdr.bg_color[0]+512]>>8);
      }
   else if (rle_dflt_hdr.ncolors == 3 && rle_dflt_hdr.ncmap == 3) {
      VPRINTF(stderr, " (%d %d %d)\n",
              rle_dflt_hdr.cmap[rle_dflt_hdr.bg_color[0]]>>8,
              rle_dflt_hdr.cmap[rle_dflt_hdr.bg_color[1]+256]>>8,
              rle_dflt_hdr.cmap[rle_dflt_hdr.bg_color[2]+512]>>8);
      }
   else
      VPRINTF(stderr, "\n");
   if (rle_dflt_hdr.comments)
      for (i = 0; rle_dflt_hdr.comments[i] != NULL; i++)
         VPRINTF(stderr, "%s\n", rle_dflt_hdr.comments[i]);
}
/*-----------------------------------------------------------------------------
 *                                                    Write the ppm image data.
 */
void write_ppm_data()
{
   rle_pixel		**scanline;
   register pixval	r, g, b;
   register pixel	*pixelrow, *pP;
   register int		scan, x, y;
/*
 *  Allocate some stuff.
 */
   pixelrow = ppm_allocrow(width);
   if (!(rle_row_alloc(&rle_dflt_hdr, &scanline) == 0)) {
      fprintf(stderr, "rle row alloc failed!\n");
      exit(-1);
      }
/*
 * Loop through those scan lines.
 */
   for (scan = 0; scan < height; scan++) {
      y = rle_getrow(&rle_dflt_hdr, scanline);
      switch (visual) {
         case GRAYSCALE:	/* 8 bits without colormap */
            for (x = 0, pP = pixelrow; x < width; x++, pP++) {
               r = scanline[0][x];
               g = scanline[0][x];
               b = scanline[0][x];
               PPM_ASSIGN(*pP, r, g, b);
               }
            break;
         case TRUECOLOR:	/* 24 bits with colormap */
            for (x = 0, pP = pixelrow; x < width; x++, pP++) {
               r = colormap[scanline[0][x]]>>8;
               g = colormap[scanline[1][x]+256]>>8;
               b = colormap[scanline[2][x]+512]>>8;
               PPM_ASSIGN(*pP, r, g, b);
               }
            break;
         case DIRECTCOLOR:	/* 24 bits without colormap */
            for (x = 0, pP = pixelrow; x < width; x++, pP++) {
               r = scanline[0][x];
               g = scanline[1][x];
               b = scanline[2][x];
               PPM_ASSIGN(*pP, r, g, b);
               }
            break;
         case PSEUDOCOLOR:	/* 8 bits with colormap */
            for (x = 0, pP = pixelrow; x < width; x++, pP++) {
               r = colormap[scanline[0][x]]>>8;
               g = colormap[scanline[0][x]+256]>>8;
               b = colormap[scanline[0][x]+512]>>8;
               PPM_ASSIGN(*pP, r, g, b);
               }
            break;
         default:
            break;
         }
/*
 * Write the scan line.
 */
      ppm_writeppmrow(stdout, pixelrow, width, maxval);
      } /* end of for scan = 0 to height */
}
/*-----------------------------------------------------------------------------
 *                               Convert a Utah rle file to a pbmplus ppm file.
 */
int
main(argc, argv)
int argc;
char **argv;
{
   char		*fname = NULL;
/*
 * Get those options.
 */
   if (!scanargs(argc,argv,
       "% v%- h%- infile%s",
       &verbose,
       &header,
       &fname))
      exit(-1);
/*
 * Open the file.
 */
   fpin = rle_open_f( cmd_name( argv ), fname, "r" );
/*
 * Read the rle file header.
 */
   read_rle_header();
   if (header)
      exit(0);
/*
 * Write the ppm file header.
 */
   ppm_writeppminit(stdout, width, height, maxval);
/*
 * Write the rest of the ppm file.
 */
   write_ppm_data();
   fclose(fpin);

   return 0;
}
