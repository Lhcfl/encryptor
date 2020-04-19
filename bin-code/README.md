# bin-code
A simple encryptor

加密一个字符串，你需要：
```
binc::incode(const char* str)
```

其中，str是明文

函数返回一个std::string

解密时：

```
binc::decode(const char* str)
```

返回一个std::string，即原文

testcode.cpp是测试

加密长度是原字符串长度的8倍

原理：字符串化为二进制直接输出
bincode.h中
```cpp bincode.h
const char m[2]={' ', '\t'};
```
可以修改为想要的值。