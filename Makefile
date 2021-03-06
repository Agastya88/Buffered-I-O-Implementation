CFLAGS=-g -Wall -pedantic

PROGS=myio testCode

myInputOutput: myio.o testCode.o
		gcc -o myInputOutput myio.o testCode.o

myInputOutput2: myio.o testCode2.o
	  gcc -o myInputOutput2 myio.o testCode2.o

myInputOutput3: myio.o testCode3.o
			gcc -o myInputOutput3 myio.o testCode3.o

myInputOutput4: myio.o testCode4.o
			gcc -o myInputOutput4 myio.o testCode4.o

myInputOutput5: myio.o testCode5.o
			gcc -o myInputOutput5 myio.o testCode5.o

myio.o: myio.c
		gcc $(CFLAGS) -c -o myio.o myio.c

testCode.o: testCode.c
		gcc $(CFLAGS) -c -o testCode.o testCode.c

testCode2.o: testCode2.c
		gcc $(CFLAGS) -c -o testCode2.o testCode2.c

testCode3.o: testCode3.c
		gcc $(CFLAGS) -c -o testCode3.o testCode3.c

testCode4.o: testCode4.c
		gcc $(CFLAGS) -c -o testCode4.o testCode4.c

testCode5.o: testCode5.c
		gcc $(CFLAGS) -c -o testCode5.o testCode5.c

.PHONY: clean
clean:
	rm -f *.o $(PROGS)
