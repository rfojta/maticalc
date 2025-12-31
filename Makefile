default: maca_matice.o maca_matice.h
	gcc -g -o maticalc maticalc.c maca_matice.o

maca_matice.o: maca_matice.c maca_matice.h
	gcc -g -c -o maca_matice.o maca_matice.c
