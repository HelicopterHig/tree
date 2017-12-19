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

        else if (!strcmp(command, "rem")){
            scanf("%s", command);
            if (!strcmp(command, "whole")){
                removetree(tree);
                tree = 0;
                printf("The tree was delete\n");
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
                    printf("Not correct item");
                }
                else if (flag == 10){
                    sscanf(command, "%d", &numtos);
                    scanf("%s", command);
                    if (!strcmp(command, "self")){
                        if (!rmel(tree, numtos, SELF)) printf("Element not found or has descendant\n");
                    }
                    else if (!strcmp(command, "sibling")){
                        if (!rmel(tree, numtos, SIBLING)) printf("Element not found, has no sibling\n");
                    }
                    else if (!strcmp(command, "descendants")){
                        if (!rmel(tree, numtos, DESCENDANTS)) printf("Element not found\n");
                    }
                    else if (!strcmp(command, "family")){
                        if (!rmel(tree, numtos, FAMILY)) printf("Element not found");
                    }
                    else
                        printf("%s", "False command\n");
                }
                else
                    printf("UNKNOWN ERROR\n");
                flag = 8;
            }
            else printf("%s", "False command\n");
        }
        else if (!strcmp(command, "save")){
            fgets(command, sizeof(command), stdin);
            c = command;
            while((*(c))!='\n') ++c;
            *c = '\0';
            if ((file = fopen(command, "wb"))!= 0){
                writenode(tree, file);
                fclose(file);
            }
            else
                printf("Error creating file\n");
        }
        else if (!strcmp(command, "load")){
            fgets(command, sizeof(command), stdin);
            c = command;
            while ((*(c))!='\n') ++c;
            *c = '\0';
            if ((file = fopen(command, "rb"))!= 0){
                if (tree){
                    removetree(tree);
                    tree = 0;
                }
                fread(&numfee, sizeof(int), 1, file);
                tree = createroot(numfee);
                loadnode(tree, file);
                statistics(tree);
                maxdepth = maximaldepth(tree, 0);
                fclose(file);
            }
            else
                printf("File not\n");
                checkprint(tree, 0);
                printf("items of nodes: %d. \nitems of internal nodes: %d.\nitem of leaves: %d.\nMaximal depth of tree: %d.\n", nodesnum,interiornum, lsumm, maxdepth);

        }
        else if (!strcmp(command, "help")){
            printf("HELP");
            fflush(stdin);
            getchar();
        }

        else if (!strcmp(command, "show")){
            checkprint(tree, 0);
            printf("items of nodes: %d. \nitems of internal nodes: %d.\nitem of leaves: %d.\nMaximal depth of tree: %d.\n", nodesnum,interiornum, lsumm, maxdepth);

        }
        else if (!strcmp(command, "exit"))
            return 0;
        else{
            printf("%s", "False command\n");
        }
        fflush(stdin);
    }

    return 0;
}

int rmel(node *tree, int numtos, char typedel){
    static int flag;
    int tmp = 0;
    if (tree){
        if (typedel == SELF){
            if (!flag)
                if (tree->left){
                    if (tree->left->item == numtos){
                        if ((tree->left->left == 0)&&(tree->left->right ==0)){
                            tmp = flag = 1;
                            free(tree->left);
                            tree->left = 0;
                        }
                    }
                }
            if (!flag)
                if (tree->right){
                    if (tree->right->item == numtos){
                        if ((tree->right->left == 0)&&(tree->right->right == 0)){
                            tmp = flag = 1;
                            free(tree->right);
                            tree->right = 0;
                        }
                    }
                }
        }

        else if (typedel == SIBLING){
            if (!flag)
                if (tree->left){
                    if (tree->left->item == numtos){
                        if (tree->right)
                            if ((tree->right->left == 0)&&(tree->right->right == 0)){
                                tmp = flag = 1;
                                free(tree->right);
                                tree->right = 0;
                            }
                    }
                }
            if (!flag)
                if (tree->right){
                    if (tree->right->item == numtos){
                        if (tree->left)
                            if ((tree->left->left == 0)&&(tree->left->right == 0)){
                                tmp = flag = 1;
                                free(tree->left);
                                tree->left = 0;
                            }
                    }
                }
        }

        else if (typedel == DESCENDANTS){
            if (!flag)
                if (tree->left){
                    if (tree->left->item == numtos){
                        if ((tree->left->left)){
                            removetree(tree->left->left);
                            tree->left->left = 0;
                        }
                        if ((tree->left->right)){
                            removetree(tree->left->right);
                            tree->left->right = 0;
                        }
                        tmp = flag = 1;
                    }
                }
            if (!flag)
                if (tree->right){
                    if (tree->right->item == numtos){
                        if ((tree->right->left)){
                            removetree(tree->right->left);
                            tree->right->left = 0;
                        }
                        if ((tree->right->right)){
                            removetree(tree->right->right);
                            tree->right->right = 0;
                        }
                        tmp = flag = 1;
                    }
                }
        }

        else if (typedel == FAMILY){
            if (!flag)
                if (tree->left){
                    if (tree->left->item == numtos){
                        removetree(tree->left);
                        tree->left = 0;

                        tmp = flag =1;
                    }
                }
            if (!flag)
                if (tree->right){
                    if (tree->right->item == numtos){
                        removetree(tree->right);
                        tree->right = 0;
                        tmp = flag = 1;
                    }
                }
        }
        else
            printf("False 3 parameter of removeelemnt\n");
    }
    else
        return 0;
    if (!flag) tmp = rmel(tree->left, numtos, typedel);
    if (!flag) tmp = rmel(tree->right, numtos, typedel);
    if (!tree->parent) flag =0;
    return tmp;
}

void removetree(node *tree){
    if (!tree) return;
    if (tree->left)
        removetree(tree->left);
    if (tree->right)
        removetree(tree->right);
    free(tree);
}

void loadnode(node *tree, FILE *file){
    int targetn, item;
    if (!fread(&targetn, sizeof(int), 1, file)) return;
    if (!fread(&item, sizeof(int), 1, file)) return;
    addnode(item, tree, targetn, PARENT, 1);
    loadnode(tree, file);
}

void writenode(node *tree, FILE *file){
    if (tree->parent){
        fwrite(&tree->parent->item, sizeof(int), 1, file);
        fwrite(&tree->item, sizeof(int), 1, file);
    }
    else {
        fwrite(&tree->item, sizeof(int), 1, file);
    }
    if (tree->left) writenode(tree->left, file);
    if (tree->right) writenode(tree->right, file);
}

int fheap(node *tree, int curdepth){
    extern int maxdepth;
    static int flag, flage1;
    int tmp = 0;
    ++curdepth;
    if (tree){
        if ((!flag))
            if (tree->parent)
                if ((tree->parent->item)<(tree->item))
                    flag = tmp = 1;
        if (!flag)
            if ((curdepth+1)<(maxdepth)){
                if (!((tree->left)&&(tree->right)))
                    tmp = flag = 1;
            }
        if (curdepth == maxdepth)
            if (flage1)
                tmp = flag = 1;
        if (!flag)
            tmp = fheap(tree->left, curdepth);

        if (!flag)
            tmp = fheap(tree->right, curdepth);
    }

    else{
        if (!flag)
            if (curdepth == maxdepth)
                flage1 = 1;
    }
    if (curdepth == 1){
        flag = 0;
        flage1 = 0;
    }
    return tmp;
}

int maximaldepth(node *tree, int n){
    int a = 0, b = 0;
    if (tree){
        ++n;
        a = maximaldepth(tree->left, n);
        b = maximaldepth(tree->right, n);
        if (a>n) n=a;
        if (b>n) n=b;
    }

    return n;
}

void statistics(node *tree){
    extern int lsumm, interiornum, nodesnum;
    if (tree){
        ++nodesnum;
        if ((tree->right)||(tree->left))
            ++interiornum;
        else
            ++lsumm;
        if (tree->right)
            statistics(tree->right);
        if (tree->left)
            statistics(tree->left);
    }
}

node* createroot(int item){
    node *temp;
    temp = (node*)malloc(sizeof(node));
    temp->left = temp->right = temp->parent = 0;
    temp->item = item;
    return temp;
}

int addnode(int item, node *current, int target, char type, int launchflag){
    static int flag;
    int ntmp = 0;
    node *temp;

    if (flag) return 1;
    if (!current){
        printf("node is empty\n");
        return 0;
    }

    if (type == PARENT){
        if (current->item == target){
            if (current->left == 0){
                temp = (node*)malloc(sizeof(node));
                temp->left = temp->right = 0;
                temp->parent = current;
                current->left = temp;
                temp->item = item;
                flag = ntmp = 1;
            }
            else if (current->right == 0){
                temp = (node*)malloc(sizeof(node));
                temp->left = temp->right = 0;
                temp->parent = current;
                current->right = temp;
                temp->item = item;
                flag = ntmp = 1;
            }
        }

        if (!flag){
            if (current->left){
                if (current->right){
                    ntmp = addnode(item, current->left, target, PARENT, 0);
                    if (!flag) ntmp = addnode(item, current->right, target, PARENT, 0);
                }
                else {
                    ntmp = addnode(item, current->left, target, PARENT, 0);
                }
            }
            else {
                if (current->right)
                    ntmp = addnode(item, current->right, target, PARENT, 0);
            }
        }
    }

    else if (type == SIBLING){
        if (current->left){
            if (current->right){
                ntmp = addnode(item, current->left, target, SIBLING, 0);
                if (!flag) ntmp = addnode(item, current->right, target, SIBLING, 0);
            }
            else{
                if (current->left->item == target){
                    temp = (node*)malloc(sizeof(node));
                    temp->left = temp->right = 0;
                    temp->parent = current;
                    current->right = temp;
                    temp->item = item;
                    flag = ntmp = 1;
                }
                else{
                    ntmp = addnode(item, current->left, target, SIBLING, 0);
                }
            }
        }
        else if (current->right){
            if (current->left){
                ntmp = addnode(item, current->left, target, SIBLING, 0);
                if (!flag) ntmp = addnode(item, current->right, target, SIBLING, 0);
            }
            else {
                if (current->right->item == target){
                    temp = (node*)malloc(sizeof(node));
                    temp->left = temp->right = 0;
                    temp->parent = current;
                    current->left = temp;
                    temp->item = item;
                    flag = ntmp = 1;
                }
                else {
                    ntmp = addnode(item, current->right, target, SIBLING, 0);
                }
            }
        }
        else
            return 0;
    }
    else {
        printf("%s", "False 4 argument of addnode\n");
        return 0;
    }
    if ((flag)&&(launchflag)) flag = 0;
    return (ntmp);
}

void printmas(node *tree){
    if (tree){
        printmas(tree->left);
        printf("%d", tree->item);
        printmas(tree->right);
    }
}

void checkprint(node *tree, int n){
    int i;
    if (tree){
        checkprint(tree->right, n+3);
        for (i=0; i<n; i++)
            putchar(' ');
        printf("%d\n", tree->item);
        checkprint(tree->left, n+3);
    }
}
