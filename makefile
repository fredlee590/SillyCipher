GCC = gcc -Wall -g

.PHONY: all
all: sillyCipher

sillyCipher : main.o sillyCipherFunc.o
	$(GCC) -o sillyCipher main.o sillyCipherFunc.o -lssl -lcrypto
main.o : main.c sillyCipherFunc.h
	$(GCC) -c main.c
sillyCipherFunc.o : sillyCipherFunc.c sillyCipherFunc.h
	$(GCC) -c sillyCipherFunc.c
.PHONY: clean
clean:
	rm -f *.o
	rm -f sillyCipher

.PHONY: install
install: sillyCipher
	mv sillyCipher /usr/local/bin/
.PHONY: uninstall
uninstall:
	rm /usr/local/bin/sillyCipher

debug: GCC += -DDEBUG
debug: sillyCipher
