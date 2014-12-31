#include <iostream>
#include <string.h>
using namespace std;

void print_cycle(int **matrix, int row, int column, int cycle)
{
    int endX = column - 1 - cycle;
    int endY = row - 1 - cycle;

    /* left to right */
    for (int i=cycle; i<=endX; i++)
        cout << matrix[cycle][i] << " ";
    /* up to down */
    if (cycle < endY)
    {
        for (int i=cycle + 1; i<=endY; i++)
            cout << matrix[i][endX] << " ";
    }
    /* right to left */
    if (cycle < endX && cycle < endY)
    {
        for (int i=endX - 1; i>=cycle; i--)
            cout << matrix[endY][i] << " ";
    }
    /* down to up */
    if (cycle < endX && cycle < endY - 1)
    {
        for (int i=endY - 1; i>=cycle + 1; i--)
            cout << matrix[i][cycle] << " ";
    }
}

void print_matrix(int **matrix, int row, int column)
{
    if (row <= 0 || column <= 0)
        return;

    int cycle = 0;
    while (column > cycle * 2 && row > cycle * 2)
    {
        print_cycle(matrix, row, column, cycle);
        cycle++;
    }
    cout << endl;
    return;
}

int main()
{
    int **a = new int *[6];
    for (int i=0; i<6; i++)
        a[i] = new int[6];
    
    int count = 1;
    for (int i=0; i<6; i++)
    {
        for (int j=0; j<6; j++)
        {
            a[i][j] = count++;
        }
    }

    /*
    int a[6][6] =
    {
        {1,  2,  3,  4,  5,  6},
        {7,  8,  9,  10, 11, 12},
        {13, 14, 15, 16, 17, 18},
        {19, 20, 21, 22, 23, 24},
        {25, 26, 27, 28, 29, 30},
        {31, 32, 33, 34, 35, 36}
    };
    */
    print_matrix(a, 6, 6);

    for (int i=0; i<6; i++)
    {
        delete a[i];
    }
    delete a;

    return 0;
}
