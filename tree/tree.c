#include "tree.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

Node **targetFind(Tree *tree, char *key);
Node *crateNode(char *key, int value);
Node **findNext(Tree *tree, char *key, int number);
Node **findNode(Tree *tree, char *key, int number);
void freeNode(Node **node);
void work(Node *root, Node **out, int a, int b, int *iteration);

int insert(Tree *tree, char *key, int value) {
    Node **targetNode = targetFind(tree, key);
    
    *targetNode = malloc(sizeof(Node));
    if (!(*targetNode))
        return 0;
    *targetNode = crateNode(key, value);
    tree->size ++;
    return 1;

}

Node **targetFind(Tree *tree, char *key) {
    Node **ptr = &(tree->root);

    while (*ptr) {
        if (strcmp(key, (*ptr)->key) < 0)
            ptr = &((*ptr)->left);
        else
            ptr = &((*ptr)->right);
    }
    return ptr;
}

Node *crateNode(char *key, int value){
    Node* out = malloc(sizeof (Node));
    if (!out)
        return NULL;
    out->key = calloc(strlen(key) + 1, sizeof(char));
    strcpy(out->key, key);
    out->value = malloc(sizeof(int));
    *(out->value) = value;
    out->left = NULL;
    out->right = NULL;
    return out;
}

Node **findNext(Tree *tree, char *key, int number){
    Node **ptr = &(tree->root);
    int curNumber = 1;

    while (*ptr){
        if (strcmp(key, (*ptr)->key) < 0){
            if (!(*ptr)->left)
                return ptr;
            ptr = &(*ptr)->left;
        } else if (strcmp(key, (*ptr)->key) > 0)
            ptr = &((*ptr)->right);
        else {
            if (curNumber > number)
                return ptr;
            else {
                ptr = &(*ptr)->right;
                curNumber++;
            }
        }
    }
    return NULL;
}

Node **findNode(Tree *tree, char *key, int number){
    Node **out = findNext(tree, key, number - 1);

    if (strcmp((*out)->key, key) !=0)
        return NULL;
    return out;
}

void freeNode(Node **node){
    free((*node)->key);
    free((*node)->value);
    free(*node);
    *node = NULL;
}

int deleteNode(Tree *tree, char *key, int number){
    Node **curNode, **newNode, *buffer;

    curNode = findNode(tree, key, number);
    if (!curNode)
        return 0;
    if (!(*curNode)->left && !(*curNode)->right){
        freeNode(curNode);
    } else {
        newNode = findNext(tree, key, number);
        buffer = *newNode;
        *newNode = ((*newNode)->left)? (*newNode)->left : (*newNode)->right;
        buffer->left = (*curNode)->left;
        buffer->right = (*curNode)->right;
        freeNode(curNode);
        *curNode = buffer;
        tree->size--;
    }
    return 1;
}

int bypass(Tree *tree, int a, int b, Node ***out){
    int iteration = 0;
    *out = calloc(tree->size, sizeof (Node *));
    if (!(*out))
        return 0;
    work(tree->root, *out, a, b, &iteration);
    return iteration;
}

void work(Node *root, Node **out, int a, int b, int *iteration) {
    if (!root)
        return;
    work(root->left, out, a, b, iteration);
    if (*(root->value) < a || *(root->value) > b){
        out[*iteration] = root;
        (*iteration) ++;
    }
    work(root->right, out, a, b, iteration);
}

int count(Tree *tree, char *key){
    Node **ptr = &(tree->root);
    int counter = 0;

    while (*ptr){
        if (strcmp(key, (*ptr)->key) < 0){
            ptr = &(*ptr)->left;
        } else {
            if (strcmp(key, (*ptr)->key) == 0)
                counter++;
            ptr = &(*ptr)->right;
        }
    }
    return counter;
}

Node *findMin(Tree *tree){
    Node *ptr = tree->root;
    if (!ptr)
        return NULL;
    while (ptr->left)
        ptr = ptr->left;
    return ptr;
}

void printTree(Node *ptr, int level){
    int i = level;
    if (ptr){
        level += 1 + (int) strlen(ptr->key);
        printTree(ptr->right, level);
        while (i-- > 0)
            printf(" ");
        printf("%s\n", ptr->key);
        printTree(ptr->left, level);
    }
}