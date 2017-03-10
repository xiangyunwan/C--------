#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>
#include<string.h>


#define p_num 16                 //������ܵ�0-1����λ��Ϊp_num,������Ӧ�ĸ���


void increase(int num,long a[])    //numΪ���������еĸ���������aΪ������ɵĽ������������ֻ��0,num-1��������
{
	int i,sum;
	srand((unsigned)time(NULL));    //ϵͳ���Ӳ�������������ɼ��ܵĳ�������������
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

//�������ַ���λ��ȡ���洢��out[]��
void binary(short num,char *out)
{
	short a=1<<15;              //�õ�15λ��Ϊ0
	char ch;
	int i=0;
	for(;i<16;i++)
	{
		ch=(num&a)?'1':'0';
		out[i]=ch;
		num<<=1;                //�ôε�λ������λ
	}
}
          
//������ʵ��
int main()
{
	long sk[p_num]={0},pk[p_num]={0},b[2]={0,1};
	short buffer[20000],z[16],d_text[20000]={0};
	int w,N,i,j=0,k=0;
	long inv_w,len=0,d1=0,d2=0;
	unsigned __int64 temp1,temp2,mid,t_text[20000]={0};             //temp1,temp2������������
	char fin_name[40],fout1_name[40],apt[16];
	char fout2_name[40];
	__int64 c_text[20000]={0},e_text[20000]={0};
	FILE *fp,*fq,*fr;
	clock_t t_start,t_end;
	memset(buffer,0x00,sizeof(buffer));






	//��¼��������ʱ�俪ʼ

	printf("\n********************************$��ʾ����ʼ$*******************************\n");
	
    //---------------------------------------------------pk[],sk[],w,in_w,N������
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
	//------------------------------------------------��Ԫ���
	printf("����w����ģ��N����Ԫinv_w=%d\n",inv_w);
	
	//ͨ��M-H�任�����Կpk[]
	for(j=0;j<p_num;j++)
	{
		mid=w*sk[j];
		pk[j]=mid%N;
	}
	printf("���ɵĹ�Կ����Ϊ��");
	//-------------------------------------------------��Կ�������
	for(j=0;j<p_num;j++)
	{
		printf("%d  ",pk[j]);
	}
	printf("\n");

    //------------------------------------------------------------��ʼ�ļ��Ķ�ȡ�����
	//���ܷ�����ʵ�ֹ���
	printf("������������ļ����ļ����ʹ洢·��,(eg:c:\\test\\a.txt):\n");       //�����ļ�������
	gets(fin_name);
	fp=fopen(fin_name,"rb");
	if(fp==NULL)
	{
		puts("�������ļ��Ҳ������˳�.\n");
		getchar();
		exit(0);
	}
	fclose(fp);
	puts("��������ܺ���ļ��Ĵ洢·��,(eg:c\\test\\a_encode.txt):");       //�����ļ��Ĵ洢·��
	gets(fout1_name);
	fq=fopen(fout1_name,"wb+");
	if(fp==NULL)
	{
		puts("�����ļ����ڴ���...\n");
		getchar();
	}
	fclose(fq);
	puts("�ļ�������...\n");
	t_start=clock();
	fp=fopen(fin_name,"rb");
	fq=fopen(fout1_name,"wb+");
	len=fread(buffer,2,10005,fp);
	//printf("�ܹ�������ֽ���=%ld\n",len);
	
	//puts("\n");
	for(k=0;k<len;k++)
	{
		binary(buffer[k],apt);
		         //buffer[k]�е�ÿһλ�������apt[0]��apt[15]֮��
		for(i=0;i<p_num;i++)
		{
			//printf("%d  ",apt[i]-48);      //��������
			c_text[k]+=(apt[i]-48)*pk[i];
			
		}
		//printf("%x  \n",c_text[k]);          //��������
	}
	fwrite(c_text,8,len,fq);
	puts("�����ļ��Ѿ������������Ѿ����.\n");
    puts("--------------------------------------------------------");
	fclose(fp);
	fclose(fq);
	t_end=clock();
	printf("��������ʱ���ܼ�(��λ��ms)��");
	printf("%lu\n",t_end-t_start);
	puts("�����������ļ��Ĵ洢·��,(eg:c\\test\\a_encode.txt):");       //�����ļ��Ĵ洢·��
	gets(fout1_name);
	puts("��������ܺ���ļ��Ĵ洢·��,(eg:c\\test\\a_decode.txt):");        //�����ļ��Ĵ洢·��
	gets(fout2_name);
	fr=fopen(fout2_name,"wb+");
	if(fr==NULL)
	{
		puts("�����ļ����ڴ���...\n");
		getchar();
	}
	fclose(fr);
	t_start=clock();
	//�ļ�������ɣ�д������
	//fwrite(c_text,8,len,fq);

	//-----------------------------------------------------------�Ӽ����ļ��ж�ȡ����
	fq=fopen(fout1_name,"rb");
	fread(e_text,8,len,fq);
	fr=fopen(fout2_name,"wb+");
	/*for(i=0;i<len;i++)
	{
	    printf("%x ",e_text[i]);                      //��������
	}
    printf("\n");*/


	//-----------------------------------------------------------��ʼ�ļ��Ķ�ȡ�����

	for(k=0;k<len;k++)
	{
		//��ֹ���������������ʱ����temp1,temp2
        temp1=inv_w%N;
	    //temp=(inv_w*c_text)%N;                       //���Գ�������
	    //printf("inv_w%N=%ld\n",temp1);
	    //printf("%x \n",c_text[0]);
	    temp2=e_text[k]%N;
	    //printf("%x \n",temp2);
 	    t_text[k]=temp1*temp2; 
	    //printf("temp=%I64d\n",temp);
	    t_text[k]=t_text[k]%N;        //��ʱ�м��������
		//printf("%x ",temp);  //��������
		for(i=p_num-1;i>=0;i--)
		{
			if(t_text[k]<sk[i])
				z[i]=0;
			else
			{
				z[i]=1;
				t_text[k]=t_text[k]-sk[i];
			}   //ÿ��0-1����������
		}
		/*for(i=0;i<p_num;i++)
		{
			printf("%d  ",z[i]);                    //���������
		}*/                                         //��������
		//�����ַ�ʮ���ƴ洢��d_text������
		for(j=0;j<p_num;j++)
		{
			d_text[k]+=pow(2,p_num-j-1)*z[j];
		}
		//printf("%x ",d_text[k]);                   //��������
		//printf("\n");                              //��������
	}
	puts("--------------------------------------------------------");
	puts("������ɣ�\n");

//--------------------------------------------------------�����ļ�д��
	fwrite(d_text,2,len,fr);
	puts("�����ļ��Ѿ������������Ѿ����.\n");
	
	//��¼��������ʱ�����
	t_end=clock();
	printf("��������ʱ���ܼ�(��λ��ms)��");
	printf("%lu\n",t_end-t_start);
	printf("\n********************************$��ʾ�������$*******************************\n");
	system("pause");
}