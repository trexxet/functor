#pragma once
#include "functree.h"
extern int reduceDebugging;
extern int reduceCalls;

int reduce (ftree_node **pnode);
void printReductionSteps (ftree_node **pnode);

