HEADERS = parser.tab.h functree.h differentiate.h reduce.h
CSRC = main.c lex.yy.c parser.tab.c functree.c differentiate.c reduce.c

all: functor

parser.tab.c parser.tab.h: parser.y
	bison -d parser.y

lex.yy.c: parser.l parser.tab.h
	flex parser.l

functor: $(CSRC) $(HEADERS)
	gcc -g $(CSRC) -lfl -lm -o functor

clean:
	rm functor parser.tab.c parser.tab.h lex.yy.c

