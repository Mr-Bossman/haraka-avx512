C=gcc
RM=rm -f
CFLAGS=-g -O3 -march=native -funroll-all-loops -fomit-frame-pointer -mavx512f
SRCS=main.c haraka.c aes.c
OBJS=$(subst .c,.o,$(SRCS))
OUTFILE="haraka"

all: haraka

haraka: $(OBJS)
	$(C) -o $(OUTFILE) $(OBJS) $(LDLIBS)

depend: .depend

.depend: $(SRCS)
	rm -f ./.depend
	$(C) $(CFLAGS) -MM $^>>./.depend;

clean:
	$(RM) $(OBJS)

dist-clean: clean
	$(RM) *~ .depend

include .depend
