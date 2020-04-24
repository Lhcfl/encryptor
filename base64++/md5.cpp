/*
 * Author: Baidu Wiki
 * 我cnmd写的什么垃圾代码
 * 改了我tm两个小时
 */
#include<string>
#include "md5.h"
using std::string;
void md5::mainLoop(unsigned int M[])
{
   unsigned int f,g;
   unsigned int a=md5::atemp;
   unsigned int b=md5::btemp;
   unsigned int c=md5::ctemp;
   unsigned int d=md5::dtemp;
   for (unsigned int i = 0; i < 64; i++)
   {
       if(i<16){
           f=md5::_F_(b,c,d);
           g=i;
       }else if (i<32)
       {
           f=md5::_G_(b,c,d);
           g=(5*i+1)%16;
       }else if(i<48){
           f=md5::_H_(b,c,d);
           g=(3*i+5)%16;
       }else{
           f=md5::_I_(b,c,d);
           g=(7*i)%16;
       }
       unsigned int tmp=d;
       d=c;
       c=b;
       b=b+md5::_SHIFT_((a+f+md5::k[i]+M[g]),md5::s[i]);
       a=tmp;
   }
   md5::atemp=a+md5::atemp;
   md5::btemp=b+md5::btemp;
   md5::ctemp=c+md5::ctemp;
   md5::dtemp=d+md5::dtemp;
}
   /*
   *填充函数
   *处理后应满足bits≡448(mod512),字节就是bytes≡56（mode64)
   *填充方式为先加一个1,其它位补零
   *最后加上64位的原来长度
   */
unsigned int* md5::add(string str)
{
   unsigned int num=((str.length()+8)/64)+1;//以512位,64个字节为一组
       unsigned int *strByte=new unsigned int[num*16];    //64/4=16,所以有16个整数
       strlength=num*16;
       for (unsigned int i = 0; i < num*16; i++)
           strByte[i]=0;
       for (unsigned int i=0; i <str.length(); i++)
       {
           strByte[i>>2]|=(str[i])<<((i%4)*8);//一个整数存储四个字节，i>>2表示i/4 一个unsigned int对应4个字节，保存4个字符信息
       }
       strByte[str.length()>>2]|=0x80<<(((str.length()%4))*8);//尾部添加1 一个unsigned int保存4个字符信息,所以用128左移
       /*
       *添加原长度，长度指位的长度，所以要乘8，然后是小端序，所以放在倒数第二个,这里长度只用了32位
       */
       strByte[num*16-2]=str.length()*8;
       return strByte;
   }
   string md5::changeHex(int a)
   {
       int b;
       string str1;
       string str="";
       for(int i=0;i<4;i++)
       {
           str1="";
               b=((a>>i*8)%(1<<8))&0xff;   //逆序处理每个字节
               for (int j = 0; j < 2; j++)
               {
                   str1.insert(0,1,md5::str16[b%16]);
                   b=b/16;
               }
               str+=str1;
           }
           return str;
       }
       string md5::getMD5(string source)
       {
   md5::atemp=md5::_A_;    //初始化
   md5::btemp=md5::_B_;
   md5::ctemp=md5::_C_;
   md5::dtemp=md5::_D_;
   unsigned int *strByte=add(source);
   for(unsigned int i=0;i<strlength/16;i++)
   {
       unsigned int num[16];
       for(unsigned int j=0;j<16;j++)
           num[j]=strByte[i*16+j];
       mainLoop(num);
   }
   return changeHex(md5::atemp).append(changeHex(md5::btemp)).append(changeHex(md5::ctemp)).append(changeHex(md5::dtemp));
}