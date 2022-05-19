#ifndef LAB4_TREE_H
#define LAB4_TREE_H

typedef struct Node {
    char *key;
    int *value;
    struct Node *left;
    struct Node *right;
} Node;

typedef struct Tree {
    Node *root;
    int size;
} Tree;



int insert(Tree *tree, char *key, int value);
int deleteNode(Tree *tree, char *key, int number);
Node **findNode(Tree *tree, char *key, int number);
int bypass(Tree *tree, int a, int b, Node ***out);
int count(Tree *tree, char *key);
Node *findMin(Tree *tree);
void printTree(Node *ptr, int level);

#endif //LAB4_TREE_H
