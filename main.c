#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "functree.h"
#include "differentiate.h"
#include "reduce.h"

void parseCmdArgs (int argc, char *argv[], char** inputFilename);
void parseInput ();
void differentiateFunction ();

ftree_node *srcFunc = NULL;
int reduceOnly = 0;


int main (int argc, char *argv[]) {
	char *inputFilename = NULL;
	parseCmdArgs (argc, argv, &inputFilename);

	// Parse input
	extern FILE *yyin;
	yyin = fopen (inputFilename, "r");
	if (!yyin) {
		fprintf (stderr, "Cannot open input file\n");
		return 1;
	}
	parseInput ();
	fclose (yyin);

	char *str = ftree_str (srcFunc);
	printf ("Source function parsed to tree: \n\t%s\n", str);
	free (str);

	printf ("Reduction steps:\n");
	while (reduce (&srcFunc)) {
		str = ftree_str (srcFunc);
		printf ("\t%s\n", str);
		free (str);
	}

	if (!reduceOnly)
		differentiateFunction (srcFunc);

	extern int reduceCalls;
	printf ("reduce() was called %d times\n", reduceCalls);

	ftree_deleteNode (srcFunc);
	return 0;
}


void parseCmdArgs (int argc, char *argv[], char** inputFilename) {
	extern int reduceDebugging;
	int opt;
	opterr = 0;
	while ((opt = getopt (argc, argv, "dr")) != -1)
		switch (opt) {
			case 'd':
				reduceDebugging = 1;
				break;
			case 'r':
				reduceOnly = 1;
				break;
			case '?':
			default:
				fprintf (stderr, "Usage: differ <input file> [-dr]\n");
				exit (1);
		}
	*inputFilename = argv[optind];
}


void parseInput () {
	extern int yyparse ();
	extern FILE *yyin;

	printf ("Source function:\n\t");
	int chr = 0;
	while ((chr = getc (yyin)) != EOF)
		putchar (chr);
	rewind (yyin);

	do yyparse(); while (!feof (yyin));
}


void differentiateFunction () {
	ftree_node *diffed = differentiate (srcFunc);
	char *str = ftree_str (diffed);
	printf ("Differentiated non-reduced function: \n\t%s\n", str);
	free (str);

	printf ("Reduction steps:\n");
	while (reduce (&diffed)) {
		str = ftree_str (diffed);
		printf ("\t%s\n", str);
		free (str);
	}
	ftree_deleteNode (diffed);

	extern int differentiateCalls;
	printf ("differentiate() was called %d times\n", differentiateCalls);
}


void yyerror (const char * msg) {
	fprintf (stderr, "Failure: %s\n", msg);
	exit(1);
}

