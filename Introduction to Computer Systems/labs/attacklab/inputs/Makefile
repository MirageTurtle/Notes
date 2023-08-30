PROJECT += input1.bin input2.bin input3.bin input4.bin
PROJECT += shellcode.o shellcode.d

CC = gcc
CFLAGS = -O1

.PHONY: all
all: $(PROJECT)

%.bin: %.txt
	./hex2raw < $< > $@

shellcode.o: shellcode.s

shellcode.d: shellcode.o
	objdump -d $< > $@

.PHONY: clean
clean:
	rm -rf $(PROJECT)
