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
#include "tree.h"

static Tree *parse_aux(char *input, Tree *tree);
static int read_input(int max, char input[], FILE *source);
static int contains_operator(const char *source, const char *operator);
static int break_string(const char *string, const char *operator, char *left, char *right);
static int get_redirection_filename(const char *source, const char *type,
				    char *filename);
static void print_node(Node *node);

/* This function reads input from the command line and
   passes it as an argument to parse_aux. It returns the result
   returned by parse_aux and its purpose is to aid the initial 
   recursive call of the parse_aux function. */ 
Tree *parse(void) {
  char input[MAX_INPUT_LEN + 1] = "";
  Tree *tree = NULL;

  if((tree = calloc(1, sizeof(Tree))) == NULL) {
    perror("Calloc failed");
    return NULL;
  }
   
  /* Read in the command line input */
  read_input(MAX_INPUT_LEN, input, stdin);
  
  tree = parse_aux(input, tree);
  print_tree(tree);
  return tree;
}

/* This function is recursive and hence, is called
   with the input already obtained from the user by 
   parse_initial. Look in parse.h for more details. */
static Tree *parse_aux(char *input, Tree *tree) {
  /* Note: The order of detection matters to the tree
     structure. */
   char input_filename[MAX_FILENAME_LEN + 1] = "";
   char output_filename[MAX_FILENAME_LEN + 1] = "";
   int len = 0;
   Node *node = NULL;
   Tree *res = NULL;

   /* Base case */
   if(strcmp(input, "") == 0) {
     return tree;
   }

   len = strlen(input);
   
   /*********** START of SUBSHELL detection ********************/ 
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
       return NULL;
     } else {
       char *left = NULL;
       /* Loop terminated because closing parentheses
	  was found */
       /* Make sure the only symbols after are input or 
	  output redirection if any */
       if(!contains_operator(c_ptr, PIPE) && !contains_operator(c_ptr, AND)
	  && !contains_operator(c_ptr, OR)) {
	 /* Perform subsequent operations if no other operator
            exists outside ). If operator exists, no return
            will take place and the next operator check will
            take place. */
	 if(contains_operator(c_ptr, INPUT)) {
	   /* Extract the filename for input redirection */
	   get_redirection_filename(c_ptr, INPUT, input_filename);
	 }

	 if(contains_operator(c_ptr, OUTPUT)) {
	   /* Extract the filename for output redirection */
	   get_redirection_filename(c_ptr, OUTPUT, output_filename);
	 }

	 
	 /* Set up the node and add it to the tree */
	 if((node = calloc(1, sizeof(Node))) == NULL) {
	   perror("Calloc failed");
	   return NULL;
	 }

	 (node->type) = COMMAND;

	 /* Add possible input and output files to the node */
	 (node->input) = (strcmp(input_filename, "") == 0) ? NULL :
	   input_filename;
	 (node->output) = (strcmp(output_filename, "") == 0) ? NULL :
	   output_filename;
	 /* Add an arguements array to the node */
	 /* Note: Subshell has no other arguments other than the name of 
	    the name of the command */
	 if(((node -> args) = calloc(2, sizeof(char *))) == NULL) {
	   perror("Calloc failed");
	   return NULL;
	 }

	 /* Dynamically allocate space for the command name */
	 if(((node -> args)[0] = calloc(strlen(SUBSHELL) + 1, 1)) == NULL) {
	   perror("Calloc failed");
	   return NULL;
	 }
     
	 strcpy((node -> args)[0], SUBSHELL);
	 (node -> args)[1] = NULL;

	 /* Add the node to the tree */
	 add_node(tree, node);

	 /* Extract the part of the input that makes
	    up the subtrees for the  subshell */
	 /* Allocate space for two less characters but include 
	    space for null byte */
	 if((left = calloc(len-1, 1)) == NULL) {
	   perror("Calloc failed");
	   return NULL;
	 }
       
	 break_string(input, SUBSHELL, left, NULL);
       
	 /* Proccess the rest of the input recursively */
	 res = parse_aux(left, tree);
	 free(left);
	 return res;
       }
     }
     
   }
   
   /********* END of SUBSHELL detection ****************/

   /********* START of OR detection *******************/
   if(contains_operator(input, OR)) {
     /* Parse OR operator */
     char *left = NULL;
     char *right = NULL;
     
     /* Set up node and add it to the tree */
     if((node = calloc(1, sizeof(Node))) == NULL) {
       perror("Calloc failed");
       return NULL;
     }

     node->type = OPERATOR;

     /* Dynamically allocate memory for the operators name in a
        NULL terminated string array */
     if((node->args = calloc(2, sizeof(char *))) == NULL) {
       perror("Calloc failed");
       return NULL;
     }

     if((node->args[0] = calloc(strlen(OR) + 1, 1)) == NULL) {
       perror("Calloc failed");
       return NULL;
     }

     strcpy(node->args[0], OR);
     add_node(tree, node);

     /* Dynamically allocate space for the left and right tokens
        of the || operator */
     /* left has at most len - 2 characters pluss null character */
     if((left = calloc (len-1, 1)) == NULL) {
       perror("Calloc failed");
       return NULL;
     }

     /* right has at most len - 2 characters pluss null character */
     if((right = calloc (len-1, 1)) == NULL) {
       perror("Calloc failed");
       return NULL;
     }
	  
     break_string(input, OR, left, right);

     /* Make sure the structure of the input is correct */
     if(*left == '\0' || *right == '\0') {
       fprintf(stderr, "Error: Invalid us of ||\n");
       return NULL;
     }

     /* Parse the left side of || and then the right side */
     res = parse_aux(left, tree);
     res = parse_aux(right, res); /* Pass the most up to date tree */

     free(left);
     free(right);
     
     return res;
   }

   return tree;
   /*********** END of OR detection ***********/
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
  if((buffer = calloc(max, 1)) == NULL) {
    perror("Calloc failed");
    return FAILURE;
  }
  
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
static int break_string(const char *string, const char *operator, char*left, char *right) {
  char *left_ptr = left;
  char *right_ptr = right;
  
  if(string != NULL) {
    /* Determine how extraction should be performed */
    if(strcmp(operator, SUBSHELL) == 0) {
      /* Extract around a subshell operator */
      const char *start = NULL;
      int len = strlen(string);
      /* stop should start from the character before the null byte */
      const char *stop = string + len - 1;  

      /* Find the opening parenthesis */
      while(*string != '\0' && *string != '(') {
	string++;
      }

      if(*string == '\0') {
	/* The opening parenthesis could not be found */
	fprintf(stderr, "Error: Operator not found\n");
	return FAILURE;
      }
      
      start = string;

      /* Find the closing parenthesis */
      while(stop > start && *stop != ')') {
	stop--;
      }

      if(stop == start) {
	/* closing parenthesis was not found */
	fprintf(stderr, "Error: Operator not found\n");
	return FAILURE;
      }

      /* Copy the contents of the subshell into left */
      start++;
      while(start < stop) {
	*left_ptr = *start;
	left_ptr++;
	start++;
      }

      *left_ptr = '\0';
    } else if(strcmp(operator, OR) == 0 ||
	      strcmp(operator, AND) == 0 ||
	      strcmp(operator, PIPE) == 0) {
      char *op_ptr = strstr(string, operator);

      if(op_ptr == NULL) {
	/* Operator was not found */
	fprintf(stderr, "Error: Operator not found\n");
	return FAILURE;
      }

      /* Copy substring to the left operator
	 into left */ 
      while(string < op_ptr) {
	*left = *string;
	left++;
	string++;
      }
      *left = '\0';

      /* Skip operator */
      string += strlen(operator);

      /* Copy substring to the right operator
	 into right */
      while(*string != '\0') {
	*right = *string;
	right++;
	string++;
      }
      *right = '\0';
      
    } else {
      fprintf(stderr, "Error: Invalid operator\n");
      return FAILURE;
    }

    return SUCCESS;
  }

  return FAILURE;
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

/* This function prints the information stored in the
   members of a Node structure. */ 
static void print_node(Node *node) {
  if(node != NULL) {
    char **args;
    
    printf("TYPE: ");
    if((node->type) == COMMAND) {
      printf("COMMAND\n");
    } else if((node->type) == OPERATOR) {
      printf("OPERATOR\n");
    } else {
      printf("UNKNOWN\n");
    }

    printf("INPUT: %s\n", (node->input));
    printf("OUTPUT: %s\n", (node->output));

    args = (node->args);
    printf("ARGS: ");
    while(*args) {
      printf("%s ", *args);
      args++;
    }

    printf("\n");
  }
}
