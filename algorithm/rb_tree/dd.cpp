#include <iostream>
#include <string>
#include "rb_tree.h"
using namespace std;

int main()
{
    RbTree<int, string> rb;
    rb.insert(1, "www");
    rb.insert(2, "wcl");
    rb.insert(3, "test");
    string a;
    rb.find(1, a);
    cout << a << endl;
    rb.find(2, a);
    cout << a << endl;
    rb.find(3, a);
    cout << a << endl;
    if (rb.find(4, a) == 0)
        cout << a << endl;

    return 0;
}
