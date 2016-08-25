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

int random_in_range(int start, int end)
{
    return rand() % (end - start + 1) + start;
}

int sort_one(int a[], int length, int left, int right)
{
    if (a == NULL || length <= 0 || left < 0 || right >= length) {
        return -1;  /* error */
    }
    int index = random_in_range(left, right);
    swap(&a[index], &a[right]);
    int small = left - 1;

    for (index = left; index < right; index++) {
        if (a[index] < a[right]) {
            small++;
            if (small != index)
                swap(&a[small], &a[index]);
        }
    }
    small++;
    swap(&a[small], &a[right]);
    return small;
}

void quick_sort(int a[], int length, int left, int right)
{
    if (left == right)
        return;

    int index = sort_one(a, length, left, right);
    if (index < 0) /* error */
        return;
    if (index > left)
        quick_sort(a, length, left, index - 1);
    if (index < right)
        quick_sort(a, length, index + 1, right);
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

    quick_sort(a, NUMBER, 0, NUMBER - 1);

    for (i=0; i<NUMBER; i++) {
        printf("%d ", a[i]);
    }
    printf("\n");
    return 0;
}
