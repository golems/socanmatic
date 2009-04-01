
CC=gcc
CFLAGS=-g -DDEBUG

all: libamccan.so

amc-test: amc-test.c
	$(CC) $(CFLAGS) -o amc-test $< -lamccan -lntcan

.c.o:
	$(CC) $(CFLAGS) -c $<

libamccan.so: amccan.o amcdrive.o ntcanopen.o
	$(CC) $(CFLAGS) -shared -Wl,-soname,$@ -o $@ amccan.o amcdrive.o ntcanopen.o

install: libamccan.so
	cp amccan.h /usr/local/include
	cp amcdrive.h /usr/local/include
	cp libamccan.so /usr/local/lib
	ldconfig

clean:
	rm -rf *.so *.o *~

