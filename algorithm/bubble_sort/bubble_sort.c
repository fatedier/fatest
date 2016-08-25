#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

#define NUMBER 20
#define RANDMIN 0
#define RANDMAX 1000

#ifndef true
typedef char bool;
#define true 1
#define false 0
#endif

void swap(int *a, int *b)
{
    if (a == b)
        return;
    *a = (*a) ^ (*b);
    *b = (*a) ^ (*b);
    *a = (*a) ^ (*b);
}

void bubble_sort(int a[], int length)
{
    for (int i=0; i<length; i++) {
        for (int k = 0; k < length - i; k++) {
            if (a[k] > a[k + 1]) {
                swap(&a[k], &a[k+1]);
            }
        }
    }
    return;
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

    bubble_sort(a, NUMBER);

    for (i=0; i<NUMBER; i++) {
        printf("%d ", a[i]);
    }
    printf("\n");
    return 0;
}
