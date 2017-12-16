 #include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct node{
    struct node *parent;
    struct node *right;
    struct node *left;
    int item;
} node;

node* createroot(int item);
int addnode(int item, node* current, int target, char type, int launchflag);
void checkprint(node *tree, int n);
void printmas(node *tree);
void statistics(node *tree);
int maximaldepth(node *tree, int n);
int fheap(node *tree, int curdepth);
void writenode(node *tree, FILE *file);
void loadnode(node *tree, FILE *file);
void removetree(node *tree);
int rmel(node *tree, int numtos, char typedel);

#define PARENT 1
#define SIBLING 2
#define SELF 3
#define DESCENDANTS 4
#define FAMILY 5

int lsumm, interiornum, nodesnum, heap, maxdepth;


int main(int argc, char *argv[])
{
    int i, numfee, numtos;
    node *tree = 0;
    FILE *file = 0;
    char command[500],flag = 8, *c=0;

    if (argc > 1){
        if ((file = fopen(argv[1], "rb")) == 0)
            printf("Error opening\n");
        else {
            fread(&numfee, sizeof(int), 1, file);
            tree = createroot(numfee);
            loadnode(tree, file);
            fclose(file);
        }
    }

    else
        printf("No trees were loaded from the command line arguments\n");

    while (flag){
        lsumm = interiornum = nodesnum = maxdepth = 0;
        statistics(tree);
        maxdepth = maximaldepth(tree, 0);
        heap = fheap(tree, 0);
        scanf("%s", command);

        if (!strcmp(command, "add")){
            scanf("%s", command);
            if (!strcmp(command, "root")){
                if (tree == 0){
                    flag = 10;
                    scanf("%s", command);
                    i=0;
                    while (command[i]){
                        if ((!(command[i]>='0'))||(!(command[i]<='9')))
                            flag = 2;
                        ++i;
                    }

                    if (flag == 2){
                        printf("Not correct item\n");
                    }
                    else if (flag == 10){
                        sscanf(command, "%d", &numfee);
                        tree = createroot(numfee);
                    }
                    flag = 8;
                }
                else
                    printf("Root already exists\n");
            }

            else if (!strcmp(command, "node")){
                flag = 10;
                scanf("%s", command);
                i = 0;
                while (command[i]){
                    if ((!(command[i]>='0'))||(!(command[i]<='9')))
                        flag = 2;
                    ++i;
                }

                if (flag == 2){
                    printf("Not correct items\n");
                }
                else if (flag == 10){
                    sscanf(command, "%d", &numfee);
                    scanf("%s", command);
                    if (!strcmp(command, "parent")){
                        scanf("%s", command);
                        if (!strcmp(command, "root")){
                            if (tree){
                                if (!tree->left){
                                    tree->left = createroot(numfee);
                                    tree->left->parent = tree;
                                }
                                else if (!tree->right){
                                    tree->right = createroot(numfee);
                                    tree->right->parent = tree;
                                }
                                else
                                    printf("%s", "Root has no free descendants\n");
                            }
                            else
                                printf("There is no root\n");
                        }
                        else{
                            flag = 10;
                            i = 0;
                            while (command[i]){
                                if ((!(command[i]>='0'))||(!(command[i]<='9')))
                                    flag = 2;
                                ++i;
                            }
                            if (flag == 2){
                                printf("Not correct item\n");
                            }
                            else if (flag == 10){
                                sscanf(command, "%d", &numtos);
                                if (addnode(numfee, tree, numtos, PARENT, 1));
                                else printf("No element with items %d or it already has 2 descendants\n", numtos);
                            }
                        }
                    }
                    else if (!strcmp(command, "sibling")){
                        scanf("%s", command);
                        flag = 10;
                        i = 0;
                        while (command[i]){
                            if ((!(command[i]>='0'))||(!(command[i]<='9')))
                                flag = 2;
                            ++i;
                        }
                        if (flag == 2){
                            printf("Not correct items\n");
                        }
                        else if (flag == 10){
                            sscanf(command, "%d", &numtos);
                            if (addnode(numfee, tree, numtos, SIBLING, 1));
                            else printf("No element with item %d or it already has a sibling\n", numtos);
                        }
                    }
                    else
                        printf("%s", "False command\n");
                    flag = 8;
                }
                flag = 8;
            }
            else
                printf("%s", "False commad\n");
        }
