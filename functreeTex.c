#include "functreeTex.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define OP(_op) (strcmp(node->op, _op) == 0)


char* ftree_tex (ftree_node *node) {
	assert (node && "Trying to build TeX string from a NULL node");
	char *str = NULL;
	char *strLChild = NULL, *strRChild = NULL;

	switch (node->type) {
		case NUMBER:
			asprintf (&str, "%g", node->num);
			return str;
		case FUNCTION:
			strLChild = (node->lchild) ? ftree_tex (node->lchild) : NULL;
			strRChild = ftree_tex (node->rchild);
			if (OP("/"))
				asprintf (&str, "\\frac{%s}{%s}", strLChild, strRChild);
			else if (OP("ln") || OP("exp") || OP("sin") || OP("cos") ||
			         OP("tan") || OP("cot") || OP("arcsin") || OP("arccos") ||
			         OP("arctan") || OP("arccot"))
				asprintf (&str, "\\%s({%s})", node->op, strRChild);
			else if (strLChild)
				asprintf (&str, "{%s}%s{%s}", strLChild, node->op, strRChild);
			else
				asprintf (&str, "%s{%s}", node->op, strRChild);
			free (strLChild);
			free (strRChild);
			return str;
		case VARIABLE:
			return strdup("x");
		default:
			assert (0 && "Something has gone wrong while building TeX string");
	}
}


char* ftree_texDoc (ftree_node *node) {
	char *funcstr = ftree_tex (node);
	char *str = NULL;
	asprintf (&str, "\\documentclass{article}\n\\begin{document}\n\\[%s\\]\n\\end{document}", funcstr);
	free (funcstr);
	return str;
}


void ftree_writeTexFile (char *filename, ftree_node *node) {
	FILE *tex = fopen (filename, "w");
	char *str = ftree_texDoc (node);
	fprintf (tex, "%s", str);
	free (str);
	fclose (tex);
}

