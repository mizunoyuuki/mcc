CFLAGS=-std=c11 -g -static
SRCS=main.c tokenizer.c parser.c generator.c util.c type.c optimize.c
OBJS=$(SRCS:.c=.o)

mcc: $(OBJS)
	$(CC) -o mcc $(OBJS) $(LDFLAGS)

$(OBJS): mcc.h

test: mcc
	./test.sh

clean:
	rm -f mcc *.o *~ tmp*

.PHONY: test clean
