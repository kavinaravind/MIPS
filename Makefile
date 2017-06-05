# Specify shell to execute recipes
SHELL=/bin/bash

CC=gcc
CFLAGS=-O0 -std=c99 -m64 -Wall -W -ggdb3 -lm

disassembler: Disassembler.o ParseResult.o MIParser.o Variables.o Labels.o InstructionsParser.o
		$(CC) $(CFLAGS) -o disassem Disassembler.o ParseResult.o MIParser.o Variables.o Labels.o InstructionsParser.o

Disassembler.o: Disassembler.c ParseResult.o MIParser.o Variables.o Labels.o
		$(CC) $(CFLAGS) -c Disassembler.c
				
MIParser.o: MIParser.c MIParser.h InstructionsParser.o
		$(CC) $(CFLAGS) -c MIParser.c InstructionsParser.o

InstructionsParser.o: InstructionsParser.c InstructionsParser.h ParseResult.o refSystemConstants.h
		$(CC) $(CFLAGS) -c InstructionsParser.c ParseResult.o

ParseResult.o: ParseResult.c ParseResult.h
		$(CC) $(CFLAGS) -c ParseResult.c

Variables.o: Variables.c Variables.h
		$(CC) $(CFLAGS) -c Variables.c
		
Labels.o: Labels.c Labels.h
		$(CC) $(CFLAGS) -c Labels.c		
		
clean:
	rm *.o
