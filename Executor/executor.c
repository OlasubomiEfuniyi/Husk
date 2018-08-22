
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "executor.h"

#define W_PERM O_WRONLY|O_CREAT|O_TRUNC
#define R_PERM O_RDONLY
#define MODE 0664
#define TRUE 1
#define FALSE 0

static int execute_aux(Tree *t, int p_input_fd,
		       int p_output_fd);

int execute(Tree *t) {
  int ret = 0;
  ret = execute_aux(t, STDIN_FILENO, STDOUT_FILENO);

  /* Free the tree and its subtrees */
  destroy_tree(t);
  free(t);
  return ret;
}

/* This function recursively executes the command represented by
   the tree stucture. It returns SUCCESS if successful and
   FAILURE if not. */ 
static int execute_aux(Tree *t, int p_input_fd,
		       int p_output_fd) {
  if(t && t -> root) {
    /* Is this a leaf node */
    if(IS_COMMAND(t -> root)) {
      /* It is a leaf node */
      pid_t child_id = 0;
    
      /**** Execute shell commands "cd" and "exit" *****/
      if(strcmp((t -> root ->args)[0], "cd") == 0) {
	/* Execute the cd command */
	if((t -> root -> args)[1] == NULL) {
	  char *home = getenv("HOME");
	  if(chdir(home)) {
	    perror(home);
	    return FAILURE;
	  }
	} else {
	  if(chdir((t -> root -> args)[1])) {
	    perror((t -> root -> args)[1]);
	    return FAILURE;
	  }
	}
      } else if(strcmp((t -> root -> args)[0], "exit") == 0) {
	/* Execute the exit command */
	exit(EXIT_SUCCESS);
      } else {
	/************ Execute UNIX commands ***********/
	char **args = NULL;
	int should_wait = TRUE; 
         /******** Check to see if & was appended at the end of the command *********/
	  args = t -> root -> args;

	  /* Skip to the end of the arguments */
	  while(*args) {
	    args ++;
	  }

	  args--; /* Look at the last argument */
         /***************** End of check *******************************/

	  if(strcmp(*args, "&") == 0) {
	    *args = NULL;
	    should_wait = FALSE;
	  }
	  
	/* Create a child to execute the command */
	if((child_id = fork()) < 0) {
	  perror("fork");
	  return FAILURE;
	}

	if(child_id) {
	  /* This is the parent */
	  int status = 0;

	  if(should_wait) {
	    /* Wait if background processing is not specified. Make only reap
               the most recent child and not any that was already existing 
               in the background. */
	    waitpid(child_id, &status, 0);
	    if(status !=  0) {
	       return FAILURE;
	    }

	    return SUCCESS;
	  } else {
	    printf("[%d] - Running\n", child_id);
	    return SUCCESS;
	  }
	  	
	} else {
	  /* This is the child */
	
	  /***** Make input and output point to the right places ******/
	  /* Check if input redirection exists */
	  if((t -> root -> input) == NULL) {
	    /* No */
	    dup2(p_input_fd, STDIN_FILENO);
	    /* Avoid closing if the two are the same */
	    if(p_input_fd != STDIN_FILENO) {
	      close(p_input_fd);
	    }
	  } else {
	    /* Yes */
	    int new_fd = 0;
	  
	    /* Open the file */
	    if((new_fd = open(t -> root -> input, R_PERM)) < 0) {
	      perror("open");
	      exit(EXIT_FAILURE);
	    }
	   
	    dup2(new_fd, STDIN_FILENO);
	    close(new_fd);
	  }
     
	  /* Check if output redirection exists */
	  if(t -> root -> output == NULL) {
	    /* No */
	    dup2(p_output_fd, STDOUT_FILENO);
	    /* Avoid closing if the two are the same */
	    if(p_output_fd != STDOUT_FILENO) {
	      close(p_output_fd);
	    }
	  } else {
	    /* Yes */
	    int new_fd = 0;
	  
	    /* Open the file */
	    if((new_fd = open(t -> root -> output, W_PERM, MODE)) < 0) {
	      perror("open");
	      exit(EXIT_FAILURE);
	    }

	    dup2(new_fd, STDOUT_FILENO);
	    close(new_fd);
	  }

	  execvp((t -> root -> args)[0], t -> root -> args);
	
	  /* This prints if execvp fails */
	  fprintf(stderr, "Failed to execute %s\n", (t -> root -> args)[0]);

	  exit(EXIT_FAILURE);
	}
      }
    } else if(strcmp(t -> root -> args[0], PIPE) == 0) {
      /* This is a pipe node */
      /******Set up a pipe *************/
      int fd[2] = {0, 0};
      pid_t child_id1 = 0, child_id2 = 0;
      int status1 = 0, status2 = 0;

      /* Check for ambiguity */
      if((t -> left -> root -> output) != NULL) {
	printf("Ambiguous output redirect.\n");
	return FAILURE;
      } else if((t -> right -> root -> input) != NULL) {
	printf("Ambiguous input redirect.\n");
	return FAILURE;
      }
      
      /* Create pipe */
      if(pipe(fd) < 0) {
	perror("pipe");
	return FAILURE;
      }

      /* Create first child */
      if((child_id1 = fork()) < 0) {
	perror("fork");
	return FAILURE;
      }
   
      if(!child_id1) {
	/* This is the first child */
	int res = 0;

	/* Close the read end of the pipe */
	close(fd[0]);

	if((t -> root -> input) == NULL) {
	  /* left gets what parent got for input */
	  res = execute_aux(t -> left, p_input_fd, fd[1]);
	} else {
	  /* left gets a new file descriptor */
	  int new_fd = 0;

	  if((new_fd = open(t -> root-> input, R_PERM)) < 0) {
	    perror("open");
	    exit(EXIT_FAILURE);
	  }

	  res = execute_aux(t -> left, new_fd, fd[1]); 
	}

	/* Close the write end of the pipe */
	close(fd[1]);

	/* Destroy this child's version of the tree */
	destroy_tree(t);
	free(t);
      
	if(res == SUCCESS) {
	  exit(EXIT_SUCCESS);
	} else {
	  exit(EXIT_FAILURE);
	}
      }

      /* Create second child */
      if((child_id2 = fork()) < 0) {
	perror("fork");
	return FAILURE;
      }

      if(!child_id2) {
	/* This is the second child */
	int res = 0;

	/* Close the write end of the pipe */
	close(fd[1]);

	if((t -> root -> output) == NULL) {
	  /* right gets what parent got for output */
	  res = execute_aux(t -> right, fd[0], p_output_fd);
	} else {
	  /* right gets a new file descriptor */
	  int new_fd = 0;

	  if((new_fd = open(t -> root -> output, W_PERM, MODE)) < 0) {
	    perror("open");
	    exit(EXIT_FAILURE);
	  }

	  res = execute_aux(t -> right, fd[0], new_fd); 
	}
      
	/* Close the read end of the pipe */
	close(fd[0]);

	/* Destroy this child's version of the tree */
	destroy_tree(t);
	free(t);
	if(res == SUCCESS) {
	  exit(EXIT_SUCCESS);
	} else {
	  exit(EXIT_FAILURE);
	}
      }
    
      /* Close the read and write end of the pipe */
      close(fd[0]);
      close(fd[1]);

      /* Wait to reap children */
      wait(&status1);
      wait(&status2);
    
      if(status1 != 0 || status2 != 0) {
	return FAILURE;
      }
    } else if(strcmp(t -> root -> args[0], AND) == 0) {
      /* This is an AND node */
      int input_fd = p_input_fd, output_fd = p_output_fd,
        res = 0;

      /* Determine source of input */
      if((t -> root -> input) != NULL) {
	/* left and right get a new input */ 
	int new_fd = 0;

	if((new_fd = open(t -> root -> input, R_PERM)) < 0) {
	  perror("open");
	  exit(EXIT_FAILURE);
	}

	input_fd = new_fd;
      }

      /* Determine output destination */
      if((t -> root -> output) != NULL) {
	/* left and right get a new output */ 
	int new_fd = 0;

	if((new_fd = open(t -> root -> output, W_PERM, MODE)) < 0) {
	  perror("open");
	  exit(EXIT_FAILURE);
	}

	output_fd = new_fd;
      }

      /* Execute the commands */
      if(execute_aux(t -> left, input_fd, output_fd) == SUCCESS) {
	res = execute_aux(t -> right, input_fd, output_fd);
   
	/* Close the opened files */
	if(t -> root -> input != NULL) {
	  close(input_fd);
	}
      
	if(t -> root -> output != NULL) {
	  close(output_fd);
	}

	return res;
      } else {
	/* Close the opened files */
	if(t -> root -> input != NULL) {
	  close(input_fd);
	}
      
	if(t -> root -> output != NULL) {
	  close(output_fd);
	}
      
	return FAILURE;
      }
    } else if(IS_SUBSHELL(t -> root)) {
      /************ This is a SUBSHELL node *******************/
      pid_t child_id = 0;
      int status = 0;
    
      /* Fork the child */
      if((child_id = fork()) < 0) {
	perror("fork");
	return FAILURE;
      }

      if(!child_id) {
	/* This is the child */
	int input_fd = p_input_fd, output_fd = p_output_fd,
	  res = 0;

	/* Determine source of input */
	if((t -> root -> input) != NULL) {
	  /* left and right get a new input */ 
	  int new_fd = 0;

	  if((new_fd = open(t -> root -> input, R_PERM)) < 0) {
	    perror("open");
	    exit(EXIT_FAILURE);
          }

          input_fd = new_fd;
	}

	/* Determine output destination */
	if((t -> root -> output) != NULL) {
	  /* left and right get a new output */ 
	  int new_fd = 0;

	  if((new_fd = open(t -> root -> output, W_PERM, MODE)) < 0) {
	    perror("open");
	    exit(EXIT_FAILURE);
	  }

	  output_fd = new_fd;
	}

	res = execute_aux(t -> left, input_fd, output_fd);

	/* Close the opened files */
	if(t -> root -> input != NULL) {
	  close(input_fd);
	}

	if(t -> root -> output != NULL) {
	  close(output_fd);
	}
      
	if(res == SUCCESS) {
	  exit(EXIT_SUCCESS);
	} else {
	  exit(EXIT_FAILURE);
	}

	/* Destroy this child's version of the tree */
	destroy_tree(t);
	free(t);
      }

      /* Wait to reap the child */
      wait(&status);

      if(status != 0) {
	return FAILURE;
      }
    }
  
    return SUCCESS;
  }

  return FAILURE;
}
