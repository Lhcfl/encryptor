#include <bits/stdc++.h>
#include "code.h"

using namespace std;

int main(int argc, char* argv[]) {
	for (int i = 0; i <= 129+126; i++){
		for (int j = 'a'; j <= 'z'; j++) {
			if ((i^j)/4+'#' + 3>126)cout <<i <<", " << j<< endl;
		}
		for (int j = 'A'; j <= 'z'; j++) {
			if ((i^j)/4+'#' + 3>126)cout <<i <<", " << j<< endl;
		}

	}
    string s;
    cin>>s;
    cout << "MD5 is: " << md5::getMD5(s)<<endl;
    cout << "------------------------------\n";
    std::vector<string> v = code::init(s, 256);
    cout <<"code num:" << v.size() << endl;
    for(std::vector<string>::iterator i =v.begin();i!=v.end();i++)cout << "code: " << *i <<endl;
    cout << "------------------------------\n";
    cout << "\n\n\ntext:" << code::decode(v);
    return 0;
}