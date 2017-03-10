#include<ctime>
#include"BigNumber.h"
#include "Timer.h"
using namespace std;

		  BigNum Gcd(BigNum a,BigNum b);//��ȡ���Լ��
		  BigNum Lcm(BigNum a,BigNum b);//��ȡ��С������
		  BigNum Lfunction(BigNum a);//��ȡL����

		  BigNum n,I=1,o=0;


int main()
{
      BigNum p,q,I(1),e(257),t,temp,d,a,b,c,O(0),middle,g,pri_key,g1,u;//bΪ���ģ�cΪ���ܺ��� 
      string A;
      Timer mytime;   //����ʱ������������ 
      srand((unsigned)time(NULL)); 
	  //int middle;
      
      cout<<"*********************************************************************"<<endl;
      cout<<"*                         Paillierϵͳ                              *"<<endl;     
      cout<<"*                                                                   *"<<endl; 
      cout<<"*********************************************************************"<<endl<<endl;
       
      cout<<"���������С�����"<<endl<<endl; 
      mytime.start();  //��ʱ��ʼ  
	  middle.Random(0);//��������
	  cout<<"���ģ�";
	  middle.print();
	  cout<<endl;
      p.GeneratePrime();   //�������� 
      q.GeneratePrime();

      while(p==q)   //�ж������������� 
          q.GeneratePrime();
      temp=p-I;
      t=q-I;
      t=t*temp;  
      cout<<"����pΪ��";   //������� 
      p.print(); 
      cout<<endl;
      cout<<"����qΪ��";
      q.print();
      cout<<endl;
      cout<<"��ԿnΪ";
      n=p*q;
	  //n=p+q;
      n.print();
      cout<<endl;
	  pri_key=Lcm(p-I,q-I);
      //d.ex_euclid(e,t);   //����˽Կd 
      cout<<"˽Կpri_keyΪ:";
      pri_key.print();
      cout<<endl;
	  //do{
		  g.GeneratePrime();
		//  g1.power_mod(pri_key,n*n);
	  //}while(Gcd(g1,n)!=I);
      cout<<"��ԿgΪ";
      g.print();
      mytime.stop(); // ��ʱֹͣ
	  mytime.running();//���������Կ˽Կ�õ���ʱ��
	  cout<<"������Կ˽Կʱ��\t"<<mytime.seconds()<<"\tseconds"<<endl;//���������Կ˽Կ������ʱ��
      //cout<<endl<<"��������Ҫ���ܵ��ַ���  "<<endl<<endl;//��������ʱ�������⣬�����ڴ˲���ϵͳ������ɵ�һ���������мӽ���
      //getline(cin,A);
      mytime.reset();   //ʱ������ 
      mytime.start(); //��ʱ��ʼ
      //a=BigNum(A);
      //cout<<endl;
	  BigNum middle1,r;
	  r.GeneratePrime();
	  middle1=g.power_mod(middle,n*n)*r.power_mod(n,n*n);
	  b=middle1.power_mod(I,n*n);
      //b=middle.power_mod(e,n);   //��������b 
      cout<<"������������:";
      b.print();
      cout<<endl;
	  u.ex_euclid(Lfunction(g.power_mod(pri_key,n*n)),n);
	  c=Lfunction(b.power_mod(pri_key,n*n))*u;
	  c=c.power_mod(I,n);
      //c=b.power_mod(d,n);   //���� 
      cout<<"���ܺ�Ľ��Ϊ:"<<endl;
      c.print();
      mytime.stop(); // ��ʱֹͣ
	  mytime.running();//������������õ���ʱ��
	  cout<<endl<<"���ܽ���ʱ��\t"<<mytime.seconds()<<"\tseconds"<<endl;//������ܽ���������ʱ��
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