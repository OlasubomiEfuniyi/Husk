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

Node *add_node(Tree *tree, Node *node) {
  if(tree != NULL && node != NULL) {
    if((tree->root) == NULL) {
      /* First node to be added to the tree */
      (tree->root) = node;
    } else if(!(tree -> left) && !(tree -> right)) {
      /* The new node can be added to either the left or the right.
         This method completes the left subtree before moving on
         to the right subtree. */
      /* Dynamically allocate a Tree and make it
         the left subtree of the current tree in 
	 question */
      (tree -> left) = calloc(1, sizeof(Tree));
      add_node((tree->left), node); 
    } else if((tree -> left) && !(tree -> right)) {
      /* Determine if the root of the left subtree is a 
         leaf or not */
      if((tree->left->root->type) == COMMAND) {	
	if(tree->left->root && strcmp("()", tree->left->root->args[0]) != 0) {
	  /* The root of the left subtree is a leaf. Subshells
	     are not treated the same as other commands in this 
	     case. */
	  (tree->right) = calloc(1, sizeof(Tree));
	  add_node((tree->right), node);
	}
      } else if((tree->left->root->type) == OPERATOR ||
	       (tree->left->root && strcmp("()", tree->left->root->args[0]) == 0)) {	
	/* The root of the left subtree is not a leaf. Subshells 
	   are treated the same as operators in this case. */
	add_node((tree->left), node);
      } else {
	fprintf(stderr, "Error: Invalid node type encountered\n");
	return NULL;
      }
    } else if((tree->left) && (tree->right)) {
      /* Both left and right subtree are non null. Add to the right
         subtree since this implies that the left subtree is complete */
      add_node((tree->right), node);
    } 
  }

  return node;
}

void print_tree(Tree *tree) {
  if(tree != NULL) {
    /* Print out the data in the left subtree */
    print_tree(tree->left);
    /* Print out the data in the node of the root */
    if(tree->root != NULL) { 
      printf("Name: %s, Type: %s\n", tree->root->args[0],
	    (tree->root->type) == COMMAND ? "COMMAND" : "OPERATOR");
    }
    /* Print out the data in the right subtree */
    print_tree(tree->right);
  }
}
