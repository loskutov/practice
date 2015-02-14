CFLAGS=-m32 -g

all: sum


sum: sum.o
	gcc $(CFLAGS) -o $@ $^

%.o: %.asm
	yasm -f elf32 -g dwarf2 -o $@ $^
