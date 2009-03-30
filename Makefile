
CC=gcc

all: libamccan.so

.c.o:
	$(CC) -c $<

libamccan.so: amccan.o ntcanopen.o
	$(CC) -shared -Wl,-soname,$@ -o $@ amccan.o ntcanopen.o

clean:
	rm -rf *.so *.o *~

