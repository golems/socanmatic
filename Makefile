## Makefile for esdcan

VERSION := 20090715
PROJECT := esdcan

LIBFILES := libntcanopen.so
BINFILES := esdcantool

default: $(LIBFILES) $(BINFILES)

# make-cmmon should handle this somehow...
CFLAGS := -g -fPIC

include /usr/share/make-common/common.1.mk

$(call LINKBIN, esdcantool, esdcantool.o ntcanopen.o, ntcan)
$(call LINKLIB, ntcanopen, ntcanopen.o)


clean:
	rm -vf *.o *.so esdcantool
doc:
	doxygen
