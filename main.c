#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "functree.h"
#include "differentiate.h"
#include "reduce.h"
#include "functreeTex.h"

extern FILE *yyin;
extern int yyparse ();
extern int yy_scan_string (char *);
extern int yylex_destroy ();

#define MAX_STDIN_INPUT_LEN 1024

void parseCmdArgs (int argc, char *argv[], char** inputFilename);
int parseInput (char *inputFilename);
void differentiateFunction ();

ftree_node *srcFunc = NULL;
int reduceOnly = 0;
int texOut = 0;


int main (int argc, char *argv[]) {
	char *inputFilename = NULL;
	parseCmdArgs (argc, argv, &inputFilename);

	if (parseInput (inputFilename) != 0)
		return EXIT_FAILURE;

	char *str = ftree_str (srcFunc);
	printf ("Source function parsed to tree: \n\t%s\n", str);
	free (str);

	printf ("Reduction steps:\n");
	printReductionSteps (&srcFunc);

	if (texOut) {
		ftree_writeTexFile ("source_tex.txt", srcFunc);
		printf ("TeX'ed and reduced source function written to source_tex.txt\n");
	}

	if (!reduceOnly)
		differentiateFunction (srcFunc);

	printf ("reduce() was called %d times\n", reduceCalls);

	ftree_deleteNode (srcFunc);
	return 0;
}


void parseCmdArgs (int argc, char *argv[], char** inputFilename) {
	int opt;
	opterr = 0;
	while ((opt = getopt (argc, argv, "drt")) != -1)
		switch (opt) {
			case 'd':
				reduceDebugging = 1;
				break;
			case 'r':
				reduceOnly = 1;
				break;
			case 't':
				texOut = 1;
				break;
			case '?':
			default:
				fprintf (stderr, "Usage: differ <input file> [-drt]\n");
				exit (1);
		}
	*inputFilename = argv[optind];
}


int parseInput (char *inputFilename) {
	if (inputFilename) {
		yyin = fopen (inputFilename, "r");
		if (!yyin) {
			fprintf (stderr, "Cannot open input file\n");
			return 1;
		}
		printf ("Source function:\n\t");
		int chr = 0;
		while ((chr = getc (yyin)) != EOF)
			putchar (chr);
		rewind (yyin);
		do yyparse(); while (!feof (yyin));
		fclose (yyin);
	}
	else {
		char inputStr[MAX_STDIN_INPUT_LEN] = {0};
		fgets (inputStr, MAX_STDIN_INPUT_LEN, stdin);
		yy_scan_string (inputStr);
		yyparse ();
	}
	yylex_destroy ();
	return 0;
}


void differentiateFunction () {
	ftree_node *diffed = differentiate (srcFunc);
	char *str = ftree_str (diffed);
	printf ("Differentiated non-reduced function: \n\t%s\n", str);
	free (str);

	printf ("Reduction steps:\n");
	printReductionSteps (&diffed);

	if (texOut) {
		ftree_writeTexFile ("differed_tex.txt", diffed);
		printf ("TeX'ed and reduced differentiated function written to differed_tex.txt\n");
	}

	ftree_deleteNode (diffed);

	printf ("differentiate() was called %d times\n", differentiateCalls);
}


void yyerror (const char * msg) {
	fprintf (stderr, "Failure: %s\n", msg);
	exit(1);
}

