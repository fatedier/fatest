#include <iostream>
#include "reflect.h"
#include "BaseBank.h"
using namespace std;

class GSBank:public BaseBank
{
public:
    virtual int setMsg() {
        cout << "拼工商银行报文" << endl;
        return 0;
    }

    virtual int sendMsg() {
        cout << "发送工商银行报文" << endl;
        return 0;
    }

    virtual int getMsg() {
        cout << "接收工商银行报文" << endl;
        return 0;
    }
    
    virtual int parseMsg() {
        cout << "解析工商银行报文" << endl;
        return 0;
    }
};

REGISTER_CLASS(GSBank)
