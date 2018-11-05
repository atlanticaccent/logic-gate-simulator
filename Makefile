all: circuits
		@echo "Other make options include verbose and debug"

clean:
		rm *.o *.out circuits circuitsDebug

circuits : streamParser.o Gate.o Circuit.o List.o main.o header.h
		gcc streamParser.o Gate.o Circuit.o List.o main.o -lm -o circuits

verbose : streamParser.o Gate.o Circuit.o List.o main.o header.h
		gcc streamParser.o Gate.o Circuit.o List.o main.o -Wall -Wextra -lm -o circuits

debug : streamParser.c Gate.c Circuit.c List.c main.c header.h
		@echo "Debug executable created as \"circuitsDebug\""
		gcc streamParser.c Gate.c Circuit.c List.c main.c -Wall -Wextra -g -O0 -lm -o circuitsDebug

extension : List.o FORCE
		@echo "Extension executable created as \"circuitsExtended\""
		cd extension && $(MAKE)

streamParser.o : streamParser.c header.h
		gcc -c streamParser.c

Gate.o : Gate.c header.h
		gcc -c Gate.c

Circuit.o : Circuit.c header.h
		gcc -c Circuit.c -lm

List.o : List.c header.h
		gcc -c List.c

main.o : main.c header.h
		gcc -c main.c -lm

FORCE:
