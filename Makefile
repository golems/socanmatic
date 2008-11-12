cc := gcc
CC := g++

CFLAGS := -g

PREFIX := /usr/local


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

doc:
	doxygen

clean:
	rm -vf *.o *.so esdcantool

distclean: clean
	rm -rf doc


install: libntcanopen.so
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
