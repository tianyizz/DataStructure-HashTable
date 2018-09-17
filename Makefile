CC = gcc
CFLAGS = -std=c99 -Wall -Wextra -pedantic -g

assign4: hash.o list.o mainFunction.o main.o

hash.o: hash.h list.h

list.o: list.h

mainFunction.o: hash.h mainFunction.h

.PHONY: clean
clean:
	rm -f assign2_test hack_object.o list.o hack_inventory.o
