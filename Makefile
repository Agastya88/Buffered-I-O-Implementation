CFLAGS=-g -Wall -pedantic

PROGS=myio testCode

myInputOutput: myio.o testCode.o
		gcc -o myInputOutput myio.o testCode.o

myio.o: myio.c
		gcc $(CFLAGS) -c -o myio.o myio.c

testCode.o: testCode.c
		gcc $(CFLAGS) -c -o testCode.o testCode.c

.PHONY: clean
clean:
	rm -f *.o $(PROGS)
