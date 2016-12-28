moveso :
	cp libudis86.so /usr/local/lib/libudis86.so
	cp udis86.h /usr/local/include/udis86.h

ppptra : readelf.o disas.o ppptra.o
	cc -o ppptra readelf.o disas.o ppptra.o -std=c99

readelf.o : readelf.c disas.h
	cc -o readelf.o readelf.c

disas.o : disas.c readelf.
	cc -o disas.o disas.c -ludis86

ppptra.o : ppptra.c disas.
	cc -o ppptra.o ppptra.c

clean :
	rm readelf.o disas.o ppptra.o
