#ifndef PARSER_H
#define PARSER_H

#include "tree.h"

#define SUCCESS 0
#define FAILURE 1
#define TRUE 1
#define FALSE 0
#define MAX_INPUT_LEN 1024
#define MAX_FILENAME_LEN 80

/* This function parses a user's input and returns it in the form of a 
   binary tree.  */
Tree *parse(void);
#endif
