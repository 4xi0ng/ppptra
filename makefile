obj = main.c readelf.c ppptra.c disas.c

ppptra : $(obj) libudis86.so
	make moveso
	gcc -o ppptra $(obj) -ludis86 -std=c99 -w

moveso :
	cp -f libudis86.so /usr/local/lib/libudis86.so
	cp -f udis86.h /usr/local/include/udis86.h
	cp -rf libudis86 /usr/local/include/libudis86
	ldconfig
