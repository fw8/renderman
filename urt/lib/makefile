#%#### makefile AUTOMATICALLY GENERATED ######
#%############### DO NOT EDIT ################
#%### EDIT makefile.src AND make config ######
DEPTH = ../
CONFIG = ../config/urt
HERE = lib/
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

# Description file for librle library.

#ifdef LIB_DEST
#set DEST LIB_DEST
DEST = /mo2/ip/lib
#endif

#ifndef NO_RANLIB
RANLIB = ranlib
#endif

# If nothing else is specified, default is to build the library.
default: buildlib

#ifadd	NEED_BSTRING	OPT_OBJS	bstring.o
#ifadd	NEED_SETLINEBUF	OPT_OBJS	sVsetlinebuf.o
#ifadd	NEED_GETOPT	OPT_OBJS	getopt.o
#emit OPT_OBJS
OPT_OBJS = 

OBJS = Runput.o buildmap.o cmd_name.o colorquant.o dither.o \
	float_to_exp.o inv_cmap.o rle_addhist.o rle_cp.o rle_getcom.o \
	rle_getraw.o rle_getrow.o rle_getskip.o rle_global.o \
	rle_open_f.o rle_putcom.o rle_putraw.o rle_putrow.o \
	rle_raw_alc.o rle_rawrow.o rle_row_alc.o scanargs.o \
	vaxshort.o \
	$(OPT_OBJS)

LIBNAME = librle.a

# Just rebuild the object files.
objs: $(OBJS)

# Copy the library to the destination directory.
# (Install a second copy on the debug subdirectory, on the theory that it
#  is more useful to have a non-debug version than nothing at all.)
# Do nothing if $(DEST) is empty (or not defined)
install: buildlib
	@sh -c "if test '$(DEST)x' != x ; then \
		echo cp $(LIBNAME) $(DEST)/$(LIBNAME) ; \
		cp $(LIBNAME) $(DEST)/$(LIBNAME) ; \
		if test x$(RANLIB) != x ; then \
			echo ranlib $(DEST)/$(LIBNAME) ; \
			ranlib $(DEST)/$(LIBNAME) ; \
		else \
			true ; \
		fi ; \
		if test -d $(DEST)/debug ; then \
			echo cp $(LIBNAME) $(DEST)/debug/$(LIBNAME) ; \
			if test x$(RANLIB) != x ; then \
				cp $(LIBNAME) $(DEST)/debug/$(LIBNAME) ; \
				echo ranlib $(DEST)/debug/$(LIBNAME) ; \
			else \
				true ; \
			fi ; \
			ranlib $(DEST)/debug/$(LIBNAME) ; \
		else \
			true ; \
		fi ; \
	else \
		true ; \
	fi"
	touch install

# Rebuild the library from all the .o files.
buildlib: $(OBJS)
	-rm -f $(LIBNAME)
	ar rc $(LIBNAME)
	ar q $(LIBNAME) $(OBJS)
#ifndef NO_RANLIB
	ranlib $(LIBNAME)
#endif
	touch buildlib

# Clean up installed stuff and binaries
pristine: clean
	-@if test $(DEST)X != X ; then \
		echo rm -f $(DEST)/$(LIBNAME) $(DEST)/debug/$(LIBNAME) ; \
		rm -f $(DEST)/$(LIBNAME) $(DEST)/debug/$(LIBNAME) ; \
	else \
		echo rm -f $(LIBNAME) ; \
		rm -f $(LIBNAME) ; \
	fi
	-rm -f install

# Get rid of everything which must be recompiled on a different computer.
clean:
	-rm -f *.o
	-@if test x$(DEST) != x ; then \
		echo rm -f $(LIBNAME) ; \
		rm -f $(LIBNAME) ; \
	else \
		true ; \
	fi
	-rm -f *.ln
	-rm -f buildlib lintlib

# Lint actions:
#
# lintall - run lint on all the source files
# lint1 - run line on one source file
# llib-lrle - build human-readable form of lint library
# llib-lrle.ln - build machine readable form of lint library for
# use in linting programs that use the library
# linstall - install the lint library
#
lintall:
	lint $(DFLAGS) $(IFLAGS) $(CFILES)

lint1:
	lint $(DFLAGS) $(IFLAGS) -u $(LINTF) llib-lrle.ln

llib-lrle: $(CFILES)
	-gemacs -tnull -q -l${AEM}/mk-llib.ml -emk-llib llib-lrle ${CFILES}

lintlib llib-lrle.ln: llib-lrle
	/lib/cpp $(IFLAGS) -C -Dlint llib-lrle | > /usr/lib/lint/lint1 -uv > \
		llib-lrle.ln
	touch lintlib

# Install the lint library.
linstall: llib-lrle.ln
	cp llib-lrle.ln llib-lrle /usr/lib/lint

# Make all objects depend on rle_config.h, even if they really don't.
# This forces a recompile whenever a reconfiguration happens.
$(OBJS):${RI}/rle_config.h

# Dependencies on .h files:
# DO NOT DELETE THIS LINE

Runput.o: ${RI}/rle.h ${RI}/rle_code.h ${RI}/rle_put.h
buildmap.o: ${RI}/rle.h
float_to_exp.o: ${RI}/rle.h
rle_addhist.o: ${RI}/rle.h
rle_cp.o: ${RI}/rle.h ${RI}/rle_code.h ${RI}/rle_put.h
rle_getcom.o: ${RI}/rle.h
rle_getraw.o: ${RI}/rle.h ${RI}/rle_raw.h
rle_getrow.o: ${RI}/rle.h ${RI}/rle_code.h
rle_getskip.o: ${RI}/rle.h ${RI}/rle_code.h
rle_global.o: ${RI}/rle.h ${RI}/rle_put.h
rle_putcom.o: ${RI}/rle.h
rle_putraw.o: ${RI}/rle.h ${RI}/rle_put.h ${RI}/rle_raw.h
rle_putrow.o: ${RI}/rle.h ${RI}/rle_put.h
rle_raw_alc.o: ${RI}/rle.h ${RI}/rle_raw.h
rle_rawrow.o: ${RI}/rle.h ${RI}/rle_raw.h
rle_row_alc.o: ${RI}/rle.h

config-subdirs:

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
