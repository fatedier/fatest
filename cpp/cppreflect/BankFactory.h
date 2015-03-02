#ifndef _BANKFACTORY_H_
#define _BANKFACTORY_H_

#include "reflect.h"
#include "BaseBank.h"

class BankFactory
{
public:
    BaseBank* createBank(const string& bank_name) {
        return (BaseBank*)Class::newInstance(bank_name);
    }
};

#endif
