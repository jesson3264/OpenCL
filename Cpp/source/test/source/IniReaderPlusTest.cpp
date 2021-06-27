#include <iostream>
#if 0
#include "IniReaderPlusTest.h"
#include "IniReaderPlus.h"
using std::endl;
void IniTest()
{
    ACE::IniReaderPlus  ini("./test.ini");
    std::string v1 = ini.GetValue("section", "key");
    std::cout<<"----------"<<endl;
    std::cout<<"va:"<<v1<<endl;
    
    ini.show();
    ini.AddBaseField("section", "key1", "key3","val3");
    std::cout<<"======="<<endl;
    ini.show();
    ini.Sync();
}
#endif