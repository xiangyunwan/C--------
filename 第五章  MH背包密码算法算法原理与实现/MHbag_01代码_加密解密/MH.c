#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>


#define p_num 5                 //������ܵ�0-1����λ��Ϊp_num,������Ӧ�ĸ���


void increase(int num,long a[])    //numΪ���������еĸ���������aΪ������ɵĽ������������ֻ��0,num-1��������
{
	int i,sum;
    a[0]=1+rand()%num;
    sum=a[0];
    for(i=1;i<num;i++)
	{ 
	    a[i]=sum+1+rand()%num;
        sum=sum+a[i];
	}
}

//��һ����w��MOD N �µ���Ԫw-1������
int inv(int xin,int n0)        //��xin*xin-1=1 mod n0
{
	long n1,n2,q,r,b1,b2,t;
    if(xin==0)
       b2=0;
    else
	{ 
		n1=n0;n2=xin;b2=1;b1=0;
        do{
			r=(n1%n2);
            q=(n1-r)/n2;
            if(r==0)
			{
				if(b2<0) b2=n0+b2;
			}
            else
			{
				n1=n2;
				n2=r;
	            t=b2;
	            b2=b1-q*b2;
				b1=t;
			}

		}while(r!=0);
    }
	return(b2);
}

//�������������Լ���������ú��������ڼ���������Ƿ��أ�
int gcd(long a,long b)
{
	long t;
    t=a;
    while(!((a%t==0)&&(b%t==0))) t--;
    return t;                          //t=1ʱ��a��b���أ����򲻻��ء�
}

//����N��wѡ������
void select_n_w(int num,long s[],int b[])     /* numΪ�������ĸ���,����sΪ����ʱ������������ĵ�ַ��
                                         ����bΪ���N��w,����b[0]���N,b[1]���w   */
{
    b[0]=2*s[num-1]+rand()%num+1;       //b[0]���N����Ϊģ��N
    while(1)                             //��1~9999�ķ�Χ��Ѱ�Һ��ʵĳ���w
	{
		b[1]=rand()%9999+1;              //w��Ϊ������С��9999�������b[1]��
		if(gcd(b[1],b[0])==1) break;     //w��N����
   }
}
          
//������ʵ��
void main()
{
	int p_text[p_num]={0},inv_p_text[p_num]={0}; //Ϊ�˷�����ʾ��p_test[p_num]Ϊ�ȶ���ֵ��ʵ���п��Ը���
	long sk[p_num]={0},pk[p_num]={0},b[2]={0,1};
	int w,N,i,j=0;
	long inv_w;
	unsigned __int64 temp,mid,temp1,temp2;
	long c_text=0;
	clock_t t_start,t_end;
	printf("������%dλ��0-1������Ϣ����(0-1�����ԡ���������)��\n",p_num);       //����������ʾΪ0-1����
	for(i=0;i<p_num;i++)                                 //ʵ���е�ֱ��0-1��������
	{
		scanf("%d,",&p_text[i]);
	}

	//��¼��������ʱ�俪ʼ
	t_start=clock();
	printf("\n**********************$��ʾ����ʼ$**********************\n");
	printf("�����%dλ0-1������Ϣ����Ϊ��\n",p_num);
	for(i=0;i<p_num;i++)
	{
		printf("%d",p_text[i]);
	}
	printf("\n");

	//˽Կsk[]-���������е�����,�����sk[]��
	increase(p_num,sk);
	printf("���ɵ�˽Կ����Ϊ��");
	for(j=0;j<p_num;j++)
	{
		printf("%d ",sk[j]);
	}
	printf("\n");
	
	//ѡ����ʵ�ģ��N�ͳ���w,�����b[2]��
	select_n_w(p_num,sk,b);
	w=b[1];N=b[0];
	printf("����w=%d,ģ��N=%d,",w,N);
	
	//�����w����ģ��N����Ԫw^(-1);
	inv_w=inv(w,N);
	printf("����w����ģ��N����Ԫinv_w=%d\n",inv_w);
	
	//ͨ��M-H�任�����Կpk[]
	for(j=0;j<p_num;j++)
	{
		mid=w*sk[j];
		pk[j]=mid%N;
	}
	printf("���ɵĹ�Կ����Ϊ��");
	for(j=0;j<p_num;j++)
	{
		printf("%d  ",pk[j]);
	}
	printf("\n");

	//���ܷ�����ʵ�ֹ���
	for(i=0;i<p_num;i++)
	{
		c_text+=pk[i]*p_text[i];
	}
	
	//������ܺ������
	printf("���ܺ��������ʽΪ��%ld\n",c_text);
	
	//���ܷ�����ʵ�ֹ�����������
	temp1=inv_w%N;
	//temp=(inv_w*c_text)%N;                       //���Գ�������
	//printf("inv_w%N=%ld\n",temp1);
	temp2=c_text%N;
	//printf("c_text%N=%ld\n",temp2);
	temp=temp1*temp2; 
	//printf("temp=%I64d\n",temp);
	temp=temp%N;
	//printf("temp=%ld\n",temp);
	printf("������ܺ���������У�\n");
	for(i=p_num-1;i>=0;i--)
	{
		if(temp<sk[i])
			inv_p_text[i]=0;
		else
		{
			inv_p_text[i]=1;
			temp-=sk[i];
		}
	}
	for(j=0;j<p_num;j++)
		printf("%d",inv_p_text[j]);
	printf("\n");

	//��¼��������ʱ�����
	t_end=clock();
	printf("��������ʱ���ܼ�(��λ��ms)��");
	printf("%lu\n",t_end-t_start);
	printf("\n**********************$��ʾ�������$**********************\n");
	system("pause");
}