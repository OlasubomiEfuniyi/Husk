#ifndef TREE_H
#define TREE_H
#define MAX_LEN 80

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

/* This function adds a node to a tree. A node is added in the 
   next open position to make the tree as complete and balanced 
   as possible. The function returns a pointer to the last node 
   added to the Tree if successful and NULL if not, including 
   when any error which can be recovered from is encountered. */    
Node *add_node(Tree * tree, Node *node);

/* This function prints the information saved within a tree. It 
   does so by performing a preorder traversal. */ 
void print_tree(Tree *tree);
#endif
