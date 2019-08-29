OBJS	= lsh.o structures.o
OUT	= lsh
CC		= g++
FLAGS	= -c -w -Wall

all: t1

structures.o: structures.cpp structures.h
	$(CC) $(FLAGS) structures.cpp
lsh.o: lsh.cpp structures.h
	$(CC) $(FLAGS) lsh.cpp
t1: $(OBJS)
	$(CC) $(OBJS) -o $(OUT)

# clean up
clean:
	rm -f $(OBJS) $(OUT)
