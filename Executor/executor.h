#ifndef EXECUTOR_H
#define EXECUTOR_H

#include "../Parser/tree.h"

#define SUCCESS 0
#define FAILURE 1

/* This function executes a command represented
   by the argument tree. It returns SUCCESS if the
   command was successfully executed and FAILURE if
   not. */
/* Note: Any child processes that are created during the course of execution and
         are not execd must free any dynamically allocated memory associated with
         the tree which they get as a result of the state of the parents heap at
         the point they were forked. */
int execute(Tree *tree);

#endif
