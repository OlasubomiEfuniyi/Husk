
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
  destroy_stack(&stack);
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
  destroy_stack(&stack);
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

  destroy_stack(&stack);
}

/********** END of stack test *************/

/*********** START of tree test *************/
/* This function tests the add_node 
   function defined in tree.c */
void test_add_node1(void) {
  Tree tree;

  Node *node = calloc(1, sizeof(Node));

  printf("test_add_node1:\n");

  init_tree(&tree);
  
  node->type = OPERATOR;
  node->args = calloc(2, sizeof(char *));
  node->args[0] = calloc(strlen(OR) + 1, 1);
  strcpy(node->args[0], OR); 
  add_node(&tree, node);
 
  
  node = calloc(1, sizeof(Node));
  node->type = OPERATOR;
  node->args = calloc(2, sizeof(char *));
  node->args[0] = calloc(strlen(AND) + 1, 1);
  strcpy(node->args[0], AND);
  add_node(&tree, node);
  
  node = calloc(1, sizeof(Node));
  node->type = OPERATOR;
  node->args = calloc(2, sizeof(char *));
  node->args[0] = calloc(strlen(PIPE) + 1, 1);
  strcpy(node->args[0], PIPE);
  add_node(&tree, node);
  
  node = calloc(1, sizeof(Node));
  node->type = COMMAND;
  node->args = calloc(2, sizeof(char *));
  node->args[0] = calloc(strlen("ls") + 1, 1);
  strcpy(node->args[0], "ls");
  add_node(&tree, node);

  node = calloc(1, sizeof(Node));
  node->type = COMMAND;
  node->args = calloc(2, sizeof(char *));
  node->args[0] = calloc(strlen(SUBSHELL) + 1, 1);
  strcpy(node->args[0], "()");
  add_node(&tree, node);

  node = calloc(1, sizeof(Node));
  node->type = COMMAND;
  node->args = calloc(2, sizeof(char *));
  node->args[0] = calloc(strlen("grep") + 1, 1);
  strcpy(node->args[0], "grep");
  add_node(&tree, node);

  node = calloc(1, sizeof(Node));
  node->type = COMMAND;
  node->args = calloc(2, sizeof(char *));
  node->args[0] = calloc(strlen("ps") + 1, 1);
  strcpy(node->args[0], "ps");
  add_node(&tree, node);
  
  node = calloc(1, sizeof(Node));
  node->type = COMMAND;
  node->args = calloc(2, sizeof(char *));
  node->args[0] = calloc(strlen("nano") + 1, 1);
  strcpy(node->args[0], "nano");
  node->input = calloc(20, 1);
  node->output = calloc(20, 1);
  strcpy(node->input, "input.txt");
  strcpy(node->output, "output.txt");
  add_node(&tree, node);

  print_tree(&tree);
  destroy_tree(&tree);
}

/* This function tests the add_node 
   function defined in tree.c */
void test_add_node2(void) {
  Node *node;
  Tree tree;

  printf("test_add_node2:\n");

  init_tree(&tree);
  
  node = calloc(1, sizeof(Node));
  node->type = COMMAND;
  node->args = calloc(2, sizeof(char *));
  node->args[0] = calloc(strlen("()") + 1, 1);
  strcpy(node->args[0], "()");
  add_node(&tree, node);

  node = calloc(1, sizeof(Node));
  node->type = OPERATOR;
  node->args = calloc(2, sizeof(char *));
  node->args[0] = calloc(strlen("&&") + 1, 1);
  strcpy(node->args[0], "&&");
  add_node(&tree, node);

  node = calloc(1, sizeof(Node));
  node->type = COMMAND;
  node->args = calloc(2, sizeof(char *));
  node->args[0] = calloc(strlen("ls") + 1, 1);
  strcpy(node->args[0], "ls");
  add_node(&tree, node);

  node = calloc(1, sizeof(Node));
  node->type = OPERATOR;
  node->args = calloc(2, sizeof(char *));
  node->args[0] = calloc(strlen("&&") + 1, 1);
  strcpy(node->args[0], "&&");
  add_node(&tree, node);

  node = calloc(1, sizeof(Node));
  node->type = COMMAND;
  node->args = calloc(2, sizeof(char *));
  node->args[0] = calloc(strlen("nano") + 1, 1);
  strcpy(node->args[0], "nano");
  add_node(&tree, node);

  node = calloc(1, sizeof(Node));
  node->type = COMMAND;
  node->args = calloc(2, sizeof(char *));
  node->args[0] = calloc(strlen("()") + 1, 1);
  strcpy(node->args[0], "()");
  add_node(&tree, node);

  node = calloc(1, sizeof(Node));
  node->type = OPERATOR;
  node->args = calloc(2, sizeof(char *));
  node->args[0] = calloc(strlen("|") + 1, 1);
  strcpy(node->args[0], "|");
  add_node(&tree, node);

  node = calloc(1, sizeof(Node));
  node->type = COMMAND;
  node->args = calloc(2, sizeof(char *));
  node->args[0] = calloc(strlen("echo") + 1, 1);
  strcpy(node->args[0], "echo");
  add_node(&tree, node);

  node = calloc(1, sizeof(Node));
  node->type = COMMAND;
  node->args = calloc(2, sizeof(char *));
  node->args[0] = calloc(strlen("grep") + 1, 1);
  strcpy(node->args[0], "grep");
  add_node(&tree, node);

  print_tree(&tree);
  destroy_tree(&tree);
}

/* This function tests the add_node 
   function defined in tree.c */
void test_add_node3(void) {
  Node *node;
  Tree tree;

  printf("test_add_node3:\n");

  init_tree(&tree);
  
  node = calloc(1, sizeof(Node));
  node->type = OPERATOR;
  node->args = calloc(2, sizeof(char *));
  node->args[0] = calloc(strlen("|") + 1, 1);
  strcpy(node->args[0], "|");
  add_node(&tree, node);

  node = calloc(1, sizeof(Node));
  node->type = COMMAND;
  node->args = calloc(2, sizeof(char *));
  node->args[0] = calloc(strlen("()") + 1, 1);
  strcpy(node->args[0], "()");
  add_node(&tree, node);

  node = calloc(1, sizeof(Node));
  node->type = OPERATOR;
  node->args = calloc(2, sizeof(char *));
  node->args[0] = calloc(strlen(PIPE) + 1, 1);
  strcpy(node->args[0], PIPE); 
  add_node(&tree, node);

  node = calloc(1, sizeof(Node));
  node->type = OPERATOR;
  node->args = calloc(2, sizeof(char *));
  node->args[0] = calloc(strlen("&&") + 1, 1);
  strcpy(node->args[0], "&&");
  add_node(&tree, node);

  node = calloc(1, sizeof(Node));
  node->type = COMMAND;
  node->args = calloc(2, sizeof(char *));
  node->args[0] = calloc(strlen("ls") + 1, 1);
  strcpy(node->args[0], "ls");
  add_node(&tree, node);

  node = calloc(1, sizeof(Node));
  node->type = COMMAND;
  node->args = calloc(2, sizeof(char *));
  node->args[0] = calloc(strlen("grep") + 1, 1);
  strcpy(node->args[0], "grep");
  add_node(&tree, node);

  node = calloc(1, sizeof(Node));
  node->type = COMMAND;
  node->args = calloc(2, sizeof(char *));
  node->args[0] = calloc(strlen("nano") + 1, 1);
  strcpy(node->args[0], "nano");
  add_node(&tree, node);

  node = calloc(1, sizeof(Node));
  node->type = COMMAND;
  node->args = calloc(2, sizeof(char *));
  node->args[0] = calloc(strlen("()") + 1, 1);
  strcpy(node->args[0], "()");
  add_node(&tree, node);

  node = calloc(1, sizeof(Node));
  node->type = COMMAND;
  node->args = calloc(2, sizeof(char *));
  node->args[0] = calloc(strlen("grep") + 1, 1);
  strcpy(node->args[0], "grep");
  add_node(&tree, node);

  print_tree(&tree);
  destroy_tree(&tree);
}
/*********** END of tree test **********/

/*********** START of parser test ******/

void test_parse(void) {
  Tree *tree = NULL;
  
  printf("test_parse:\n");
  tree = parse();
  print_tree(tree);
  destroy_tree(tree);
  free(tree);
}

/*********** END of parser test *******/

int main(void) {

  test_init_stack();
  test_push();
  test_pop();
  
  test_add_node1();
  test_add_node2();
  test_add_node3();
  test_parse();
  
  exit(EXIT_SUCCESS);
}
