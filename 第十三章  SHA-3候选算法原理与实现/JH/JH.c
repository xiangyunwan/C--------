#include <string.h>
#include <time.h>
#include "jh_ref.h" 
#include <stdio.h>
    
long filesize(FILE * pFile)
{
	long fileSize = 0;
	if (pFile == NULL)
	{
		printf("Open File Error\n" );
		return 0;
	}
	else
	{
		// ��ָ�붨λ���ļ�ĩβ
		fseek(pFile, 0L, SEEK_END);
		fileSize = ftell(pFile);
		rewind(pFile);
		return fileSize;
	}
}
main()
{
FILE* fp;
long filelen;
int i,j;	
clock_t start,finish;
double duration;

uint8_t h[28];//��ϢժҪ
uint8_t ml[1024000];//1000kB����Ϣ
puts("================================================================\n");
puts("                        J-H�㷨��ʾ\n");
puts("================================================================\n");
for(i=0;i<1024000;i++)
{
	ml[i]=0x00;
}
for(i=100;i<=500;i=i+200)
{
	start=clock();

	Hash(224, ml,1024*8*i, h);

	finish=clock();
	duration=(double)(finish-start)/1000;
	printf("���㳤��Ϊ%dKB��Ϣ��ժҪֵ�����ʱ��Ϊ��%f��\n",i,duration);
	printf("ժҪֵΪ��\n");
	for(j=0;j<28;j++)
	{
		printf("%02x",h[j]);
	}
	printf("\n\n");

}
//�����ļ���Hashֵ
printf("�����ļ�test.txt��ժҪֵ\n");
if((fp=fopen("test.txt","r"))==NULL)
printf("Open File Error��\n");
else
//printf ("�ļ��ĳ���Ϊ��%ld\n",filesize(fp));
{
	filelen=filesize(fp);
	printf("�ļ��Ĵ�СΪ��%ldB\n",filelen);
	for(i=0;i<filelen;i++)
	{
		fread(ml,1,1,fp);
	}
	fclose(fp);
	start=clock();
	Hash(224, ml,filelen*8, h);
	finish=clock();
	duration=(double)(finish-start)/1000;
	printf("�ļ���ժҪֵΪ��\n");
	for(j=0;j<28;j++)
	{
		printf("%02x",h[j]);
	}
	printf("\n");
	printf("�����ļ���ժҪֵ�����ѵ�ʱ��Ϊ��%f��\n",duration);
}
system("pause");

}

