#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstdlib>
#include <algorithm>
#include <string>
#include <vector>
#include <map>

#include "md5.h"
using std::cout;
using std::cin;
using std::endl;
const char* charmap = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-=!!!!";
const char* intc =    "pJMdhj6Ki1tNx9ZQFVL3bw8BksrG2OlaznfXTmIU=vE05cCDS-RAuoye7H4YWPqg!!!!";
const int cint[128]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,49,0,0,43,9,28,19,58,44,6,56,22,13,0,0,0,40,0,0,0,51,23,46,47,42,16,27,57,38,1,7,18,2,11,29,61,15,50,48,36,39,17,60,35,59,14,0,0,0,0,0,0,31,20,45,3,55,34,63,4,8,5,24,30,37,33,53,0,62,26,25,10,52,41,21,12,54,32,0,0,0,0,0};
// intc: int转char
// cint: char转int
/*
//未来更新
inline unsigned int chint(char c){
	//32字符域转换为int
	if (c >= 'a' && c <= 'z') return c - 'a';
	if (c == ',') return 26;
	if (c == '.') return 27;
	if (c == '"') return 28;
	if (c == '?') return 29;
	if (c == ' ') return 30;
}
inline char chchr(int c){
	//32字符域转换为int
	if (c >= 0 && c <= 25) return c + 'a';
	if (c == 26) return ',';
	if (c == 27) return '.';
	if (c == 28) return '"';
	if (c == 29) return '?';
	if (c == 30) return ' ';
}

std::string ch3264(const char* head, bool res=false) {
	//6个32字符域字符转化为5个64字符域字符
	std::string ret;
	unsigned int t = 0;
	for (int i = 0; i < 6; i++) {
		t |= chint(head[i]) << i*5;
	}
	if (res) t = ~t;
	for (int tmp = 0; tmp < 5; tmp++) {
		ret += intc[t & 63];
		t >>= 6;
	}
	return ret;
}
*/
std::string chff64(const char* head, bool res=false) {
	//3个char字符转化为4个64字符域字符
	std::string ret;
	unsigned int t = 0;
	for (int i = 0; i < 3; i++) {
		t |= (unsigned int)head[i] << i*8;
	}
	if (res) t = ~t;
	for (int tmp = 0; tmp < 4; tmp++) {
		ret += intc[t & 63];
		t >>= 6;
	}
	return ret;
}

std::string ch256(const char* head, bool res=false) {
	//4个64字符域字符转化为3个char字符
	std::string ret;
	unsigned int t = 0;
	for (int i = 0; i < 4; i++) {
		t |= (unsigned int)cint[head[i]] << i*6;
	}
	if (res) t = ~t;
	for (int tmp = 0; tmp < 3; tmp++) {
		ret += (char)(t & 255);
		t >>= 8;
	}
	return ret;
}
/*
//未来更新
std::string ch32(const char* head, bool res=false) {
	//5个64字符域字符转化为6个32字符
	std::string ret;
	unsigned int t = 0;
	for (int i = 0; i < 5; i++) {
		t |= (unsigned int)cint[head[i]] << i*6;
	}
	if (res) t = ~t;
	for (int tmp = 0; tmp < 6; tmp++) {
		ret += chchr(t & 31);
		t >>= 5;
	}
	return ret;
}
*/
inline char modechar(bool password = false, bool res = false, bool MD5open = false, bool rndchr = false, bool chrnum = 1) {
	return intc[password | (res<<1) | (MD5open<<2) | (rndchr<<3) | (chrnum<<4)];
}

inline char merge_char(char a, char b) {
	return intc[(cint[a] ^ cint[b]) & 63];
}

std::string incode(std::string text, char mode, std::string pswd = "") {
	// 密文形式：
	// [1字节格式码][MD5校验区][随机char区][密文区]
	// 其中， 只有随机char区和密文区是受到密码保护的
	// 初始化返回密文
	string ret = "", rnd = "", code = ""; 
	ret += mode;
	// 模式解析
	bool openpswd = false, res = false, MD5open = false, rndchr = false, chrnum = 1;
	mode = cint[mode];
	openpswd = mode & 1;
	res = mode & 2;
	MD5open = mode & 4;
	rndchr = mode & 8;
	chrnum = mode & 32;
	// MD5生成
	if (MD5open) {
		string codeMD5 = md5::getMD5(text);
		ret += codeMD5;
	}
	// 密文生成
	int len = text.length();
	for (int i = 0; i < len;) {
		//chff64加密
		if (i + 2 >= len) {
			char s[5] = {0,0,0,0,0};
			for (int k = 0; i + k < len; k++) {
				s[k] = text[k + i];
			}
			code += chff64(text.c_str() + i, res);
		} else {
			code += chff64(text.c_str() + i, res);
		}
		i += 3;
			
	}
	//rnd加密
	if (rndchr) {
		for (int i = 1; i <= (chrnum+1)*16; i++) {
			rnd += intc[rand()%64];
		}
		string rnd_code = incode(rnd, modechar(0,0,0,0,0));
		//自我迭代
		for (int i = 1; i <= (chrnum+1)*16; i++) {
			rnd_code = incode(rnd_code, modechar(0,0,0,0,0));
		}
		int code_len =  code.length(), rnd_code_len = rnd_code.length();
		//cout << "rnd_code_len is :" << rnd_code_len << endl;
		for (int i = 0; i < code_len; i++) {
			code[i] = merge_char(rnd_code[i%rnd_code_len], code[i]);
		}
	}
	//密码处理
	if (openpswd) {
		string pswd_code = incode(pswd, modechar(0,0,0,0,0));
		//自我迭代
		int code_len =  code.length();
		while (pswd_code.length() < code_len && pswd_code.length() < 1e7) {
			pswd_code = incode(pswd_code, modechar(0,0,0,0,0));
		}
		int pswd_code_len = pswd_code.length();
		//cout << "pswd_code_len is :" << pswd_code_len << endl;
		for (int i = 0; i < code_len; i++) {
			code[i] = merge_char(pswd_code[i%pswd_code_len], code[i]);
		}
	}
	return ret+rnd+code;
}

std::string decode(std::string text, int& successcode, std::string pswd = "") {
	// 密文形式：
	// [1字节格式码][MD5校验区][随机char区][密文区]
	// 其中， 只有随机char区和密文区是受到密码保护的
	
	int p = 1;
	//p means pointer: the location of this string.
	// 初始化
	string ret = "", rnd="", codeMD5;
	// 模式解析
	bool openpswd = false, res = false, MD5open = false, rndchr = false, chrnum = 1;
	int mode = cint[text[0]];
	openpswd = mode & 1;
	res = mode & 2;
	MD5open = mode & 4;
	rndchr = mode & 8;
	chrnum = mode & 32;
	// MD5生成
	if (MD5open) {
		for (int i = 0; i < 32; i++) codeMD5 += text[p++];
	}
	// 随机解析
	if (rndchr) {
		for (int i = 1; i <= (chrnum+1)*16; i++) rnd += text[p++];
		string rnd_code = incode(rnd, modechar(0,0,0,0,0));
		//自我迭代
		for (int i = 1; i <= (chrnum+1)*16; i++) {
			rnd_code = incode(rnd_code, modechar(0,0,0,0,0));
		}
		int code_len =  text.length(), rnd_code_len = rnd_code.length();
		//cout << "rnd_code_len is :" << rnd_code_len << endl;
		for (int i = 0; i + p < code_len; i++) {
			text[i+p] = merge_char(rnd_code[i%rnd_code_len], text[i+p]);
		}
	}
	// 密码解析
	if (openpswd) {
		string pswd_code = incode(pswd, modechar(0,0,0,0,0));
		//自我迭代
		int code_len = text.length();
		while(pswd_code.length() < code_len - p && pswd_code.length() < 1e7) {
			pswd_code = incode(pswd_code, modechar(0,0,0,0,0));
		}
		int pswd_code_len = pswd_code.length();
		//cout << "pswd_code_len is :" << pswd_code_len << endl;
		for (int i = 0; i + p < code_len; i++) {
			text[i+p] = merge_char(pswd_code[i%pswd_code_len], text[i+p]);
		}
	}
	// 明文生成
	int len = text.length();
	while (p < len) {
		//ch256解密
		ret += ch256(text.c_str() + p, res);
		p += 4;
	}
	//MD5验证
	successcode = 1;
	if (MD5open && codeMD5 != md5::getMD5(ret.c_str())) {
		successcode = 0;
	}
	return ret;
}

using namespace std;
int main() {
	std::string s;
	//cout << chff64("abc") << endl;
	//cout << ch256(chff64("abc").c_str()) << endl;
	cin >> s;
	cout << md5::getMD5(s) << endl;
    cout << (s = incode(s, modechar(1,0,1,1,1), "donotdieda")) << endl;
	int a = 0;
	cout << decode(s, a, "donotdieda").c_str() << endl;
	cout << (a==0?"N":"Y")<<endl;
    return 0;
}
