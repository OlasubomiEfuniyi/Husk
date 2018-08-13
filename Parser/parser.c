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
#define MAX_FILENAME_LEN 80
#define PIPE "|"
#define AND "&&"
#define OR "||"
#define SUBSHELL "()"
#define INPUT "<"
#define OUTPUT ">"

static int read_input(int max, char input[], FILE *source);
static int contains_operator(const char *source, const char *operator);
static int break_string(char *string, char *operator, char **left, char **right);
static int get_redirection_filename(const char *source, const char *type,
				    char *filename);

int main(void) {
  parse();
  
  return SUCCESS;
}

Tree *parse() {
   char input[MAX_INPUT_LEN + 1] = "";
   char input_filename[MAX_FILENAME_LEN + 1] = "";
   char output_filename[MAX_FILENAME_LEN + 1] = "";
   
   int len = 0;
   
   /* Create the Tree */
   Tree tree = {NULL, 0};
   
   /* Read in the command line input */
   read_input(MAX_INPUT_LEN, input, stdin);
   len = strlen(input);
   
   /* Look for a subshell to begin with */ 
   if(*input == '(') {
     /* Find the last closing parenthesis */
     char *c_ptr = input + len -1;
     while(*c_ptr != ')' && c_ptr != input) {
       c_ptr--;
     }

     /* Check if loop terminated because no closing
        parenthesis was found */ 
     if(c_ptr == input) {
       fprintf(stderr, "Invalid command: Parentheses are unbalanced\n");
     }

     /* Loop terminated because closing parentheses
        was found */
     /* Make sure the only symbols after are input or 
        output redirection if any */
     if(!contains_operator(c_ptr, PIPE) && !contains_operator(c_ptr, AND)
	&& !contains_operator(c_ptr, OR)) { 
       if(contains_operator(c_ptr, INPUT)) {
	 /* Extract the filename for input redirection */
	 get_redirection_filename(c_ptr, INPUT, input_filename);
	 printf("INPUT: %s\n", input_filename);
       }

       if(contains_operator(c_ptr, OUTPUT)) {
	 /* Extract the filename for output redirection */
	 get_redirection_filename(c_ptr, OUTPUT, output_filename);
	 printf("OUTPUT: %s\n", output_filename);
       }
     }

     /* Set up the node and add it to the tree */
   } 
   
   return NULL;
} 

/* This function will read an entire string from
   the terminal console. It will read in a total
   of max-1 characters and then complete the string
   with the null character. The returned string will
   be trimmed so that trailing and beginning spaces
   will no longer be part of the input string.
   The string is returned via the input output parameter. 
   The function returns SUCCESS on success and FAILURE 
   on failure.*/
static int read_input(int max, char input[], FILE *source) {
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
      char_ptr++;
    }

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
static int contains_operator(const char *source, const char *operator) {
  if(strstr(source, operator) == NULL) {
    return FALSE;
  } else {
    return TRUE;
  }
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

/* This function extracts the filename for INPUT or OUTPUT redirection and
   returns it using the filename output parameter. The function returns
   SUCCESS on success and FAILURE on failure. Failure will be due to the 
   fact that no redirection of the specified type exists. */
static int get_redirection_filename(const char *source, const char *type, 
                                    char *filename) {
  char *start = NULL;

  /* Start should point to the first character after the redirection
     symbol */
  start = strstr(source, type) + 1;
  if(start == NULL) {
    /* Redirection symbol was not found */
    return FAILURE;
  }

  /* Find the first non space character */
  while(*start == ' ') {
    start++;
  }

  /* Copy filename character by charcter */
  while(*start != ' ' && *start != '\0') {
    *filename = *start;
    filename++;
    start++;
  }

  *filename = '\0';
  
  return SUCCESS;
}
 
