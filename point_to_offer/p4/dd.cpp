#include <iostream>
#include <string.h>
using namespace std;

/* transfer space to "%20",str has enough memory space */
void transfer(char *str)
{
    int num = 0;
    for (int i=0; i<(int)strlen(str); i++)
    {
        if (str[i] == ' ')
            num++;
    }
    
    char *tail = str + strlen(str) + 2 * num;
    char *str_end = str + strlen(str) - 1;
    *(tail--) = '\0';
    int count = 0;
    while (count < num)
    {
        if (*str_end == ' ')
        {
            *(tail--) = '0';
            *(tail--) = '2';
            *(tail--) = '%';
            str_end--;
            count++;
        }
        else
        {
            *(tail--) = *(str_end--);
        }
    }
}

int main()
{   
    char a[100] = "abc def gh";
    transfer(a);
    cout << a << endl;
    return 0;
}
