#include<ctime>
#include"BigNumber.h"
#include "Timer.h"
using namespace std;


BigNum I=1,o=0,e=2;


//��Կ�������̣�����ѡ��һ����p�Լ�����С��p�������g��gΪp��ԭ������x������y=g^x mod p.��(y,g,p)��Ϊ������Կ��x��Ϊ������Կ

int main()
{
      BigNum p,q,I(1),e(2),O(0),middle,g,x,y,k,k1,C1,C2,M1,M2,M; 
	  //BigNum test[20000];
	  Timer time;
	  time.start();
      //string A;
	  //int middle;
      
      cout<<"*********************************************************************"<<endl;
      cout<<"*                         ElGamal��Կ��������                             *"<<endl;     
      cout<<"*********************************************************************"<<endl<<endl;
        
	  middle.Random(0);//��������
	  cout<<"���ģ�"<<endl;
	  middle.print();
	  cout<<endl;
	  p.GeneratePrime();
	  g.Random(p-I);
	  //��������p������������p��ԭ��g
      /*do
	  {
		  cout<<"1"<<endl;
		  q.GeneratePrime();   //��������q 
		  p=q*e+I;
	  }while(p.Rabin_Miller()!=1);
	  cout<<"��һ��do-whileѭ��"<<endl;
      do
	  {
		  cout<<"2"<<endl;
		  g.Random(p-I);
	  }while((g.power_mod(e,p)==I)||(g.power_mod(q,p)==I));//�Ѿ��������ԭ��Ϊg 
	  cout<<"�ڶ���do-whileѭ��"<<endl;*/
      cout<<"����pΪ��"<<endl;   //������� 
      p.print(); 
      cout<<endl;
      cout<<"p��ԭ��gΪ��"<<endl;
      g.print();
      cout<<endl;
	  x.Random(p);
	  y=g.power_mod(x,p);
	  cout<<"��ԿyΪ��"<<endl;
	  y.print();
	  cout<<endl;
      //d.ex_euclid(e,t);   //����˽Կd 
      cout<<"˽ԿxΪ"<<endl;
      x.print();
      cout<<endl;
      //a=BigNum(A);
      //cout<<endl;
      //��ʼ��������middle�����ܹ��̣���������������ϢM�����ѡһ��p-1���ص�����k������C1=g^k mod p��C2=y^k*M mod p,����ΪC=(C1��C2)

	  k.GeneratePrime();//���������õ������k
	  C1=g.power_mod(k,p);//������һ������C1
	  C2=y.power_mod(k,p);
	  C2=(C2*middle)%p;//�����ڶ�������C2
	  cout<<"������ģ�"<<endl;
	  cout<<"���C1:"<<endl;
	  C1.print();
	  cout<<endl;
	  cout<<"���C2:"<<endl;
	  C2.print();
	  cout<<endl;
	  //���ܽ���
	  //��ʼ��������C1��C2
	  M1=C1.power_mod(x,p);
      M2.ex_euclid(M1,p);
	  /*test=(M1*M2)%p;
	  cout<<"���test"<<endl;
	  test.print();
	  cout<<endl;*/
	  M=(C2*M2)%p;
	  cout<<"������ܺ�����ģ�"<<endl;
	  M.print();
	  cout<<endl;
	  time.stop();
	  time.running();
	  cout<<endl<<"���ܽ���ʱ��\t"<<time.seconds()<<"\tseconds"<<endl;
	  
      system("pause");
}
