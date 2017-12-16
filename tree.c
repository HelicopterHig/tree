#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct joint{
    struct joint *parent;
    struct joint *right;
    struct joint *left;
    int item;
} joint;
