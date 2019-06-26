#include <stdio.h>
#include <rle.h>
#include "RiTokens.h"
#include "AdminGlobals.h"
#include "AdminHash.h"
#include "RiError.h"

typedef unsigned char ubyte;

typedef struct {
	int width,height;
	int nchannels;
	RtToken mode;
	ubyte ***map;
	} TextureMap;


Rm_textureV(fn,chan,u,v,color)
char *fn;
RtFloat chan,u,v;
RtColor color;
{
  FILE     *fd;
  struct rle_hdr hdr;
  int i,j,channel = (int)chan;

  TextureMap *tm;

  if (!(tm=(TextureMap *)RmLookUpTexture(fn))) {

#ifdef DEBUG
    fprintf(stderr,"\nreading texture map from '%s' ...",fn);
#endif

    tm=(TextureMap *)RmMalloc(sizeof(TextureMap));
  
    if (!(fd=fopen(fn,"r"))) {
      sprintf(RmMsg,"can't open '%s'",fn); 
      fatal(1,RmMsg);
    }
    hdr.rle_file = fd;
    switch (rle_get_setup (&hdr)) {
      case RLE_SUCCESS: RmMsg[0]='\0'; break;      /* success */
      case RLE_NOT_RLE: sprintf(RmMsg,"'%s' is not a Utah RLE file",fn);
      case RLE_NO_SPACE: sprintf(RmMsg,"no memory for reading RLE file");
      case RLE_EMPTY: sprintf(RmMsg,"'%s' is empty",fn);
      case RLE_EOF: sprintf(RmMsg,"EOF encountered while reading RLE header");
      default: sprintf(RmMsg,"rle_get_setup returned something strange!");
    }
    if (RmMsg[0]) fatal(1,RmMsg); /* something goes wrong */

    if (hdr.alpha) {
      sprintf(RmMsg,"discarding alpha channel of %s",fn);
      warning(1,RmMsg);
      RLE_CLR_BIT (hdr, RLE_ALPHA);
    }

    if (hdr.ncolors != 3) {
      sprintf(RmMsg,"wrong number of planes %d in '%s'",
                    hdr.ncolors,fn);
      fatal(1,RmMsg); /* this is not forever !!! */
    }

    tm->nchannels = hdr.ncolors;
    tm->height = hdr.ymax - hdr.ymin + 1;
    tm->width = hdr.xmax - hdr.xmin + 1;

    tm->map = (ubyte ***)RmMalloc(tm->height*sizeof(ubyte **));

    for (i=0;i<tm->height;i++) {
      tm->map[i]=(ubyte **)RmMalloc(hdr.ncolors*sizeof(ubyte *));
      for (j=0;j<hdr.ncolors;j++) {
        tm->map[i][j]=(ubyte *)RmMalloc(tm->width*sizeof(ubyte));
      }
      rle_getrow (&hdr,tm->map[i]);
    }
    fclose(fd);

    tm->mode = RI_WRAP; /* immoment */

    RmInstallNewTexture(fn,tm);
#ifdef DEBUG
    fprintf(stderr,"done\n");
#endif
  }

  
  if (tm->mode == RI_WRAP) {
    while (u<0.) u += 1.;
    while (v<0.) v += 1.;
    while (u>=1.) u -= 1.;
    while (v>=1.) v -= 1.;
  }

  for (i=0;i<3 && i+channel < tm->nchannels;i++) {
    color[i] = tm->map[(int)(v * tm->height)][i+channel][(int)(u * tm->width)];
    color[i] *= 0.003921568;  /* 1/255 */
  }

}
