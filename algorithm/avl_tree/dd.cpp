#include <iostream>
#include <stdio.h>
#include <string.h>
#include "avl_tree.h"
using namespace std;

int main(int argc, char **argv)
{
    AvlTree avl;
    avl.insert(5);
    avl.insert(20);
    avl.insert(14);
    avl.insert(2);
    avl.insert(100);
    avl.print_tree();
    printf("\n");
    return 0;
}
