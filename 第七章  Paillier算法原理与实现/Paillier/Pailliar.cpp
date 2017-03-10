#include<ctime>
#include"BigNumber.h"
#include "Timer.h"
using namespace std;

		  BigNum Gcd(BigNum a,BigNum b);//求取最大公约数
		  BigNum Lcm(BigNum a,BigNum b);//求取最小公倍数
		  BigNum Lfunction(BigNum a);//求取L函数

		  BigNum n,I=1,o=0;


int main()
{
      BigNum p,q,I(1),e(257),t,temp,d,a,b,c,O(0),middle,g,pri_key,g1,u;//b为密文，c为解密后结果 
      string A;
      Timer mytime;   //定义时间类类型数据 
      srand((unsigned)time(NULL)); 
	  //int middle;
      
      cout<<"*********************************************************************"<<endl;
      cout<<"*                         Paillier系统                              *"<<endl;     
      cout<<"*                                                                   *"<<endl; 
      cout<<"*********************************************************************"<<endl<<endl;
       
      cout<<"程序运行中···"<<endl<<endl; 
      mytime.start();  //计时开始  
	  middle.Random(0);//产生明文
	  cout<<"明文：";
	  middle.print();
	  cout<<endl;
      p.GeneratePrime();   //产生素数 
      q.GeneratePrime();

      while(p==q)   //判断两个素数不等 
          q.GeneratePrime();
      temp=p-I;
      t=q-I;
      t=t*temp;  
      cout<<"素数p为：";   //输出素数 
      p.print(); 
      cout<<endl;
      cout<<"素数q为：";
      q.print();
      cout<<endl;
      cout<<"公钥n为";
      n=p*q;
	  //n=p+q;
      n.print();
      cout<<endl;
	  pri_key=Lcm(p-I,q-I);
      //d.ex_euclid(e,t);   //计算私钥d 
      cout<<"私钥pri_key为:";
      pri_key.print();
      cout<<endl;
	  //do{
		  g.GeneratePrime();
		//  g1.power_mod(pri_key,n*n);
	  //}while(Gcd(g1,n)!=I);
      cout<<"公钥g为";
      g.print();
      mytime.stop(); // 计时停止
	  mytime.running();//计算产生公钥私钥用的总时间
	  cout<<"产生公钥私钥时间\t"<<mytime.seconds()<<"\tseconds"<<endl;//输出产生公钥私钥所用总时间
      //cout<<endl<<"请输入您要加密的字符串  "<<endl<<endl;//数据输入时存在问题，所以在此采用系统随机生成的一个参数进行加解密
      //getline(cin,A);
      mytime.reset();   //时间清零 
      mytime.start(); //计时开始
      //a=BigNum(A);
      //cout<<endl;
	  BigNum middle1,r;
	  r.GeneratePrime();
	  middle1=g.power_mod(middle,n*n)*r.power_mod(n,n*n);
	  b=middle1.power_mod(I,n*n);
      //b=middle.power_mod(e,n);   //产生密文b 
      cout<<"生成密文如下:";
      b.print();
      cout<<endl;
	  u.ex_euclid(Lfunction(g.power_mod(pri_key,n*n)),n);
	  c=Lfunction(b.power_mod(pri_key,n*n))*u;
	  c=c.power_mod(I,n);
      //c=b.power_mod(d,n);   //解密 
      cout<<"解密后的结果为:"<<endl;
      c.print();
      mytime.stop(); // 计时停止
	  mytime.running();//计算产生素数用的总时间
	  cout<<endl<<"加密解密时间\t"<<mytime.seconds()<<"\tseconds"<<endl;//输出加密解密所用总时间
	  //program > a.txt;
  
      system("pause");
      return 0;
}

BigNum Gcd(BigNum a,BigNum b)
{
	BigNum i;
	do
	{
		i=a%b;
		a=b;
		b=i;
	}while(b!=o);
	return a;
}

BigNum Lcm(BigNum a,BigNum b)
{
	BigNum i;
	i=Gcd(a,b);
	i=a*b/i;
	return i;
}

BigNum Lfunction(BigNum a)
{
	BigNum i;
	i=(a-I)/n;
	return i;
}