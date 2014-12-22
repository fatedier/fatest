#include <iostream>
#include <stdio.h>
#include <string.h>
#include "avl_tree.h"
using namespace std;

int main(int argc, char **argv)
{
    AvlTree avl;
    avl.insert(5);

    avl.print_tree();
    printf("\n");

    avl.insert(20);

    avl.print_tree();
    printf("\n");

    avl.insert(14);
    avl.print_tree();
    printf("\n");
    
    avl.insert(2);

    avl.print_tree();
    printf("\n");

    avl.insert(1);
    avl.print_tree();
    printf("\n");
    return 0;
}
