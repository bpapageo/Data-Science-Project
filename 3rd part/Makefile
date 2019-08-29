OBJS	= cluster.o structures.o lsh.o recommendation.o
OUT	= recommendation
CC		= g++
FLAGS	= -c -w -Wall

all: t1

lsh.o: lsh.cpp lsh.h structures.h
	$(CC) $(FLAGS) lsh.cpp
cluster.o: cluster.cpp cluster.h lsh.h structures.h
	$(CC) $(FLAGS) cluster.cpp
structures.o: structures.cpp structures.h cluster.h lsh.h
	$(CC) $(FLAGS) structures.cpp
recommendation.o: recommendation.cpp cluster.h lsh.h
	$(CC) $(FLAGS) recommendation.cpp
t1: $(OBJS)
	$(CC) $(OBJS) -o $(OUT)

# clean up
clean:
	rm -f $(OBJS) $(OUT)
