#include "reflect.h"

map<string, register_func> Class::m_register;

void* Class::newInstance(const string& class_name) {
    map<string, register_func>::iterator it = m_register.find(class_name);
    if (it == m_register.end())
        return NULL;
    else
        return it->second();
}

void Class::registerClass(const string& class_name, register_func func) {
    m_register[class_name] = func;
}
