GCC = gcc
LFLAGS = -Wall -o
TARGET = ahmes-asm
DEBUG = --debug

all:
	$(GCC) $(LFLAGS) $(TARGET).out $(TARGET).c
debug:
	$(GCC) $(DEBUG) $(LFLAGS) $(TARGET).out $(TARGET).c
clean: 
	rm -v *.out
run:
	./$(TARGET).out
