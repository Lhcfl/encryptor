#include "bincode.h"
#include <string>
#include <cstring>
std::string binc::incode(const char* s) {
	std::string ret;
	int len = strlen(s);
	for (int i = 0; i < len; i++) {
		char t = s[i];
		for (int j = 0; j < 8 * sizeof(char); j++) {
			ret += binc::m[t&1];
			t >>= 1;
		}
	}
	return ret;
}
std::string binc::decode(const char* s) {
	std::string ret;
	int len = strlen(s);
	for (int i = 0; i < len; i += 8 * sizeof(char)) {
		char t = 0;
		for (int j = i; j < i + 8 * sizeof(char); j++) {
			t |= (s[j] == binc::m[1]) << j-i;
		}
		ret += t;
	}
	return ret;
}

