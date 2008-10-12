cc := gcc
CC := g++

CFLAGS := -g

.PHONY: doc clean distclean

esdcantool: esdcantool.o
	$(cc) -o esdcantool esdcantool.o -lntcan

esdcantool.o: esdcantool.c
	$(cc) -c $(CFLAGS) -o $@ $<

doc:
	doxygen

clean:
	rm -vf *.o esdcantool

distclean: clean
	rm -rf doc
