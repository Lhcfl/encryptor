#include <string>
#include <cstring>

#ifndef _BINCODE_H_
#define _BINCODE_H_
using std::string;
namespace binc{
const char m[2]={' ', '\t'};
//m为密码字数组，可以自行修改
std::string incode(const char* s);
std::string decode(const char* s);

}
#include "bincode.cpp"

#endif
