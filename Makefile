
CC=gcc
CFLAGS=-g

all: libamccan.so

.c.o:
	$(CC) $(CFLAGS) -c $<

libamccan.so: amccan.o amcdrive.o ntcanopen.o
	$(CC) $(CFLAGS) -shared -Wl,-soname,$@ -o $@ amccan.o amcdrive.o ntcanopen.o

clean:
	rm -rf *.so *.o *~

