#include<stdio.h>
#include<time.h>
#include<stdlib.h>
#include"����.h"
int main()
{
	char filename[1024],pub_name[1024],pri_name[1024],output[1024];
	int tag;
	__int64 n;
	while(1)
	{
		puts("1.����Կ��\n2.����\n3.����");
		scanf("%d",&tag);
		time_t start;
		switch(tag)
		{
			case 1:
				puts("�������Ź�Կ���ļ���:");
				fflush(stdin);
				gets(pub_name);
				puts("��������˽Կ���ļ���:");
				gets(pri_name);
				start=time(0);
				RSA::Create_Key(pub_name,pri_name);
				//printf("%g\n",difftime(time(0),start));
				break;
			case 2:
				puts("�������Ź�Կ���ļ���:");
				fflush(stdin);
				gets(pub_name);
				//puts("����Ҫ���ܵ�����");
				//scanf("%I64d",&n);
				//printf("���ܽ��Ϊ��%d\n",RSA::Encrypt_Int16(pub_name,n));
				puts("������Ҫ���ܵ��ļ���:");
				gets(filename);
				puts("������������ļ���:");
				gets(output);
				start=time(0);
				RSA::Encrypt(filename,output,pub_name);
			//	printf("%g\n",difftime(time(0),start));
				
				break;
			case 3:
				puts("��������˽Կ���ļ���:");
				fflush(stdin);
				gets(pri_name);
				//puts("����Ҫ���ܵ�����");
				//scanf("%I64d",&n);
				//printf("���ܽ��Ϊ��%d\n",RSA::DeEncrypt_Int16(pri_name,n));
				puts("������Ҫ���ܵ��ļ���:");
				gets(filename);
				puts("������������ļ���:");
				gets(output);
				start=time(0);
				RSA::DeEncrypt(filename,output,pri_name);
			//	printf("%g\n",difftime(time(0),start));
				
				break;
		}
	}
	return 0;
}