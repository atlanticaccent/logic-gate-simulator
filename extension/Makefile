all: extension

clean:
		rm *.o *.out circuitsExtended

extension : streamParserExtended.o GateExtended.o CircuitExtended.o mainExtended.o
		gcc streamParserExtended.o GateExtended.o CircuitExtended.o ../List.o mainExtended.o -lm -o circuitsExtended

streamParserExtended.o : streamParserExtended.c headerExtended.h
		gcc -c streamParserExtended.c

GateExtended.o : GateExtended.c headerExtended.h
		gcc -c GateExtended.c

CircuitExtended.o : CircuitExtended.c headerExtended.h
		gcc -c CircuitExtended.c -lm

mainExtended.o : mainExtended.c headerExtended.h
		gcc -c mainExtended.c -lm
