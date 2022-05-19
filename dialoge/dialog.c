#include <time.h>
#include "dialog.h"
#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "conio.h"

const char *dialogMsgs[] = {"0. Quit", "1. Add", "2. Bypass", "3. Find", "4. Delete", "5. Show", "6. Timing"};
const char *findMsgs[] = {"0. Back", "1. Key", "2. Min key"};
const char *addMsgs[] = {"0. Back", "1. Key", "2. File"};

void (*dialogFptr[])(Tree *) = {NULL, DAdd, DBypass, DFind, DDelete, DShow, DTiming};

int getInt() {
    int out, flag;
    char *error;
    error = "";
    do {
        flag = 1;
        puts(error);
        error = "Please enter number";
        if (!scanf("%d%*[ ]", &out))
            flag = 0;
        if (getchar() != '\n')
            flag = 0;
        fflush(stdin);
    } while (!flag);
    return out;
}

char *getStr() {
    char *s, *buf = (char *) calloc(sizeof(char), 256);
    int flag;
    char *error;
    error = "";
    do {
        flag = 1;
        puts(error);
        error = "Please enter string without spaces";
        if (!scanf("%s%*[ ]", buf))
            flag = 0;
        if (getchar() != '\n')
            flag = 0;
        fflush(stdin);
    } while (!flag);
    s = (char *) calloc(sizeof(char), strlen(buf) + 1);
    strcpy(s, buf);
    free(buf);
    buf = NULL;
    return s;
}

int getOption(int optionNum, char *msg, char *errMsg, char **options) {
    int rc, i;
    char *error = "";
    do {
        puts(error);
        error = errMsg;
        for (i = 0; i < optionNum; ++i)
            puts(options[i]);
        puts(msg);
        rc = getInt();
    } while (rc < 0 || rc >= optionNum);
    return rc;
}

void dialog(Tree *tree) {
    int rc;
    while ((rc = getOption(7, "Select the command",
                           "Wrong command number, repeat", (char **) dialogMsgs))) {
        dialogFptr[rc](tree);
        if (rc == 6)
            return;
    }
}

void DAdd(Tree *tree) {
    int option;
    option = getOption(3, "Select the add1 option",
                       "Wrong option number, repeat", (char **) addMsgs);
    if (!option)
        return;
    if (option == 1) {
        char *key;
        int value;
        puts("Enter key");
        key = getStr();
        puts("Enter value");
        value = getInt();
        if (!insert(tree, key, value))
            printf("Memory Error");
        free((key));
    } else {
        FILE *file;
        char *fileName, *key;
        int value;

        puts("Enter filename");
        fileName = getStr();
        file = fopen(fileName, "r");
        if (!file){
            puts("No such file");
            return;
        }

        key = calloc(256, sizeof (char ));
        while (fscanf(file, "%s%*[ ]", key) != EOF && fscanf(file, "%d%*[ ]", &value) != EOF)
            insert(tree, key, value);
        free(key);
        fclose(file);
    }
}

void DFind(Tree *tree) {
    int option;
    char *key;
    Node *node;

    option = getOption(3, "Select the search option",
                       "Wrong option number, repeat", (char **) findMsgs);
    if (!option)
        return;
    if (option == 1) {
        int number;

        puts("Enter the key");
        key = getStr();
        number = count(tree, key);
        if (number == 0)
            puts("Not found");
        else if (number == 1){
            node = * findNode(tree, key, 1);
            printf("(%s : %d)\n", node->key, *(node->value));
        } else {
            Node** out = calloc(sizeof (Node*), number);
            for (int i = 1; i <= number; i++){
                node = *findNode(tree, key, i);
                printf("(%s : %d)\n", node->key, *(node->value));
            }


        }
    } else if (option == 2) {
        node = findMin(tree);
        if (!node)
            printf("Tree is empty");
        else
            printf("(%s : %d)\n", node->key, *(node->value));
    }
}

void DBypass(Tree *tree){
    int a, b, i, number;
    Node** out;
    char *msg;
    puts("Enter boards");
    puts("Enter left board");
    a = getInt();
    msg= ("Enter right board");
    do {
        puts(msg);
        msg = "Right board must be bigger than left";
        b = getInt();
    } while (b <= a);
    number = bypass(tree, a, b, &out);
    if (number == 0)
        puts("Nothing was found");
    for (i = 0; i < number; i++)
        printf("(%s:%d)\n", out[i]->key, *(out[i]->value));
    free(out);
}

void DDelete(Tree *tree) {
    int number, option;
    char *key;

    puts("Enter the key");
    key = getStr();
    number = count(tree, key);
    if (number == 0)
        puts("Not found");
    else if (number == 1){
        deleteNode(tree, key, 1);
    } else {
        do {
            printf("Put number from 1 to %d\n", number);
            option = getInt();
        } while (option < 1 || option > number);
        deleteNode(tree, key, option);
    }
}

void DShow(Tree *tree) {
    if (tree->size == 0)
        puts("Tree is empty");
    else
        printTree(tree->root, 0);
}

void DTiming(Tree *tree) {
    puts("Step of nodes number: 1000; Search number: 10'000'000");
    Tree newTree = {NULL, 0};
    srand ( time(NULL) );
    for (int i = 0; i < 10000; i++) {
        char *key = malloc(4);
        int value = rand() % 1001;
        itoa(value, key, 10);
        insert(&newTree, key, value);
        free(key);
        if (i % 1000 == 0){
            clock_t oldTime = clock();
            for (int j = 0; j < 10000000; j++){
                key = malloc(4);
                value = rand() % 1001;
                itoa(value, key, 10);
                count(&newTree, key);
                free(key);
            }
            printf("%.3f\n", (float)(clock()-oldTime) / CLOCKS_PER_SEC);
        }

    }
}