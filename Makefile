CFLAGS=-g -Wall -pedantic

myio: myio.c
	gcc $(CFLAGS) -o myio myio.c

.PHONY: clean
clean:
	rm -f myls
