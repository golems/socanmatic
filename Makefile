
CC=gcc

all: libamccan.so

.c.o:
	$(CC) -c $<

libamccan.so: amccan.o amcdrive.o ntcanopen.o
	$(CC) -shared -Wl,-soname,$@ -o $@ amccan.o amcdrive.o ntcanopen.o

clean:
	rm -rf *.so *.o *~

