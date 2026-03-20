# Daniel Omoniyi, 24396 - Operating Systems Project 1


simpleshell: src/simpleshell.c src/commands.c src/simpleshell.h
	gcc -Wall src/simpleshell.c src/commands.c -o bin/simpleshell

clean:
	rm -f bin/simpleshell
