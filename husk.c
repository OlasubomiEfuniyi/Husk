#include <stdio.h>
#include <stdlib.h>
#include "Parser/tree.h"
#include "Parser/parser.h"
#include "Executor/executor.h"

#define PROMPT "Husk $$ "

int main(void) {  
  /* Run shell until it is terminated by a command */
  while(1) {
    /* Set the color for the prompt to bold blue and print
       the prompt */
    fprintf(stdout, "\033[1;34m");
    fprintf(stdout, PROMPT);
    /* Reset fprintf to use its default color */
    fprintf(stdout, "\033[0m");
    
    execute(parse());
  }
  
  exit(EXIT_SUCCESS);
}
