OBJS	= cube.o structures.o
OUT	= cube
CC		= g++
FLAGS	= -c -Wall

all: t1

structures.o: structures.cpp structures.h
	$(CC) $(FLAGS) structures.cpp
cube.o: cube.cpp structures.h
	$(CC) $(FLAGS) cube.cpp
t1: $(OBJS)
	$(CC) $(OBJS) -o $(OUT)

# clean up
clean:
	rm -f $(OBJS) $(OUT)
