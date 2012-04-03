linker: main.o tokenizer.o utility.o
	gcc -o linker main.o tokenizer.o utility.o

main.o: main.c main.h
	gcc -std=c99 -c main.c

tokenizer.o: tokenizer.c tokenizer.h
	gcc -std=c99 -c tokenizer.c

utility.o: utility.c utility.h
	gcc -std=c99 -c utility.c

clean:
	rm linker main.o tokenizer.o utility.o
