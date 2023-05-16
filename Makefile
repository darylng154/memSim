CC = gcc
CFLAGS = -g -Wall -Werror
OBJS = page_table.o tlb.o
HEADER = page_table.h tlb.h

FRAMES = 256
TESTFILE = fifo1.txt

all:  memSim

memSim: memSim.c ${OBJS}
	$(CC) $(CFLAGS) -o memSim memSim.c ${OBJS}

fifo: clean memSim
	./memSim testcases/${TESTFILE} ${FRAMES} FIFO

lru: clean memSim
	../memSim testcases/${TESTFILE} ${FRAMES} LRU

opt: clean memSim
	./memSim testcases/${TESTFILE} ${FRAMES} OPT


clean:
	rm -f memSim
	rm -f *.o
