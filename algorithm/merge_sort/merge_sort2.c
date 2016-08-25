#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

#define NUMBER 20
#define RANDMIN 0
#define RANDMAX 1000

void merge(int a[], int left, int mid, int right) {
    int* temp = (int*)malloc((right - left + 1) * sizeof(int));
    int i = left;
    int j = mid + 1;
    int index = 0;
    while (i <= mid && j <= right) {
        if (a[i] <= a[j]) {
            temp[index] = a[i];
            i++;
        } else {
            temp[index] = a[j];
            j++;
        }
        index++;
    }
    if (j > right) {
        for (int k=i; k<=mid; k++) {
            temp[index] = a[k];
            index++;
        }
    } else {
        for (int k=j; k<=right; k++) {
            temp[index] = a[k];
            index++;
        }
    }

    for (int k=0; k<index; k++) {
        a[left + k] = temp[k];
    }
    free(temp);
    return;
}

void merge_sort2(int a[], int left, int right) {
    if (left >= right)
        return;
    int mid = left + (right - left) / 2;
    merge_sort2(a, left, mid);
    merge_sort2(a, mid+1, right);
    merge(a, left, mid, right);
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
    
    merge_sort2(a, 0, NUMBER - 1);
    
    for (i=0; i<NUMBER; i++) {
        printf("%d ", a[i]);
    }
    printf("\n");

    return 0;
}
