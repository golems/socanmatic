# Project Name
PROJECT := amcdrive

# Project Version 
VERSION := 20100404

# Binary Files
BINFILES := amcdrived

# Library files
SHAREDLIBS := amcdrive 


LC_ALL := ascii
LANG := ascii

all: default

CC=g++
include /usr/share/make-common/common.1.mk
CFLAGS += -Wall -Wextra -Wpointer-arith --std=gnu99

default: $(LIBFILES) $(BINFILES)

## BUILDING LIBRARIES: call with  $(call LINKLIB, name_of_lib, list of object files)
$(call LINKLIB, amcdrive, amccan.o amcdrive.o)

## BUILDING BINARIES: call with $(call LINKBIN, name_of_binary, object files, shared libs, static libs)
$(call LINKBIN, amcdrived, amcdrived.o amcdrive.o amccan.o, ntcan ntcanopen somatic ach protobuf-c stdc++ rt blas lapack)

.PHONY: default clean

clean:
	rm -fr *.o $(BINFILES) $(LIBFILES) debian *.deb *.lzma

depclean: clean
	rm -fr .deps
