#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>


#define p_num 5                 //定义加密的0-1序列位数为p_num,可以相应的更改


void increase(int num,long a[])    //num为超递增数列的个数，数组a为存放生成的结果，且数组中只有0,num-1是有用数
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

//求一个数w在MOD N 下的逆元w-1函数；
int inv(int xin,int n0)        //求xin*xin-1=1 mod n0
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

//求两个数的最大公约数函数，该函数可用于检测两个数是否互素；
int gcd(long a,long b)
{
	long t;
    t=a;
    while(!((a%t==0)&&(b%t==0))) t--;
    return t;                          //t=1时，a与b互素，否则不互素。
}

//整数N和w选择函数；
void select_n_w(int num,long s[],int b[])     /* num为超递增的个数,数组s为调用时传超递增数组的地址，
                                         数组b为存放N和w,其中b[0]存放N,b[1]存放w   */
{
    b[0]=2*s[num-1]+rand()%num+1;       //b[0]存放N，作为模数N
    while(1)                             //在1~9999的范围内寻找合适的乘数w
	{
		b[1]=rand()%9999+1;              //w作为乘数，小于9999，存放在b[1]中
		if(gcd(b[1],b[0])==1) break;     //w与N互素
   }
}
          
//主函数实现
void main()
{
	int p_text[p_num]={0},inv_p_text[p_num]={0}; //为了方便演示，p_test[p_num]为既定赋值，实际中可以更改
	long sk[p_num]={0},pk[p_num]={0},b[2]={0,1};
	int w,N,i,j=0;
	long inv_w;
	unsigned __int64 temp,mid,temp1,temp2;
	long c_text=0;
	clock_t t_start,t_end;
	printf("请输入%d位的0-1明文信息序列(0-1代码以“，”隔开)：\n",p_num);       //明文输入提示为0-1序列
	for(i=0;i<p_num;i++)                                 //实际中的直接0-1明文输入
	{
		scanf("%d,",&p_text[i]);
	}

	//记录程序运行时间开始
	t_start=clock();
	printf("\n**********************$演示程序开始$**********************\n");
	printf("输入的%d位0-1明文信息序列为：\n",p_num);
	for(i=0;i<p_num;i++)
	{
		printf("%d",p_text[i]);
	}
	printf("\n");

	//私钥sk[]-超递增序列的生成,存放在sk[]中
	increase(p_num,sk);
	printf("生成的私钥序列为：");
	for(j=0;j<p_num;j++)
	{
		printf("%d ",sk[j]);
	}
	printf("\n");
	
	//选择合适的模数N和乘数w,存放在b[2]中
	select_n_w(p_num,sk,b);
	w=b[1];N=b[0];
	printf("乘数w=%d,模数N=%d,",w,N);
	
	//求乘数w关于模数N的逆元w^(-1);
	inv_w=inv(w,N);
	printf("乘数w关于模数N的逆元inv_w=%d\n",inv_w);
	
	//通过M-H变换求出公钥pk[]
	for(j=0;j<p_num;j++)
	{
		mid=w*sk[j];
		pk[j]=mid%N;
	}
	printf("生成的公钥序列为：");
	for(j=0;j<p_num;j++)
	{
		printf("%d  ",pk[j]);
	}
	printf("\n");

	//加密方案的实现过程
	for(i=0;i<p_num;i++)
	{
		c_text+=pk[i]*p_text[i];
	}
	
	//输出加密后的密文
	printf("加密后的密文形式为：%ld\n",c_text);
	
	//解密方案的实现过程与输出结果
	temp1=inv_w%N;
	//temp=(inv_w*c_text)%N;                       //测试程序所用
	//printf("inv_w%N=%ld\n",temp1);
	temp2=c_text%N;
	//printf("c_text%N=%ld\n",temp2);
	temp=temp1*temp2; 
	//printf("temp=%I64d\n",temp);
	temp=temp%N;
	//printf("temp=%ld\n",temp);
	printf("输出解密后的明文序列：\n");
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

	//记录程序运行时间结束
	t_end=clock();
	printf("程序运行时间总计(单位：ms)：");
	printf("%lu\n",t_end-t_start);
	printf("\n**********************$演示程序结束$**********************\n");
	system("pause");
}