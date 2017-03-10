#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>
#include<string.h>


#define p_num 16                 //定义加密的0-1序列位数为p_num,可以相应的更改


void increase(int num,long a[])    //num为超递增数列的个数，数组a为存放生成的结果，且数组中只有0,num-1是有用数
{
	int i,sum;
	srand((unsigned)time(NULL));    //系统种子播种器，随机生成加密的超递增背包序列
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

//二进制字符按位读取，存储在out[]中
void binary(short num,char *out)
{
	short a=1<<15;              //让低15位变为0
	char ch;
	int i=0;
	for(;i<16;i++)
	{
		ch=(num&a)?'1':'0';
		out[i]=ch;
		num<<=1;                //让次低位变成最高位
	}
}
          
//主函数实现
int main()
{
	long sk[p_num]={0},pk[p_num]={0},b[2]={0,1};
	short buffer[20000],z[16],d_text[20000]={0};
	int w,N,i,j=0,k=0;
	long inv_w,len=0,d1=0,d2=0;
	unsigned __int64 temp1,temp2,mid,t_text[20000]={0};             //temp1,temp2可做测试所用
	char fin_name[40],fout1_name[40],apt[16];
	char fout2_name[40];
	__int64 c_text[20000]={0},e_text[20000]={0};
	FILE *fp,*fq,*fr;
	clock_t t_start,t_end;
	memset(buffer,0x00,sizeof(buffer));






	//记录程序运行时间开始

	printf("\n********************************$演示程序开始$*******************************\n");
	
    //---------------------------------------------------pk[],sk[],w,in_w,N的生成
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
	//------------------------------------------------逆元输出
	printf("乘数w关于模数N的逆元inv_w=%d\n",inv_w);
	
	//通过M-H变换求出公钥pk[]
	for(j=0;j<p_num;j++)
	{
		mid=w*sk[j];
		pk[j]=mid%N;
	}
	printf("生成的公钥序列为：");
	//-------------------------------------------------公钥序列输出
	for(j=0;j<p_num;j++)
	{
		printf("%d  ",pk[j]);
	}
	printf("\n");

    //------------------------------------------------------------开始文件的读取与加密
	//加密方案的实现过程
	printf("请输入待加密文件的文件名和存储路径,(eg:c:\\test\\a.txt):\n");       //加密文件的输入
	gets(fin_name);
	fp=fopen(fin_name,"rb");
	if(fp==NULL)
	{
		puts("待加密文件找不到！退出.\n");
		getchar();
		exit(0);
	}
	fclose(fp);
	puts("请输入加密后的文件的存储路径,(eg:c\\test\\a_encode.txt):");       //加密文件的存储路径
	gets(fout1_name);
	fq=fopen(fout1_name,"wb+");
	if(fp==NULL)
	{
		puts("加密文件正在创建...\n");
		getchar();
	}
	fclose(fq);
	puts("文件加密中...\n");
	t_start=clock();
	fp=fopen(fin_name,"rb");
	fq=fopen(fout1_name,"wb+");
	len=fread(buffer,2,10005,fp);
	//printf("总共读入的字节数=%ld\n",len);
	
	//puts("\n");
	for(k=0;k<len;k++)
	{
		binary(buffer[k],apt);
		         //buffer[k]中的每一位都存放在apt[0]到apt[15]之中
		for(i=0;i<p_num;i++)
		{
			//printf("%d  ",apt[i]-48);      //测试所用
			c_text[k]+=(apt[i]-48)*pk[i];
			
		}
		//printf("%x  \n",c_text[k]);          //测试所用
	}
	fwrite(c_text,8,len,fq);
	puts("加密文件已经创建，加密已经完成.\n");
    puts("--------------------------------------------------------");
	fclose(fp);
	fclose(fq);
	t_end=clock();
	printf("加密运行时间总计(单位：ms)：");
	printf("%lu\n",t_end-t_start);
	puts("请输入密文文件的存储路径,(eg:c\\test\\a_encode.txt):");       //加密文件的存储路径
	gets(fout1_name);
	puts("请输入解密后的文件的存储路径,(eg:c\\test\\a_decode.txt):");        //解密文件的存储路径
	gets(fout2_name);
	fr=fopen(fout2_name,"wb+");
	if(fr==NULL)
	{
		puts("解密文件正在创建...\n");
		getchar();
	}
	fclose(fr);
	t_start=clock();
	//文件加密完成，写出即可
	//fwrite(c_text,8,len,fq);

	//-----------------------------------------------------------从加密文件中读取测试
	fq=fopen(fout1_name,"rb");
	fread(e_text,8,len,fq);
	fr=fopen(fout2_name,"wb+");
	/*for(i=0;i<len;i++)
	{
	    printf("%x ",e_text[i]);                      //测试所用
	}
    printf("\n");*/


	//-----------------------------------------------------------开始文件的读取与解密

	for(k=0;k<len;k++)
	{
		//防止数据溢出，引入临时变量temp1,temp2
        temp1=inv_w%N;
	    //temp=(inv_w*c_text)%N;                       //测试程序所用
	    //printf("inv_w%N=%ld\n",temp1);
	    //printf("%x \n",c_text[0]);
	    temp2=e_text[k]%N;
	    //printf("%x \n",temp2);
 	    t_text[k]=temp1*temp2; 
	    //printf("temp=%I64d\n",temp);
	    t_text[k]=t_text[k]%N;        //临时中间数组变量
		//printf("%x ",temp);  //测试所用
		for(i=p_num-1;i>=0;i--)
		{
			if(t_text[k]<sk[i])
				z[i]=0;
			else
			{
				z[i]=1;
				t_text[k]=t_text[k]-sk[i];
			}   //每组0-1代码解码完成
		}
		/*for(i=0;i<p_num;i++)
		{
			printf("%d  ",z[i]);                    //代码检测输出
		}*/                                         //测试所用
		//解码字符十进制存储在d_text数组中
		for(j=0;j<p_num;j++)
		{
			d_text[k]+=pow(2,p_num-j-1)*z[j];
		}
		//printf("%x ",d_text[k]);                   //测试所用
		//printf("\n");                              //测试所用
	}
	puts("--------------------------------------------------------");
	puts("解密完成！\n");

//--------------------------------------------------------解密文件写出
	fwrite(d_text,2,len,fr);
	puts("解密文件已经创建，解密已经完成.\n");
	
	//记录程序运行时间结束
	t_end=clock();
	printf("解密运行时间总计(单位：ms)：");
	printf("%lu\n",t_end-t_start);
	printf("\n********************************$演示程序结束$*******************************\n");
	system("pause");
}