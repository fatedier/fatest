#include <iostream>
#include <string.h>
using namespace std;

class CMyString
{
public:
    CMyString(const char *pData = NULL);
    CMyString(const CMyString &str);
    ~CMyString();

    CMyString& operator =(const CMyString &str);
    void print();
private:
    char *m_pData;
};

CMyString::CMyString(const char *pData)
{
    if (pData == NULL)
    {
        m_pData = NULL;
        return;
    }

    m_pData = new char[strlen(pData) + 1];
    strcpy(m_pData, pData);
}

CMyString::CMyString(const CMyString &str)
{
    m_pData = new char[strlen(str.m_pData) + 1];
    strcpy(m_pData, str.m_pData);
}

CMyString& CMyString::operator =(const CMyString &str)
{
    if (this == &str)
        return *this;
    
    char *temp = new char[strlen(str.m_pData) + 1];
    if (temp == NULL)
        return *this;

    delete []m_pData;
    m_pData = temp;
    strcpy(m_pData, str.m_pData);

    return *this;
}

CMyString::~CMyString()
{
    delete []m_pData;
}

void CMyString::print()
{
    cout << m_pData << endl;
}

int main()
{
    CMyString a("1234");
    a.print();
    CMyString b;
    b = a;
    b.print();
    return 0;
}
