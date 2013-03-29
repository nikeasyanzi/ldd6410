all: symbol ref-s ref-m
symbol: var.o main.o
	gcc -o symbol var.o main.o
ref-s: single-multithrad.o
	gcc -o ref-s single-multithrad.o
ref-m: single-multithrad.o
	gcc -o ref-m  single-multithrad.o -lpthread
clean:
	rm *.o symbol ref*

