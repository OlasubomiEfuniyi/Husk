#ifndef TREE_H
#define TREE_H
#define MAX_LEN 80

#define PIPE "|"
#define AND "&&"
#define OR "||"
#define SUBSHELL "()"
#define INPUT "<"
#define OUTPUT ">"

typedef enum type{OPERATOR, COMMAND} Type;

/* This structure represents a node in the tree */
typedef struct node {
  Type type;
  char *input;
  char *output;
  char **args; /* Null terminated array of argument strings */
} Node;

/* This  structure represents a tree */
typedef struct tree {
  Node *root;
  struct tree *left, *right;
} Tree;

/* This function properly initializes a tree and is
   intended to be called before any other tree.h function.
   This function resets any tree that contains data. */
void init_tree(Tree *tree);

/* This function adds a node to a tree. A node is added in the 
   next open position to make the tree as complete and balanced 
   as possible. The function returns a pointer to the last node 
   added to the Tree if successful and NULL if not, including 
   when any error which can be recovered from is encountered. 
   The effect of adding a node to an uninitialized tree is undefined.*/    
Node *add_node(Tree * tree, Node *node);

/* This function prints the information saved within a tree. It 
   does so by performing a preorder traversal. The effect of printing 
   an uninitialized tree is undefined. */ 
void print_tree(Tree *tree);
#endif
