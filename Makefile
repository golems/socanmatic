## Makefile for js

VERSION := 0.0.1
PROJECT := amcdrive

SHAREDLIBS := amcdrive
BINFILES := amc-test-dual-js

default: all

include /usr/share/make-common/common.1.mk

all: $(LIBFILES) $(BINFILES)

# apparently ach requires this, or at least c99
CFLAGS += --std=gnu99

$(call LINKLIB, amcdrive, amccan.o amcdrive.o)
$(call LINKBIN, amc-test-dual-js, amc-test-dual-js.o, amcdrive ntcan ntcanopen pthread)

clean: 
	rm -vf *.o *.so amc-test-dual-js *.deb 
	rm -rf .deps debian doc $(PROJECT)-$(VERSION)

doc:
	doxygen
