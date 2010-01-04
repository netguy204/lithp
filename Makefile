CFLAGS=-g $(ECFLAGS)
SRCS=parse_wrapper.c \
     lithp_primitives.c \
     lithp_eval.c \
     environment.c \
     cons.c

MYOBJS=$(patsubst %.c, %.o, $(SRCS))

OBJS=$(MYOBJS) \
     lex.yy.o \
     y.tab.o \

test_parser: test_parser.o $(OBJS) 

%.o: %.c
	$(CC) -MMD -c -o $@ $< $(CFLAGS)
	@cp $*.d $*.P; \
	sed -e 's/#.*//' -e 's/^[^:]*: *//' -e 's/ *\\$$//' \
	-e '/^$$/ d' -e 's/$$/ :/' < $*.d >> $*.P; \
	rm -f $*.d

parse_wrapper.c: y.tab.h

y.tab.c y.tab.h: lithp.y
	bison -y -v -d $<
	cat append.y.tab.h y.tab.h > y.tab.h.temp
	mv y.tab.h.temp y.tab.h
lex.yy.c: lithp.lex
	flex $<
clean:
	rm -f *.o
	rm -f test_parser y.*
	rm -f lex.yy.c y.tab.c
	rm -f *.P

-include $(OBJS:%.o=%.P)
