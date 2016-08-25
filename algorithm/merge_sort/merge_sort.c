#include <string.h>
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

void merge(int a[], int first, int mid, int last)
{
    int *temp;
    temp = (int *)malloc(sizeof(int) * (last - first + 1));

    int i = first;
    int j = mid + 1;
    int n;
    for (n=0; n<(last - first + 1); n++) {
        if (a[i] <= a[j]) {
            temp[n] = a[i];
            i++;
            if (i > mid) {
                memcpy(&temp[n + 1], &a[j], sizeof(int)*(last - j + 1));
                break;
            }
        } else {
            temp[n] = a[j];
            j++;
            if (j > last) {
                memcpy(&temp[n + 1], &a[i], sizeof(int)*(mid - i + 1));
                break;
            }
        }
    }

    for (n=0; n<(last - first + 1); n++) {
        a[first + n] = temp[n];
    }

    free(temp);
}

void merge_sort(int a[], int first, int last)
{
    int mid = (first + last) / 2;

    if (first == last)
        return;

    merge_sort(a, first, mid);
    merge_sort(a, mid + 1, last);

    merge(a, first, mid, last);
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
    
    merge_sort(a, 0, NUMBER - 1);
    
    for (i=0; i<NUMBER; i++) {
        printf("%d ", a[i]);
    }
    printf("\n");

    return 0;
}
