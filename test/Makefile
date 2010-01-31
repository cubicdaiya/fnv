
VPATH = ../

CC = gcc
CFLAGS = -Wall -g -std=c99
LFLAGS = -lcunit
OBJS = fnv_test.o fnv.o
TARGETS = fnv_test

.c.o:
	$(CC) $(CFLAGS) -c $<

all : $(TARGETS)

fnv_test : $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LFLAGS)

check :
	@./fnv_test

clean :
	rm $(OBJS) $(TARGETS)

fnv_test.o: fnv_test.c ../fnv.h
fnv.o: ../fnv.c ../fnv.h
