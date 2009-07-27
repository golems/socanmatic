
PREFIX=/usr/local

CC=gcc
CFLAGS=-g -fPIC --std=gnu99 

all: libamccan.so motord

amc-test: amc-test.c
	$(CC) $(CFLAGS) -o amc-test $< -lamccan -lntcan -lntcanopen

amc-test-vring: amc-test-vring.c
	$(CC) $(CFLAGS) -o amc-test-vring $< -lamccan -lntcan -lntcanopen

amc-test-dual: amc-test-dual.c
	$(CC) $(CFLAGS) -o amc-test-dual $< -lamccan -lntcan -lntcanopen

amc-test-dual-js: amc-test-dual-js.c
	$(CC) $(CFLAGS) -o amc-test-dual-js $< -lamccan -lntcan -lntcanopen -lpthread

amc-test-dual-current: amc-test-dual-current.c
	$(CC) $(CFLAGS) -o amc-test-dual-current $< -lamccan -lntcan -lntcanopen

.c.o:
	$(CC) $(CFLAGS) -c $<

libamccan.so: amccan.o amcdrive.o
	$(CC) $(CFLAGS) -shared -Wl,-soname,$@ -o $@ $^

motord: motord.c libamccan.so
	$(CC) $(CFLAGS) -I. -o $@ $< -lamccan -lntcan -lntcanopen -lach -lpthread -lrt -L.

install: libamccan.so
	install --mode 644 amccan.h $(PREFIX)/include
	install --mode 644 byteorder.h $(PREFIX)/include
	install --mode 644 amcdrive.h $(PREFIX)/include
	install --mode 755 libamccan.so $(PREFIX)/lib
	install --mode 755 motord $(PREFIX)/bin
	ldconfig

clean:
	rm -rf *.so *.o *~

