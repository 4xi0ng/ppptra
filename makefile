obj = main.c readelf.c ppptra.c disas.c
lib = udis86

moveso :
	cp -f libudis86.so /usr/local/lib/libudis86.so
	cp -f udis86.h /usr/local/include/udis86.h
	cp -rf libudis86 /usr/local/include/libudis86
	ldconfig

ppptra :
	gcc -o ppptra main.c readelf.c ppptra.c disas.c -ludis86 -std=c99 -w
