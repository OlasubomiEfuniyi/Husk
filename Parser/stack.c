/* Name: stack.c
   Purpose: This file provides functions that helps a user
            manipulate an array as a stack.
   Author: Olasubomi Efuniyi
   Date: 08|15|2018
*/

#include <stdio.h>
#include <stdlib.h>
#include "stack.h"

void init_stack(Stack *stack, int num_elems) {
  if(stack != NULL) {
    if((stack->data = calloc(num_elems, sizeof(void *))) == NULL) {
      perror("Memory allocation failed");
    }
    stack->top = 0;
    stack->size = num_elems;
  }
}

int push(Stack *stack, void *data) {
  if(stack && stack->data) {
    /* stack is not NULL and a data array exists */
    if(stack->top < stack->size) {
      /* stack is not full */
      (stack->data)[(stack->top)++] = data;
      return SUCCESS;
    }
    return FAILURE;
  }
  return FAILURE;
}

void *pop(Stack *stack) {
  if(stack && stack->data) {
    /* stack is not null and a data array is present
       within the structure */
    if(stack->top > 0) {
      /* Stack is not empty */
      (stack->top)--;
      return (stack->data)[stack->top];
    }
    return NULL;
  }

  return NULL;
}

int is_full(Stack *stack) {
  return(stack->top == stack->size);
}

int is_empty(Stack *stack) {
  return(stack->top == 0);
}

void destroy_stack(Stack *stack) {
  if(stack != NULL) {
    free(stack -> data);
  }
}
