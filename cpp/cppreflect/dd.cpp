#include <string>
#include "reflect.h"
#include "BaseBank.h"
#include "GSBank.h"
#include "RMBank.h"
#include<assert.h>
using namespace std;

int main(int argc, char **argv)
{
    if (argc < 2) {
        cout << "option error!" << endl;
        return 1;
    }
    string bank_name = argv[1];
    BaseBank* t = (BaseBank*)Class::newInstance(bank_name);
    if (t == NULL) {
        cout << "银行编码错误！" << endl;
        return 2;
    }
    t->setMsg();
    t->sendMsg();
    t->getMsg();
    t->parseMsg();
    delete t;
    return 0;
}
