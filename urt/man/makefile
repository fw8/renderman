#%#### makefile AUTOMATICALLY GENERATED ######
#%############### DO NOT EDIT ################
#%### EDIT makefile.src AND make config ######
DEPTH = ../
CONFIG = ../config/urt
HERE = man/
# Sony NEWS configuration file.
# With X11.
# All conversion programs compiled.

ExtraCFLAGS = 

ROFF = ditroff
ROFFOPT = -man
ROFFPIPE = | lpr -n

OTHERJUNK = core *~ #*#

LIBX11 = -lX11
INCX11 = 

RI = ../include
RL = ../lib
DIRS =
ALLDIRS =
PGMS =

ExtraLIBS = 
LIBS = -L$(RL) -lrle $(DIRLIBS) $(ExtraLIBS)

MachFLAGS =  -f68881
CFLAGS = -O $(DFLAGS) $(DIRFLAGS) $(ExtraCFLAGS) $(IFLAGS) $(MachFLAGS) 
IFLAGS = -I. -I$(RI)

SHELL = /bin/sh

#
# Makefile for Utah Raster Toolkit documentation directory
#

default: all

# Only set DEST if MAN_DEST is defined.
#ifdef MAN_DEST
#set	DEST	MAN_DEST
DEST = /mo2/ip/man
#endif MAN_DEST

# Destination directories for manual pages
DEST1 = $(DEST)/man1
DEST3 = $(DEST)/man3
DEST5 = $(DEST)/man5
SUFF1 = 1

# Program manual pages, except those configured below.
PAGES1 = \
	man1/applymap.1 \
	man1/avg4.1 \
	man1/crop.1 \
	man1/dvirle.1 \
	man1/fant.1 \
	man1/into.1 \
	man1/mcut.1 \
	man1/mergechan.1 \
	man1/pyrmask.1 \
	man1/rawtorle.1 \
	man1/repos.1 \
	man1/rleClock.1 \
	man1/rleaddcom.1 \
	man1/rleaddeof.1 \
	man1/rlebg.1 \
	man1/rlebox.1 \
	man1/rlecat.1 \
	man1/rlecomp.1 \
	man1/rledither.1 \
	man1/rleflip.1 \
	man1/rlehdr.1 \
	man1/rlehisto.1 \
	man1/rleldmap.1 \
	man1/rlemandl.1 \
	man1/rlenoise.1 \
	man1/rlepatch.1 \
	man1/rleprint.1 \
	man1/rlequant.1 \
	man1/rlescale.1 \
	man1/rleselect.1 \
	man1/rlesetbg.1 \
	man1/rlespiff.1 \
	man1/rlesplice.1 \
	man1/rlesplit.1 \
	man1/rlestereo.1 \
	man1/rleswap.1 \
	man1/rletoascii.1 \
	man1/rletoraw.1 \
	man1/rlezoom.1 \
	man1/smush.1 \
	man1/to8.1 \
	man1/tobw.1 \
	man1/unexp.1 \
	man1/unslice.1 \
	man1/urt.1 \
	$(XPAGES1)

# Installation and printing of manual pages depends on configuration

#ifdef ALL_MAN
#define	CUBICOMP
#define	MACPAINT
#define	ABEKASA60
#define	ABEKASA62
#define TARGA
#define	WASATCH
#define	GRAYFILES
#define GIF
#define TIFF
#define	FB
#define	XWINDOWS
#define APOLLO
#define IRIS
#define IRIS4D
#define BOBCAT
#define SUNTOOLS
#define CX3D
#define X11
#define HPQCR
#define TAAC1
#define WAVEFRONT
#define PBMPLUS
#endif

#ifadd	ABEKASA60	XPAGES1		man1/rletoabA60.1
#ifadd	ABEKASA62	XPAGES1		man1/rletoabA62.1
#ifadd	APOLLO		XPAGES1		man1/getap.1
#ifadd	BOBCAT		XPAGES1		man1/getbob.1 man1/getren.1 
#ifadd	BOBCAT		XPAGES1		man1/read98721.1
#ifadd	CUBICOMP	XPAGES1		man1/cubitorle.1
#ifadd	CX3D		XPAGES1		man1/getcx3d.1
#ifadd	FB		XPAGES1		man1/getfb.1
#ifadd	GIF		XPAGES1		man1/giftorle.1 man1/rletogif.1
#ifadd	GRAYFILES	XPAGES1		man1/graytorle.1 man1/rletogray.1
#ifadd	GRINNELL	XPAGES1		man1/getgmr.1
#ifadd	HPQCR		XPAGES1		man1/getqcr.1
#ifadd	IRIS		XPAGES1		man1/getiris.1 man1/getmex.1
#ifadd	IRIS4D		XPAGES1		man1/get4d.1
#ifadd	MACPAINT	XPAGES1		man1/painttorle.1 man1/rletopaint.1
#ifadd	ORION		XPAGES1		man1/get_orion.1
#ifadd	PBMPLUS		XPAGES1		man1/pgmtorle.1 man1/ppmtorle.1
#ifadd	PBMPLUS		XPAGES1		man1/rletoppm.1
#ifadd	POSTSCRIPT	XPAGES1		man1/rletops.1
#ifadd	SUNRASTER	XPAGES1		man1/rastorle.1 man1/rletorast.1
#ifadd	SUNTOOLS	XPAGES1		man1/getsun.1
#ifadd	TAAC1		XPAGES1		man1/gettaac.1
#ifadd	TARGA		XPAGES1		man1/targatorle.1
#ifadd	TIFF		XPAGES1		man1/rletotiff.1 man1/tifftorle.1
#ifadd	WASATCH		XPAGES1		man1/wasatchrle.1
#ifadd	WAVEFRONT	XPAGES1		man1/rlatorle.1 man1/rletorla.1
#ifadd	X10		XPAGES1		man1/getx10.1
#ifadd	X11		XPAGES1		man1/getx11.1
#emit XPAGES1
XPAGES1 =  man1/giftorle.1 man1/rletogif.1 man1/graytorle.1 man1/rletogray.1 man1/painttorle.1 man1/rletopaint.1 man1/rletops.1 man1/targatorle.1 man1/getx11.1

# Must list names explicitly, as some are longer than 14 chars, so *.3
# won't work.
PAGES3 = ${MANPAGES3} ${SOPAGES3}

MANPAGES3 = \
	man3/buildmap.3 \
	man3/colorquant.3 \
	man3/dither.3 \
	man3/float_to_exp.3 \
	man3/inv_cmap.3 \
	man3/librle.3 \
	man3/rgb_to_bw.3 \
	man3/rle_addhist.3 \
	man3/rle_cp.3 \
	man3/rle_get_set.3 \
	man3/rle_getraw.3 \
	man3/rle_getrow.3 \
	man3/rle_getskip.3 \
	man3/rle_hdr.3 \
	man3/rle_op.3 \
	man3/rle_open_f.3 \
	man3/rle_put_init.3 \
	man3/rle_putcom.3 \
	man3/rle_puteof.3 \
	man3/rle_putraw.3 \
	man3/rle_putrow.3 \
	man3/rle_raw_free.3 \
	man3/rle_rawtorow.3 \
	man3/rle_row_free.3 \
	man3/rle_skiprow.3

SOPAGES3 = \
	man3/bwdithermap.3 \
	man3/ditherbw.3 \
	man3/dithergb.3 \
	man3/dithermap.3 \
	man3/make_square.3 \
	man3/rle_debug.3 \
	man3/rle_delcom.3 \
	man3/rle_freeraw.3 \
	man3/rle_get_error.3 \
	man3/rle_get_setup.3 \
	${GET_SETUP_OK_PAGE3} \
	man3/rle_getcom.3 \
	man3/rle_open_f_noexit.3 \
	man3/rle_put_setup.3 \
	man3/rle_raw_alloc.3 \
	man3/rle_row_alloc.3

#ifdef FILE_NAME_14_CHARS
#@PAGE3_INSTALL_NAME = \`basename \$$i .3 | sed 's/^\(............\).*/\1/'\`.3
#else
PAGE3_INSTALL_NAME = \`basename \$$i\`
GET_SETUP_OK_PAGE3 = man3/rle_get_setup_ok.3
#endif

PAGES5 = ${MANPAGES5} ${SOPAGES5}

MANPAGES5 = rle.5

SOPAGES5 = RLE.5

# Default action is to print all man pages
all:
	$(ROFF) $(ROFFOPT) `ls $(PAGES1) | sort` $(ROFFPIPE)
	$(ROFF) $(ROFFOPT) `ls $(MANPAGES3) | sort` $(ROFFPIPE)
	$(ROFF) $(ROFFOPT) ${MANPAGES5} $(ROFFPIPE)

# Put the manual pages into a global location
install: install-1 install-3 install-5
	touch install

install-1: $(PAGES1)
	@-sh -c "if test '$(DEST)X' != X ; then \
		for i in $? ; do \
			dpgm=\`basename \$$i .1\`.$(SUFF1) ; \
			echo cp \$$i $(DEST1)/\$$dpgm ; \
			cp \$$i $(DEST1)/\$$dpgm ; \
		done ; \
	else \
		true ; \
	fi"
	touch install-1

install-3: $(PAGES3)
	@-sh -c "if test '$(DEST)X' != X ; then \
		for i in $? ; do \
			dpgm=${PAGE3_INSTALL_NAME} ; \
			echo cp \$$i $(DEST3)/\$$dpgm ; \
			cp \$$i $(DEST3)/\$$dpgm ; \
		done ; \
	else \
		true ; \
	fi"
	touch install-3

install-5: man5/*.5
	@-sh -c "if test $(DEST)X != X; then \
		echo cp $? $(DEST5); \
		cp $? $(DEST5); \
	else true; \
	fi"
	touch install-5

# Clean up installed man pages
pristine:
	-rm -f install*
	@-if test $(DEST)X != X ; then \
	    for i in man1/*.1 ; do \
		echo "rm -f $(DEST1)/`basename $$i .1`.$(SUFF1)" ;\
		rm -f $(DEST1)/`basename $$i .1`.$(SUFF1) ;\
	    done ; \
	    for i in $(PAGES3) ; do \
		eval "echo rm -f $(DEST3)/${PAGE3_INSTALL_NAME}" ; \
		eval "rm -f $(DEST3)/${PAGE3_INSTALL_NAME}" ; \
	    done ; \
	    for i in man5/*.5 ; do \
		echo rm -f $(DEST5)/$$i ; \
		rm -f $(DEST5)/$$i ; \
	    done ; \
	else \
	    true ;\
	fi

# No other actions
clean:


################ Beginning of makefile.tlr ################
# Stuff that goes at the end of all the makefiles, but is not
# configuration parameters should be in this file.  It is included
# after the makefile.src.

subdirs:
	@sh -c "if test 'x$(DIRS)' != x ; then eval \
		'set -e ; for dir in $(DIRS) ; do \
			(cd \$$dir ; echo Make ${HERE}\$$dir ; \
			 	make $(MFLAGS) $(DIRMFLAGS) ) ; \
		done' ; \
	  else \
		true ; \
	  fi"

# Install all programs
install-pgm: $(PGMS)
	@sh -c "if test 'x$?' != x ; then eval \
		'for pgm in $? ; do \
			dpgm=\`basename \$$pgm .out\` ; \
			echo cp \$$pgm $(DEST)/\$$dpgm ; \
			cp \$$pgm $(DEST)/\$$dpgm; \
		done' ; \
	else \
	    true ; \
	fi"
	touch install-pgm

# Install subdirectories
install-subdirs: subdirs
	@sh -c "if test 'x$(DIRS)' != x  ; then eval \
		'for dir in $(DIRS) ; do \
			(cd \$$dir ; echo Install ${HERE}\$$dir ; \
			 make $(MFLAGS) $(DIRMFLAGS) install) ; \
		done' ; \
	  else \
		true ; \
	  fi"

#config reads the configuration file and munches the apropriate make files
config: doit
	${DEPTH}Configure "${CONFIG}" "${DEPTH}" "${HERE}" "${MFLAGS}"

# post-config is for any actions a directory may require following
# configuration or reconfiguration.
post-config: doit

config-subdirs: doit
	@-if test 'x$(ALLDIRS)' != x ; then eval \
	    'for d in $(ALLDIRS) ; do \
		    ( cd $$d ; \
		      echo Configuring ${HERE}$$d ; \
		      ${DEPTH}Configure "${CONFIG}" "${DEPTH}" "${HERE}$$d/" \
				    "${MFLAGS}" ) \
	    done' ; \
	else \
	    true ; \
	fi

doit:


# Clean up installed stuff and binaries
# pristine-pgm target invoked pristine target in dirs that make programs
# pristine target in library directory is different.
# 
pristine-pgm: clean-pgm
	@-if test 'x$(PGMS)' != x ; then eval \
		'for pgm in $(PGMS); do \
			dpgm=`basename $$pgm .out`; \
			echo rm -f $(DEST)/$$dpgm; \
			rm -f $(DEST)/$$dpgm; \
		done' ; \
	else \
	    true ; \
	fi
	@-if test 'x$(ALLDIRS)' != x ; then eval \
	    'for dir in $(ALLDIRS); do \
		if test -d $$dir ; then \
			(cd $$dir; echo Make ${HERE}$$dir pristine ; \
			 make $(MFLAGS) pristine); \
		else \
			true; \
		fi; \
	    done' ; \
	else \
	    true ; \
	fi
	-rm -f install install-pgm

# Clean up binaries on program directories.
clean-pgm:
	-rm -f $(PGMS) install*
	-rm -f *.o *.out *.new
	-rm -f $(OTHERJUNK)
	@-if test 'x$(ALLDIRS)' != x ; then eval \
	    'for dir in $(ALLDIRS); do \
		if test -d $$dir ; then \
			(cd $$dir; echo Clean ${HERE}$$dir ; \
			 make $(MFLAGS) clean); \
		else \
			true; \
		fi; \
	    done' ; \
	else \
	    true ; \
	fi

#ifndef NO_MAKE_MAKEFILE
# Copy config action to here so that GNU make program will work.
#@makefile:: makefile.src
#@	${DEPTH}Configure "${CONFIG}" "${DEPTH}" "${HERE}" "${MFLAGS}"
#endif

# Keep people who like to 'make depend' happy.  Allow depend::
# actions in specific makefiles if desired.
depend::

################ End of makefile.tlr ################
