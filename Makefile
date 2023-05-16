.PHONY: all clean

CC = gcc
CFLAGS = -g -Wall -Werror

PROG   = memSim
SRCS   = memSim.c page_table.c tlb.c safeutil.c options.c
OBJS   = memSim.o page_table.o tlb.o safeutil.o options.o
HEADER = memSim.o page_table.h tlb.h safeutil.h options.h

FRAMES = 256
TESTFILE = fifo1.txt

all:  $(PROG)

#$(PROG): memSim.c ${OBJS}
#	$(CC) $(CFLAGS) -o memSim memSim.c ${OBJS}

$(PROG): ${OBJS}
	$(CC) $(CFLAGS) -o $@ $^

%.o : %.c
	$(CC) $(CFLAGS) -c $<

depends:
	@echo Regenerating local dependencies
	makedepend -Y $(SRCS)

fifo: clean memSim
	./memSim testcases/${TESTFILE} ${FRAMES} FIFO

lru: clean memSim
	../memSim testcases/${TESTFILE} ${FRAMES} LRU

opt: clean memSim
	./memSim testcases/${TESTFILE} ${FRAMES} OPT


clean:
	-rm -f $(PROG)
	-rm -f *.o
# DO NOT DELETE

memSim.o: options.h
page_table.o: page_table.h
tlb.o: tlb.h page_table.h
safeutil.o: options.h
options.o: options.h safeutil.h
