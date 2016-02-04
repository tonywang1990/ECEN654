CC = g++
CFLAGS = -g -Wall

all: main.o sa.o graph.o
		$(CC)  main.o sa.o graph.o -o sap
main.o: main.cpp
		$(CC) $(CFLAGS) -c main.cpp
sa.o: sa.cpp sa.h
		$(CC) $(CFLAGS) -c sa.cpp
graph.o: graph.cpp graph.h
		$(CC) $(CFLAGS) -c graph.cpp
clean: 
		rm *.o sap 

