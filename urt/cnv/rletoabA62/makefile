#%#### makefile AUTOMATICALLY GENERATED ######
#%############### DO NOT EDIT ################
#%### EDIT makefile.src AND make config ######
DEPTH = ../../
CONFIG = ../../config/urt
HERE = cnv/rletoabA62/
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

RI = ../../include
RL = ../../lib
DIRS =
ALLDIRS =
PGMS =

ExtraLIBS = 
LIBS = -L$(RL) -lrle $(DIRLIBS) $(ExtraLIBS)

MachFLAGS =  -f68881
CFLAGS = -O $(DFLAGS) $(DIRFLAGS) $(ExtraCFLAGS) $(IFLAGS) $(MachFLAGS) 
IFLAGS = -I. -I$(RI)

SHELL = /bin/sh

# Makefile for abekas A62 conversion routines.

default		:	all

#set	DEST	CNV_DEST	Where to install it
DEST = /mo2/ip/bin

# Extra libraries for this directory.
DIRLIBS = -lm

PGMS = rletoabA62.out

all		:	$(PGMS)

# Executables.  The .out will be stripped off in the install action.

rletoabA62.out	:	rletoabA62.o rle.o
			$(CC) $(CFLAGS) -o rletoabA62.new \
			  rletoabA62.o rle.o $(LIBS)
			mv rletoabA62.new rletoabA62.out

# Incremental install, copies executable to DEST dir.
install		:	default install-pgm
			touch install

# Clean up installed binaries
pristine	:	pristine-pgm

# Clean up binaries
clean		:	clean-pgm

lint		:
			lint $(RI) rletoabA62.c rle.c sig.c

# Dependencies
rletoabA62.o:	$(RI)/rle_config.h
rle.o:		$(RI)/rle.h $(RI)/rle_config.h
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
