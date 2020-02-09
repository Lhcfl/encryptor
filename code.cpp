#include "md5.h"
#include <string>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <iostream>

using std::vector;
using std::string;

string code::getkey(unsigned int seed) {
	string ret = "";
	srand(seed);
	for (int i = 1; i <= 32; i++) ret += rand() % 2  ? (char)(rand() % 26 + 'a') : (char)(rand() % 26 + 'A');
	return ret;
}

void code::filltrash(char* str, int point, int len) {
	for (int i = point; i < len; i++) str[i] = rand() % 2  ? (char)(rand() % 26 + 'a') : (char)(rand() % 26 + 'A');
	str[len]='\0';
}

vector<string> code::init(string text, int len = 1500) {
	// len:密文单个字符串长度
	// 加密后密文长度len, 第一条密文前32位是原文md5，再后32位是随机字串（密钥）
	// len不得小于128
	// 返回空vector为错误
	vector<string> ret;
	char *str = new char[len + 5];
	string key = getkey((unsigned int)(time(NULL)));
	if (len < 128) return ret;
	string codemd5 = md5::getMD5(text);
	for (int i = 0; i < 32; i++) str[i] = codemd5[i];
	for (int i = 32; i < 64; i++) str[i] = key[i - 32];
	//前64位填充完毕
	int point = 0, textlen = text.length();
	bool over = false;
	//point: text指针
	//over: 加密结束标记
	for (int i = 64; i < len; i+=4) {
		if (point == textlen) {
			filltrash(str, i, len);
			str[i] = '!';
			str[i+1] = '!';
			str[i+2] = '!';
			str[i+3] = '!';
			str[len] = 0;
			ret.push_back(str);
			over = true;
			break;
		} else if (i + 7 < len) {
			int tmp = (129 + int(text[point++])) ^ key[i%32]; 
			str[i] = '#' + tmp/4 ;
			str[i+1] = '#' + tmp/4 + 2;
			str[i+2] = '#' + tmp/4 + 1;
			str[i+3] = '#' + tmp/4 + tmp % 4;
		} else {
			filltrash(str, i, len);
			ret.push_back(str);
		}
	}
	while (!over) {
		for (int i = 0; i < len; i+=4) {
			if (point == textlen) {
				filltrash(str, i, len);
				str[i] = '!';
				str[i+1] = '!';
				str[i+2] = '!';
				str[i+3] = '!';

				str[len] = 0;
				ret.push_back(str);
				over = true;
				break;
			} else if (i + 7 < len) {
				int tmp = (129 + int(text[point++])) ^ key[i%32]; 
				str[i] = '#' + tmp/4 ;
				str[i+1] = '#' + tmp/4 + 2;
				str[i+2] = '#' + tmp/4 + 1;
				str[i+3] = '#' + tmp/4 + tmp % 4;
			} else {
				filltrash(str, i, len);
				ret.push_back(str);
			}
		}
	}
	return ret;
}

string code::decode(vector<string> code) {
	string ret="";
	char codemd5[40];
	char key[40];
	int SIZE = code.size();
	for (int I = 0; I < SIZE; I++) {
		string &str = code[I];
		if (I == 0) {
			for (int i = 0; i < 32; i++) codemd5[i] = str[i];
				codemd5[32]=0;
			for (int i = 32; i < 64; i++) key[i - 32] = str[i];
				key[32]=0;
			//std::cout << "receive md5: " << codemd5 << " key: " << key << std::endl << "code: " ;
			int strlen = str.length();
			for (int i = 64; i + 7 < strlen; i+=4) {
				if (str[i] == '!' || str[i+1] == '!' || str[i+2] == '!' || str[i+3] == '!') goto fin;
				//std::cout<<(str[i]) << str[i+1] << str[i+2] << str[i+3];
				int tmp = int(str[i]) + str[i+1] + str[i+2] + str[i+3] - 4*'#' -3;
				tmp = (tmp^key[i%32]) - 129;
				ret += char(tmp);
			}
		} else {
			int strlen = str.length();
			for (int i = 0; i + 7 < strlen; i+=4) {
				if (str[i] == '!' || str[i+1] == '!' || str[i+2] == '!' || str[i+3] == '!') goto fin;
				//std::cout<<(str[i]) << str[i+1] << str[i+2] << str[i+3];
				int tmp = int(str[i]) + str[i+1] + str[i+2] + str[i+3] - 4*'#' - 3;
				tmp = (tmp^key[i%32]) - 129;
				ret += char(tmp);
			}
		}
	}
	fin:
	if(md5::getMD5(ret) != codemd5) return "ERROR: Failed. get MD5 is" + md5::getMD5(ret);
	std::cout<<"\n";
	return ret;
}