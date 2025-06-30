main.o: main.c utils.h lois_probabilite.h file.h simulations.h
	gcc -c main.c -lm

utils.o: utils.c utils.h
	gcc -c utils.c

lois_probabilite.o: lois_probabilite.c lois_probabilite.h
	gcc -c lois_probabilite.c -lm

file.o: file.c file.h
	gcc -c file.c

simulations.o: simulations.c simulations.h utils.h file.h lois_probabilite.h
	gcc -c simulations.c -lm

main: main.o utils.o lois_probabilite.o file.o simulations.o
	gcc main.o utils.o lois_probabilite.o file.o simulations.o -o main -lm
