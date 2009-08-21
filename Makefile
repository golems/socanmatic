## Makefile for esdcan

VERSION := 20090820
PROJECT := esdcan

SHAREDLIBS := ntcanopen
BINFILES := esdcantool


# make-cmmon should handle this somehow...
CFLAGS := -g -fPIC

default: all

include /usr/share/make-common/common.1.mk

all: $(LIBFILES) $(BINFILES)

$(call LINKBIN, esdcantool, esdcantool.o ntcanopen.o, ntcan)
$(call LINKLIB, ntcanopen, ntcanopen.o)


clean:
	rm -vf *.o *.so esdcantool
doc:
	doxygen
