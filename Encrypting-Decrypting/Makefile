CC = clang
CFLAGS = -Wall -Wpedantic -Werror -Wextra -g $(shell pkg-config --cflags gmp)
LFLAGS = -lm $(shell pkg-config --libs gmp)

all: encrypt decrypt keygen

encrypt: encrypt.o rsa.o numtheory.o randstate.o
	$(CC) $(LFLAGS) -o encrypt encrypt.o rsa.o numtheory.o randstate.o

encrypt.o: encrypt.c rsa.h numtheory.h randstate.h
	$(CC) $(CFLAGS) -c encrypt.c

decrypt: decrypt.o rsa.o numtheory.o randstate.o
	$(CC) $(LFLAGS) -o decrypt decrypt.o rsa.o numtheory.o randstate.o

decrypt.o: decrypt.c rsa.h numtheory.h randstate.h
	$(CC) $(CFLAGS) -c decrypt.c

keygen: keygen.o rsa.o numtheory.o randstate.o
	$(CC) $(LFLAGS) -o keygen keygen.o rsa.o numtheory.o randstate.o

keygen.o: keygen.c rsa.h numtheory.h randstate.h
	$(CC) $(CFLAGS) -c keygen.c

clean:
	rm -f encrypt decrypt keygen *.o

format:
	clang-format -i -style=file *.[ch]
