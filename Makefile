
CC=gcc

all: amccan.so

.c.o:
	$(CC) -c $<

amccan.so: amccan.o ntcanopen.o
	$(CC) -shared -Wl,-soname,$@ -o $@ $<

clean:
	rm -rf *.so *.o *~

