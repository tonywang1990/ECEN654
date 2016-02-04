CC = g++
CFLAGS = -g -Wall

all: main.o graph.o partition.o
		$(CC)  main.o graph.o partition.o -o klp
main.o: main.cpp
		$(CC) $(CFLAGS) -c main.cpp
graph.o: graph.cpp graph.h
		$(CC) $(CFLAGS) -c graph.cpp
partition.o: partition.cpp partition.h
		$(CC) $(CFLAGS) -c partition.cpp
clean: 
		rm *.o klp

