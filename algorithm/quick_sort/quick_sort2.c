#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>

#define NUMBER 20
#define RANDMIN 0
#define RANDMAX 1000

void swap(int *a, int *b)
{
    if (a == b)
        return;
    *a = (*a) ^ (*b);
    *b = (*a) ^ (*b);
    *a = (*a) ^ (*b);
}

void quick_sort2(int a[], int left, int right) {
    if (left >= right)
        return;
    int pos = (left - right) / 2 + right;
    swap(&a[pos], &a[right]);
    int small = left - 1;
    for (int i=left; i<=right; i++) {
        if (a[i] < a[right]) {
            small++;
            if (small != i)
                swap(&a[small], &a[i]);
        }
    }
    pos = small + 1;
    swap(&a[pos], &a[right]);
    quick_sort2(a, left, pos-1);
    quick_sort2(a, pos+1, right);
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

    quick_sort2(a, 0, NUMBER - 1);

    for (i=0; i<NUMBER; i++) {
        printf("%d ", a[i]);
    }
    printf("\n");
    return 0;
}
