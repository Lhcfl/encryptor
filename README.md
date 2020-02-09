# encryptor
A simple encryptor

加密一个字符串，你需要：
```
code::init(str, len)
```

其中，str是明文，len是加密出的每组密文长度

函数返回一个vector<string>

解密时：

```
code::decode(code)
```

返回一个string，即原文

testcode.cpp是测试
