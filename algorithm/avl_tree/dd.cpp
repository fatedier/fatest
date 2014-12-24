#include <iostream>
#include <stdio.h>
#include <string.h>
#include "avl_tree.h"
using namespace std;

int main(int argc, char **argv)
{
    int a[5] = {5, 20, 14, 2, 1};

    AvlTree t;
    for (int i=0; i<5; i++)
    {
        t.insert(a[i]);
        t.print_tree();
        printf("\n");
    }

    cout << t.find(20) << endl;
    cout << t.find(11) << endl;
    return 0;
}
