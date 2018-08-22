#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "Parser/tree.h"
#include "Parser/parser.h"
#include "Executor/executor.h"

#define PROMPT "Husk "
#define DIR_LEN 1024
int main(void) {
  char dirname[DIR_LEN + 1] = {'\0'};
  char *start = NULL;
  int count = 0;
  
  /* Run shell until it is terminated by a command */
  while(1) {
    /* Get the name of the current directory */
    getcwd(dirname, DIR_LEN + 1);

    /* Extract the last two directories */
    start = dirname + strlen(dirname) - 1; /* Start from last char */

    /* Scan the string for the second foward slash 
       from the end */
    while(start >= dirname && count < 2) {
      if(*start == '/') {
	count++; 
      }
      start--;
    }

    start++; /* Start from the foward slash */
    count = 0; /* Reset count */
    
    /* Set the color for the prompt to bold blue and print
       the prompt */
    fprintf(stdout, "\033[1;34m");
    fprintf(stdout, PROMPT);

    /* Print pwd in cyan color */
    fprintf(stdout, "\033[0;36m");
    fprintf(stdout, "{%s} ", start);

    /* Print $$ in magenta */
    fprintf(stdout, "\033[1;34m");
    fprintf(stdout, "$$ ");
    
    /* Reset fprintf to use its default color */
    fprintf(stdout, "\033[0m");
    
    execute(parse());
  }

  exit(EXIT_SUCCESS);
}
