#include<stdio.h>
#include<time.h>
#include<stdlib.h>
#include"加密.h"
int main()
{
	char filename[1024],pub_name[1024],pri_name[1024],output[1024];
	int tag;
	__int64 n;
	while(1)
	{
		puts("1.生成钥匙\n2.加密\n3.解密");
		scanf("%d",&tag);
		time_t start;
		switch(tag)
		{
			case 1:
				puts("请输入存放公钥的文件名:");
				fflush(stdin);
				gets(pub_name);
				puts("请输入存放私钥的文件名:");
				gets(pri_name);
				start=time(0);
				RSA::Create_Key(pub_name,pri_name);
				//printf("%g\n",difftime(time(0),start));
				break;
			case 2:
				puts("请输入存放公钥的文件名:");
				fflush(stdin);
				gets(pub_name);
				//puts("输入要加密的整数");
				//scanf("%I64d",&n);
				//printf("加密结果为：%d\n",RSA::Encrypt_Int16(pub_name,n));
				puts("请输入要加密的文件名:");
				gets(filename);
				puts("请输入输出的文件名:");
				gets(output);
				start=time(0);
				RSA::Encrypt(filename,output,pub_name);
			//	printf("%g\n",difftime(time(0),start));
				
				break;
			case 3:
				puts("请输入存放私钥的文件名:");
				fflush(stdin);
				gets(pri_name);
				//puts("输入要解密的整数");
				//scanf("%I64d",&n);
				//printf("解密结果为：%d\n",RSA::DeEncrypt_Int16(pri_name,n));
				puts("请输入要解密的文件名:");
				gets(filename);
				puts("请输入输出的文件名:");
				gets(output);
				start=time(0);
				RSA::DeEncrypt(filename,output,pri_name);
			//	printf("%g\n",difftime(time(0),start));
				
				break;
		}
	}
	return 0;
}