#ifndef _BASE_H_
#define _BASE_H_

class BaseBank
{
public:
    virtual int setMsg() = 0;
    virtual int sendMsg() = 0;
    virtual int getMsg() = 0;
    virtual int parseMsg() = 0;
};

#endif
