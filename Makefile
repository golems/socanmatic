cc := gcc
CC := g++

CFLAGS := -g

PREFIX := /usr/local


.PHONY: doc clean distclean install default

default: esdcantool


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
