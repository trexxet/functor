all: functor

parser.tab.c parser.tab.h: parser.y
	bison -d parser.y

lex.yy.c: parser.l parser.tab.h
	flex parser.l

functor: lex.yy.c parser.tab.c parser.tab.h functree.c functree.h differentiate.c differentiate.h reduce.c reduce.h
	gcc -g functree.c differentiate.c reduce.c parser.tab.c lex.yy.c -lfl -lm -o functor

clean:
	rm functor parser.tab.c parser.tab.h lex.yy.c

