CC = clang
CFLAGS = -Wall -Wpedantic -Werror -Wextra
LFLAGS = -lm

all: tsp

tsp: tsp.o graph.o path.o stack.o
	$(CC) $(LFLAGS) -o tsp tsp.o graph.o path.o stack.o 

tsp.o: tsp.c graph.h path.h stack.h vertices.h
	$(CC) $(CFLAGS) -c tsp.c

clean:
	rm -f tsp *.o

format:
	clang-format -i -style=file *.[ch]

debug: CFLAGS += -g
debug: LFLAGS += -g
debug: clean all
