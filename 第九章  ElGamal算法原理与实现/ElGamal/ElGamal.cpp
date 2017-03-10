#include<ctime>
#include"BigNumber.h"
#include "Timer.h"
using namespace std;


BigNum I=1,o=0,e=2;


//密钥产生过程：首先选择一素数p以及两个小于p的随机数g（g为p的原根）和x，计算y=g^x mod p.以(y,g,p)作为公开密钥，x作为秘密密钥

int main()
{
      BigNum p,q,I(1),e(2),O(0),middle,g,x,y,k,k1,C1,C2,M1,M2,M; 
	  //BigNum test[20000];
	  Timer time;
	  time.start();
      //string A;
	  //int middle;
      
      cout<<"*********************************************************************"<<endl;
      cout<<"*                         ElGamal公钥密码体制                             *"<<endl;     
      cout<<"*********************************************************************"<<endl<<endl;
        
	  middle.Random(0);//产生明文
	  cout<<"明文："<<endl;
	  middle.print();
	  cout<<endl;
	  p.GeneratePrime();
	  g.Random(p-I);
	  //生成素数p，并且求素数p的原根g
      /*do
	  {
		  cout<<"1"<<endl;
		  q.GeneratePrime();   //产生素数q 
		  p=q*e+I;
	  }while(p.Rabin_Miller()!=1);
	  cout<<"第一个do-while循环"<<endl;
      do
	  {
		  cout<<"2"<<endl;
		  g.Random(p-I);
	  }while((g.power_mod(e,p)==I)||(g.power_mod(q,p)==I));//已经求得素数原根为g 
	  cout<<"第二个do-while循环"<<endl;*/
      cout<<"素数p为："<<endl;   //输出素数 
      p.print(); 
      cout<<endl;
      cout<<"p的原根g为："<<endl;
      g.print();
      cout<<endl;
	  x.Random(p);
	  y=g.power_mod(x,p);
	  cout<<"公钥y为："<<endl;
	  y.print();
	  cout<<endl;
      //d.ex_euclid(e,t);   //计算私钥d 
      cout<<"私钥x为"<<endl;
      x.print();
      cout<<endl;
      //a=BigNum(A);
      //cout<<endl;
      //开始加密明文middle，加密过程：设欲加密明文消息M，随机选一与p-1互素的整数k，计算C1=g^k mod p，C2=y^k*M mod p,密文为C=(C1，C2)

	  k.GeneratePrime();//产生加密用的随机数k
	  C1=g.power_mod(k,p);//产生第一个密文C1
	  C2=y.power_mod(k,p);
	  C2=(C2*middle)%p;//产生第二个密文C2
	  cout<<"输出密文："<<endl;
	  cout<<"输出C1:"<<endl;
	  C1.print();
	  cout<<endl;
	  cout<<"输出C2:"<<endl;
	  C2.print();
	  cout<<endl;
	  //加密结束
	  //开始解密密文C1和C2
	  M1=C1.power_mod(x,p);
      M2.ex_euclid(M1,p);
	  /*test=(M1*M2)%p;
	  cout<<"输出test"<<endl;
	  test.print();
	  cout<<endl;*/
	  M=(C2*M2)%p;
	  cout<<"输出解密后的明文："<<endl;
	  M.print();
	  cout<<endl;
	  time.stop();
	  time.running();
	  cout<<endl<<"加密解密时间\t"<<time.seconds()<<"\tseconds"<<endl;
	  
      system("pause");
}
