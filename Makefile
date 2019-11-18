CFLAGS=-g -Wall -pedantic

PROGS=myio testCode

myInputOutput: myio.o testCode.o
		gcc -o myInputOutput myio.o testCode.o

myInputOutput2: myio.o testCode2.o
	  gcc -o myInputOutput2 myio.o testCode2.o

myInputOutput3: myio.o testCode3.o
			gcc -o myInputOutput3 myio.o testCode3.o

myio.o: myio.c
		gcc $(CFLAGS) -c -o myio.o myio.c

testCode.o: testCode.c
		gcc $(CFLAGS) -c -o testCode.o testCode.c

testCode2.o: testCode2.c
		gcc $(CFLAGS) -c -o testCode2.o testCode2.c

testCode3.o: testCode3.c
		gcc $(CFLAGS) -c -o testCode3.o testCode3.c

.PHONY: clean
clean:
	rm -f *.o $(PROGS)
