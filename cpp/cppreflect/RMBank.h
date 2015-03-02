#include <iostream>
using namespace std;

#include "reflect.h"
#include "BaseBank.h"

class RMBank:public BaseBank
{
public:
    virtual int setMsg() {
        cout << "拼人民银行报文" << endl;
        return 0;
    }

    virtual int sendMsg() {
        cout << "发送人民银行报文" << endl;
        return 0;
    }

    virtual int getMsg() {
        cout << "接收人民银行报文" << endl;
        return 0;
    }

    virtual int parseMsg() {
        cout << "解析人民银行报文" << endl;
        return 0;
    }
};

REGISTER_CLASS(RMBank)
