## Makefile for esdcan

## Edit thise variables to configure installation path
# root to install to for `make install'
PREFIX := /usr/local
# if you use stow, root of your stow package directory
STOWBASE := /usr/local/stow



## Don't worry about the rest

VERSION := 0.01
PROJECT := esdcan

STOWDIR := $(PROJECT)-$(VERSION)
STOWPREFIX := $(STOWBASE)/$(STOWDIR)

DISTPATH := $(HOME)/prism/tarballs
DOXPATH := $(HOME)/prism/public_html/dox


cc := gcc
CC := g++

CFLAGS := -g -fPIC


.PHONY: doc clean distclean install default linkinstall_lib uninstall_lib

default: esdcantool libntcanopen.so


esdcantool: esdcantool.o ntcanopen.o
	$(cc) -o esdcantool esdcantool.o ntcanopen.o -lntcan

esdcantool.o: esdcantool.c ntcanopen.h
	$(cc) -c $(CFLAGS) -o $@ $<

ntcanopen.o: ntcanopen.c ntcanopen.h
	$(cc) -c $(CFLAGS) -o $@ $<

libntcanopen.so: ntcanopen.o
	gcc -shared -Wl,-soname,$@ -o $@ $<

clean:
	rm -vf *.o *.so esdcantool

distclean: clean
	rm -rf doc


install: libntcanopen.so
	mkdir -p $(PREFIX)/include
	mkdir -p $(PREFIX)/bin
	mkdir -p $(PREFIX)/lib
	install --mode=755 libntcanopen.so $(PREFIX)/lib
	install --mode=644 ntcanopen.h $(PREFIX)/include
	install --mode=755 esdcantool $(PREFIX)/bin

bininstall: esdcantool
	install --mode=755 esdcantool $(PREFIX)/bin

uninstall_lib:
	rm -f $(PREFIX)/lib/libntcanopen.so
	rm -f $(PREFIX)/include/ntcanopen.h


linkinstall_lib: libntcanopen.so esdcantool
	ln -s $(PWD)/libntcanopen.so $(PREFIX)/lib
	ln -s $(PWD)/ntcanopen.h $(PREFIX)/include

stow:
	mkdir -p $(STOWPREFIX)/include
	mkdir -p $(STOWPREFIX)/bin
	mkdir -p $(STOWPREFIX)/lib
	install --mode=755 libntcanopen.so $(STOWPREFIX)/lib
	install --mode=644 ntcanopen.h $(STOWPREFIX)/include
	install --mode=755 esdcantool $(STOWPREFIX)/bin
	cd $(STOWBASE) && stow $(STOWDIR)

dist: distclean
	cd .. &&               \
	tar --exclude=.svn --lzma -cvf $(DISTPATH)/$(PROJECT)-$(VERSION).tar.lzma $(PROJECT)

doc:
	doxygen

docul: doc
	cp -Tr doc/html $(DOXPATH)/$(PROJECT)
