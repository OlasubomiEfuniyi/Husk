#ifndef STACK_H
#define STACK_H
#define SUCCESS 0
#define FAILURE 1
#define TRUE 1
#define FALSE 0

typedef struct stack {
  /* Data is an array of pointers */ 
  void **data;
  /* Top is the index of the next available
     position on the stack. */ 
  int top;
  /* Size is the number of elements in the array */
  int size;
} Stack;

/* This function initializes a Stack structure
   by dynamically allocating an array of pointers
   and setting the top pf the stack to 0. */
void init_stack(Stack *stack, int num_elems);

/* This function adds a pointer to a data data 
   to the top of the stack. The function returns 
   SUCCESS if addition was successful and FAILURE
   otherwise. The effect of pushing to an uninitialized 
   stack is undefined. */ 
int push(Stack *stack, void *data);

/* This function removes the value at the top of the
   stack. It returns the popped value if successful and 
   NULL if it was unable to pop a value. */
void *pop(Stack *stack);

/* This function returns TRUE if the stack is full and FALSE 
   otherwise. */
int is_full(Stack *stack);

/* This function returns TRUE if the stack is empty and FALSE
   otherwise. */
int is_empty(Stack *stack);

/* This function deallocates any dynamically allocated memory associated
   with a stack. */
void destroy_stack(Stack *stack);
#endif
