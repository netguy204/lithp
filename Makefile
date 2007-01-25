CFLAGS=-g $(ECFLAGS)
OBJS=parse_wrapper.o \
     lithp_primitives.o \
     lithp_eval.o \
     lex.yy.o \
     y.tab.o \
     environment.o \
     cons.o

all: y.tab.c lex.yy.c test_parser
test_parser: test_parser.o $(OBJS) 

lithp_primitives.o: lithp_primitives.c
lithp_eval.o: lithp_eval.c
lex.yy.o: lex.yy.c
y.tab.o: y.tab.c
environment.o: environment.c
cons.o: cons.c
y.tab.c: lithp.y
	bison -y -v -d $<
	cat append.y.tab.h y.tab.h > y.tab.h.temp
	mv y.tab.h.temp y.tab.h
lex.yy.c: lithp.lex
	flex $<
clean:
	rm -f *.o
	rm -f test_parser y.*
	rm -f lex.yy.c y.tab.c
