#include <iostream>
using namespace std;

const int g_max_value = 6;

void printAll(int n)
{
    int max_number = g_max_value * n + 1;
    int *sum_array1 = new int[max_number];
    int *sum_array2 = new int[max_number];
    for (int i=0; i<max_number; i++)
    {
        sum_array1[i] = 0;
        sum_array2[i] = 0;
    }

    bool use_sum_array1 = true;
    for (int i=1; i<=g_max_value; i++)
    {
        sum_array2[i] = 1;
    }

    for (int i=2; i<=n; i++)
    {
        if (use_sum_array1)
        {
            for (int j=1; j<=g_max_value * i; j++)
            {
                int sum = 0;
                for (int k=1; k<=g_max_value; k++)
                {
                    if (j - k > 0)
                        sum += sum_array2[j - k];
                }
                sum_array1[j] = sum;
            }
            use_sum_array1 = !use_sum_array1;
        }
        else
        {
            for (int j=1; j<=g_max_value * i; j++)
            {
                int sum = 0;
                for (int k=1; k<=g_max_value; k++)
                {
                    if (j - k > 0)
                        sum += sum_array1[j - k];
                }
                sum_array2[j] = sum;
            }
            use_sum_array1 = !use_sum_array1;
        }
    }

    use_sum_array1 = !use_sum_array1;
    for (int i=1; i<max_number; i++)
    {
        if (use_sum_array1)
            cout << sum_array1[i] << " ";
        else
            cout << sum_array2[i] << " ";
    }
    cout << endl;
    delete []sum_array1;
    delete []sum_array2;
}

int main()
{
    printAll(6);
    return 0;
}
