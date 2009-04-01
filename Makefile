
PREFIX=/usr/local

CC=gcc
CFLAGS=-g -DDEBUG

all: libamccan.so

amc-test: amc-test.c
	$(CC) $(CFLAGS) -o amc-test $< -lamccan -lntcan -lntcanopen

.c.o:
	$(CC) $(CFLAGS) -c $<

libamccan.so: amccan.o amcdrive.o
	$(CC) $(CFLAGS) -shared -Wl,-soname,$@ -o $@ $^

install: libamccan.so
	cp amccan.h $(PREFIX)/include
	cp amcdrive.h $(PREFIX)/include
	cp libamccan.so $(PREFIX)/lib
	ldconfig

clean:
	rm -rf *.so *.o *~

