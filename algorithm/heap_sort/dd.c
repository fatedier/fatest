#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

#define NUMBER 50
#define RANDMIN 0
#define RANDMAX 1000

#ifndef true
typedef char bool;
#define true 1
#define false 0
#endif

void swap(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

void heap_adjust(int a[], int n, int size)
{
    int lchild = 2 * n + 1;
    int rchild = 2 * n + 2;

    int max = n;

    if (n <= (size-2)/2)
    {
        if (lchild <= (size-1) && a[lchild] > a[max])
            max = lchild;
        if (rchild <= (size-1) && a[rchild] > a[max])
            max = rchild;

        if (max != n) {
            swap(a+n, a+max);
            heap_adjust(a, max, size);
        }
    }
}

void build_heap(int a[], int size)
{
    int i;
    for (i=((size-1)/2-1); i>=0; i--) {
        heap_adjust(a, i, size);
    }
}

void heap_sort(int a[], int size)
{
    int i;
    build_heap(a, size);
    for (i=size-1; i>=0; i--) {
        swap(a+i, a);
        heap_adjust(a, 0, i);
    }
}

int main(int argc, char **argv)
{
    int a[NUMBER];
    srand(time(NULL));
    int i;
    for (i=0; i<NUMBER; i++) {
        a[i] = rand() % (RANDMAX - RANDMIN) + RANDMIN;
        printf("%d ", a[i]);
    }
    printf("\n");

    heap_sort(a, NUMBER);
    for (i=0; i<NUMBER; i++) {
        printf("%d ", a[i]);
    }
    printf("\n");

    return 0;
}
