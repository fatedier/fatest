#ifndef _REFLECT_H_
#define _REFLECT_H_

#include <map>
#include <string>
#include <iostream>
using namespace std;

typedef void* (*register_func)();

class Class
{
public:
    static void* newInstance(const string& class_name);
    static void registerClass(const string& class_name, register_func func);

private:
    /* key is class name and value is function to create instance of class */
    static map<string, register_func> m_register;
};


class Register
{
public:
    Register(const string& class_name, register_func func) {
        Class::registerClass(class_name, func);
    }
};

#define REGISTER_CLASS(class_name) \
    class class_name##Register { \
    public: \
        static void* newInstance() { \
            return new class_name; \
        } \
    private: \
        static const Register reg; \
    }; \
    const Register class_name##Register::reg(#class_name, class_name##Register::newInstance);

#endif
