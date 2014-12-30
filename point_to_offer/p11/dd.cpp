#include <iostream>
#include <string.h>
#include <exception>
#include <stdexcept>
using namespace std;

bool double_equal(double num1, double num2)
{
    if ((num1 - num2) > -0.000001 && (num1 - num2) < 0.000001)
        return true;
    else
        return false;
}

double power_with_unsigned_exponent(double base, unsigned int exponent)
{
    if (exponent == 0)
        return 1;

    if (exponent == 1)
        return base;

    double result = power_with_unsigned_exponent(base, exponent >> 1);
    result *= result;
    if ((exponent & 0x1) == 1)
        result *= base;

    return result;
}

double power(double base, int exponent)
{
    double result = 1.0;
    if (double_equal(base, 0.0) && exponent < 0)
    {
        throw logic_error("input error");
    }
    
    if (exponent == 0)
    {
        return 1.0;
    }
    else if (exponent > 0)
    {
        result = power_with_unsigned_exponent(base, exponent);
        return result;
    }
    else
    {
        exponent = -1 * exponent;
        result = power_with_unsigned_exponent(base, exponent);
        return 1.0 / result;
    }
}

int main()
{
    try
    {
        cout << power(2, 12) << endl;
    }
    catch(exception &e)
    {
        cout << e.what() << endl;
    }
    return 0;
}
