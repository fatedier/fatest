#include <iostream>
#include <string.h>
using namespace std;

int number_of_one(int n)
{
    int count = 0;
    int flag = 1;
    while (flag)
    {
        if (flag & n)
            count++;

        flag = flag << 1;
    }
    return count;
}

int number_of_one_other(int n)
{
    int count = 0;
    while (n)
    {
        count++;
        n = (n - 1) & n;    /* set the rightest number "1" of n to "0" */
    }
    return count;
}

int main()
{
    cout << number_of_one(-1) << endl;
    cout << number_of_one_other(3) << endl;
    return 0;
}
