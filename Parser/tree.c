/* Name: tree.c
   Purpose: This file defines methods that helps
   a user manage a binary Tree.
   Author: Olasubomi Efuniyi
   Date: 08|13|2018
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tree.h"
#include "stack.h"

#define STACK_SIZE 20 
static Node *add_node_aux(Tree *tree, Node *node, Stack *stack);

Node *add_node(Tree *tree, Node *node) {
  Stack stack;
  init_stack(&stack, STACK_SIZE);
  return add_node_aux(tree, node, &stack);
}

/* This function aids the recursive call of add_node. */
static Node *add_node_aux(Tree *tree, Node *node, Stack *stack) {
  if(tree != NULL && node != NULL) {
    if((tree->root) == NULL) {
      /* First node to be added to the tree */
      (tree->root) = node;
      return node;
    } else if((tree->root->type == COMMAND &&
	      strcmp("()", tree->root->args[0]) != 0) ||
	      (strcmp("()", tree->root->args[0]) == 0 &&
	       tree->left)) {
      /* The root of this tree is a COMMAND node and 
         is not a subshell command or it represents 
         a subshell which already has its maximum
         number of subtrees, 1. */
      /* Get the last overlooked right subtree */
      Tree *t = pop(stack);

      if(t == NULL) {
	/* The stack is empty */
	fprintf(stderr, "Error: Tree structure incorrect\n");
	return NULL;
      }

      if((t->right = calloc(1, sizeof(Tree))) == NULL) {
	perror("Memory allocation failed");
	return NULL;
      }

      /* Attempt to add the node to the last overlooked right
         subtree */
      return add_node_aux(t->right, node, stack);
    } else if(!(tree -> left) && !(tree -> right)) {
      /* The new node can be added to either the left or the right.
         This method completes the left subtree before moving on
         to the right subtree. */
      /* Dynamically allocate a Tree and make it
         the left subtree of the current tree in 
	 question */
      (tree -> left) = calloc(1, sizeof(Tree));
      return add_node_aux((tree->left), node, stack); 
    } else if((tree -> left) && !(tree -> right)) {
      /* Determine if the root of the left subtree is a 
         leaf or not */
      if(tree->left->root && strcmp("()", tree->left->root->args[0]) != 0
	 && (tree->left->root->type) == COMMAND) {	
	 /* The root of the left subtree is a leaf. Subshells
	    are not treated the same as other commands in this 
	    case. */
	 (tree->right) = calloc(1, sizeof(Tree));
	 return add_node_aux((tree->right), node, stack);
      } else if((tree->left->root->type) == OPERATOR ||
	       (tree->left->root && strcmp("()", tree->left->root->args[0]) == 0)) {	
	/* The root of the left subtree is not a leaf. Subshells 
	   are treated the same as operators in this case. */
	/* Save the tree so that you can potentially access the right 
           subtree down the line. Don't save tree if its root is
           a subshell. */
	if(strcmp("()", tree->root->args[0]) != 0) {
	  push(stack, tree);
	}
	/* Attempt to add the node to the left subtree first
           and overlook the right subtree */
	return add_node_aux((tree->left), node, stack);
      } else {
	fprintf(stderr, "Error: Invalid node type encountered\n");
	return NULL;
      }
    } else if((tree->left) && (tree->right)) {
      /* Both left and right subtree are non null. Add to the right
         subtree since this implies that the left subtree is complete */
      return add_node_aux((tree->right), node, stack);
    } 
  }

  return NULL;
}

void print_tree(Tree *tree) {
  if(tree != NULL) {
    /* Print out the data in the left subtree */
    print_tree(tree->left);
    /* Print out the data in the node of the root */
    if(tree->root != NULL) { 
      printf("NAME: %s, TYPE: %s ", tree->root->args[0],
	    (tree->root->type) == COMMAND ? "COMMAND" : "OPERATOR");
      if(tree->root->input) {
	printf("INPUT: %s ", tree->root->input);
      }

      if(tree->root->output) {
	printf("OUTPUT: %s ", tree->root->output);
      }

      printf("\n");
    }
    /* Print out the data in the right subtree */
    print_tree(tree->right);
  }
}
