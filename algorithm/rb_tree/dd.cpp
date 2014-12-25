#include <iostream>
#include <string>
#include "rb_tree.h"
using namespace std;

int main()
{
    RbTree<int, string> rb;
    rb.insert(1, "wwww");
    string a;
    rb.find_by_key(1, a);
    cout << a << endl;
    return 0;
}
