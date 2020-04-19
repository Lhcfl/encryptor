#include<bits/stdc++.h>
#include "bincode.h"
using namespace std;
int main() {
	string s;
    cin >> s;
    cout << binc::incode(s.c_str()) <<endl;
    cout << binc::decode(binc::incode(s.c_str()).c_str()) <<endl;
}
