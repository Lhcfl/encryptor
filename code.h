#ifndef CODE_H
#define CODE_H
#include "md5.h"
#include <string>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <iostream>

namespace code{
using std::vector;
using std::string;
string getkey(unsigned int seed);
void filltrash(char* str, int point, int len);
vector<string> init(string text, int len);
string decode(vector<string> code);
}
#include "code.cpp"
#endif