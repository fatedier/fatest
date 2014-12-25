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
    rb.find_by_key(1, a);
    cout << a << endl;
    rb.find_by_key(2, a);
    cout << a << endl;
    rb.find_by_key(3, a);
    cout << a << endl;
    if (rb.find_by_key(4, a) == 0)
        cout << a << endl;

    return 0;
}
