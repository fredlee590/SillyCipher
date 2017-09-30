GCC = gcc -Wall -g

all: sillyCipher

sillyCipher : main.o sillyCipherFunc.o
	$(GCC) -o sillyCipher main.o sillyCipherFunc.o -lssl -lcrypto
main.o : main.c sillyCipherFunc.h
	$(GCC) -c main.c
sillyCipherFunc.o : sillyCipherFunc.c sillyCipherFunc.h
	$(GCC) -c sillyCipherFunc.c
clean:
	rm -f *.o
	rm -f sillyCipher

debug: GCC += -DDEBUG
debug: sillyCipher
