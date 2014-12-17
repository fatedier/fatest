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

void quick_sort(int a[], int left, int right)
{
    if (right <= left)
        return;

    int p_left = left;
    int p_right = right;
    int p_mid = (p_left + p_right) / 2;
    int temp;
    
    bool is_left = true;

    while (p_left != p_right) {
        if (is_left == true) {
            if (a[p_left] > a[p_mid]) {
                temp = a[p_mid];
                a[p_mid] = a[p_left];
                a[p_left] = temp;
                p_mid = p_left;
                is_left = false;
                continue;
            } else {
                if (p_left == p_mid) {
                    is_left = false;
                    continue;
                }
                p_left++;
                continue;
            }
        } else {
            if (a[p_right] < a[p_mid]) {
                temp = a[p_mid];
                a[p_mid] = a[p_right];
                a[p_right] = temp;
                p_mid = p_right;
                is_left = true;
                continue;
            } else {
                if (p_right == p_mid) {
                    is_left = true;
                    continue;
                }
                p_right--;
                continue;
            }
        }
    }

    quick_sort(a, left, p_mid - 1);
    quick_sort(a, p_mid + 1, right);
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

    quick_sort(a, 0, NUMBER - 1);
    for (i=0; i<NUMBER; i++) {
        printf("%d ", a[i]);
    }
    printf("\n");

    return 0;
}
