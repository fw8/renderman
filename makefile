#
# Top level makefile for RenderMan and ShadingCompiler
#

# Subdirectories that get made
DIRS = src sc

# SYS V Make reads this from environment, so override it
SHELL = /bin/sh

top: all


# clean deletes all but source, pristine (below) deletes installed stuff, too
all clean: doit
	for d in $(DIRS) ; do ( cd $$d ; make $(MFLAGS) $@ ) ; done

# install puts library, binaries and documentation into global location
# (see individual makefiles for configuration variables)

install makefile pristine depend: doit
	for d in $(DIRS) man ; do ( cd $$d ; make $(MFLAGS) $@ ) ; done

doit:
