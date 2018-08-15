
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "parser.h"
#include "tree.h"
#include "stack.h"

/*********** START of stack test *************/
/* This function tests the init_stack
   function defined in stack.c */
void test_init_stack() {
  Stack stack;
  int num_elems = 10, i = 0;

  printf("test_init_stack:\n");
  init_stack(&stack, num_elems);
  printf("Top: %d\n", stack.top);

  for(i = 0; i < num_elems; i++) {
    stack.data[i] = &i;
  }
  
  for(i = 0; i < num_elems; i++) {
    printf("%d ", *((int *)(stack.data)[i]));
  }
  printf("\n");

  assert(stack.size == num_elems);
}

/* This function tests the push function defined in
   stack.c */
void test_push() {
  Stack stack;
  int num_elems = 5, i = 0, arr[5] = {5, 4, 3, 2, 1};

  printf("test_push:\n");
  init_stack(&stack, num_elems);

  for(i = 0; i < num_elems; i++) {
    push(&stack, &(arr[i]));
  }
  for(i = 0; i < num_elems; i++) {
    printf("%d ", *((int *)stack.data[i]));
  }
  printf("\n");

  assert(stack.top == 5);
  assert(push(NULL, NULL) == FAILURE); 
  assert(is_full(&stack));
  
  /* Try to push to a full stack */
  assert(push(&stack, &i) == FAILURE);
  assert(stack.top == stack.size);
}

/* This function tests the pop function
   defined in stack.c */
void test_pop() {
  Stack stack;
  int num_elems = 5, i = 0, arr[5] = {5, 4, 3, 2, 1};
  int *res;
  
  printf("test_pop:\n");
  init_stack(&stack, num_elems);

  for(i = 0; i < num_elems; i++) {
    push(&stack, &(arr[i]));
  }
  for(i = 0; i < num_elems; i++) {
    printf("%d ", *((int *)stack.data[i]));
  }
  printf("\n");
  
  assert(is_full(&stack));

  /* Pop one by one and test the state of the stack */
  for(i = 0; i < 5; i++) {
    res = pop(&stack);
    assert(*res == i + 1);
    assert(stack.top == 5 - i - 1);
  }

  assert(is_empty(&stack));
  /* Try to pop from an empty stack */
  assert(pop(&stack) == NULL);
  assert(stack.top == 0);
}

/********** END of stack test *************/

/*********** START of tree test *************/
/* This function tests the add_node 
   function defined in tree.c */
int test_add_node(void) {
  Tree tree = {NULL, NULL, NULL};

  Node *node = calloc(1, sizeof(Node));
   
  node->type = COMMAND;
  node->args = calloc(2, sizeof(char *));
  node->args[0] = calloc(strlen(SUBSHELL) + 1, 1);
  strcpy(node->args[0], "()");
  
  print_tree(&tree);
  add_node(&tree, node);
  print_tree(&tree);

  printf("\n");
  
  node = calloc(1, sizeof(Node));
  node->type = OPERATOR;
  node->args = calloc(2, sizeof(char *));
  node->args[0] = calloc(strlen(AND) + 1, 1);
  strcpy(node->args[0], AND);
  add_node(&tree, node);
  print_tree(&tree);

  printf("\n");
  
  node = calloc(1, sizeof(Node));
  node->type = COMMAND;
  node->args = calloc(2, sizeof(char *));
  node->args[0] = calloc(strlen("ls") + 1, 1);
  strcpy(node->args[0], "ls");
  add_node(&tree, node);
  print_tree(&tree);

  printf("\n");
  
  node = calloc(1, sizeof(Node));
  node->type = OPERATOR;
  node->args = calloc(2, sizeof(char *));
  node->args[0] = calloc(strlen("grep") + 1, 1);
  strcpy(node->args[0], "grep");
  add_node(&tree, node);
  print_tree(&tree);

  printf("\n");
  node = calloc(1, sizeof(Node));
  node->type = OPERATOR;
  node->args = calloc(2, sizeof(char *));
  node->args[0] = calloc(strlen("nano") + 1, 1);
  strcpy(node->args[0], "nano");
  add_node(&tree, node);
  print_tree(&tree);
  
  return SUCCESS;
}
/*********** END of tree test **********/

int main(void) {

  test_init_stack();
  test_push();
  test_pop();
  
  /*test_add_node();*/
  
  exit(EXIT_SUCCESS);
}
