CC=mpicc
CFLAGS= -Wall -g
LIBS=-lpthread -lm
MAIN=relax
SRCS=main.c matrix.c relax.c
OBJS=$(SRCS:.c=.o)
TEST=test.sh

.PHONY: all
all: $(MAIN)
	@echo "Compiled $(MAIN)"

$(MAIN) : $(OBJS)
	${CC} ${CFLAGS} -o $(MAIN) $(OBJS) $(LIBS)

.c.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c $<  -o $@

.PHONY: clean
clean:
	${RM} *.o *.~ $(MAIN) grid.bmp time.*.log

.PHONY: test
test: $(MAIN)
	bash $(TEST)
