#pragma once
#define _GNU_SOURCE
#include "functree.h"

char* ftree_tex (ftree_node *node);
char* ftree_texDoc (ftree_node *node);
void ftree_writeTexFile (char *filename, ftree_node *node);

