/* Name: parser.c
   Author: Olasubomi Efuniyi
   Purpose: This file contains functions to parse a users
            input from the command line.
   Date: 08|11|2018
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "parser.h"

#define MAX_INPUT_LEN 1024
#define PIPE "|"
#define AND "&&"
#define OR "||"
#define SUBSHELL "()"

static int read_input(int max, char input[], FILE *source, int *length);
static int contains_operator(char *source, char *operator);
static int break_string(char *string, char *operator, char **left, char **right);

int main(void) {
  parse();
  
  return SUCCESS;
}

Tree *parse() {
   char input[MAX_INPUT_LEN + 1] = "";
   int len = 0;

   /* Create the Tree */
   Tree tree = {NULL, 0};
   
   /* Read in the command line input */
   read_input(MAX_INPUT_LEN, input, stdin, &len);
   printf("Length: %d\n", len);
   puts(input);
   /* Look for a subshell to begin with 
   if(*input == '(' && *(input + len -1) == ')') {
     Node node = {NULL, NULL, COMMAND, NULL, NULL, NULL};
     } */
   
   return NULL;
} 

/* This function will read an entire string from
   the terminal console. It will read in a total
   of max-1 characters and then complete the string
   with the null character. The string is returned via 
   the input output parameter. The function returns
   SUCCESS on success and FAILURE on failure.*/
static int read_input(int max, char input[], FILE *source, int *length) {
  int len = 0;
  char *buffer = NULL;
  
  /* Dynamically allocate memory for the buffer */
  buffer = calloc(max, 1);
  
  if(fgets(buffer, max, source) != NULL) {
    /* Remove the newline character and determine the 
       length of the string */
    char *char_ptr = buffer, *start = buffer, *end = NULL;
    
    while(*char_ptr) {
      if(*char_ptr == '\n') {
	*char_ptr = '\0';
	/* End points to the character before the 
           first occurence of the null character */
	end = char_ptr - 1;
	break;
      }
      len++;
      char_ptr++;
    }

    *length = len;

    /* Trim the input string */
    /* Skip spaces at the beginning */
    while(*start == ' ') {
      start++;
    }
    /* Skip spaces at the end */
    while(*end == ' ') {
      end--;
    }

    /* Copy the rest of the string character by character */
    while(start <= end) {
      *input = *start;
      start++;
      input++;
    }

    /* Null terminate the string in the input array */
    *input = '\0';
    
    /* Free dynamically allocated memory */
    free(buffer);
    return SUCCESS;
  } else {
    free(buffer);
    return FAILURE;
  }
}

/* This function will determine whether or not a 
   source string contains an operator. It will return
   TRUE if it does and FALSE otherwise. */
static int contains_operator(char *source, char *operator) {
  return 0;
}

/* This function breaks a string around the first occurence
   of a specified operator. It returns the left part of the 
   broken down string using the left output parameter and the 
   right part of the broken down string using the right output
   parameter. It returns SUCCESS on success and FAILURE on 
   failure. */ 
static int break_string(char *string, char *operator, char **left, char **right) {
  return 0;
}
