#include <iostream>
#include <string>
#include <time.h>
#include <stdlib.h>
#include "rb_tree.h"
using namespace std;

#define NUMBER 1000000

int main()
{
    RbTree<int, int> rb;
    srand(time(NULL));
    
    
    int num;
    for (int i=0; i<NUMBER; i++)
    {
        num = rand() % (NUMBER * 10);
        rb.insert(num, 0);
    }

    return 0;
}
