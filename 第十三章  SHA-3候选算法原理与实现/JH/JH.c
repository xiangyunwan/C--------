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
		// 将指针定位到文件末尾
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

uint8_t h[28];//消息摘要
uint8_t ml[1024000];//1000kB长消息
puts("================================================================\n");
puts("                        J-H算法演示\n");
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
	printf("计算长度为%dKB消息的摘要值所需的时间为：%f秒\n",i,duration);
	printf("摘要值为：\n");
	for(j=0;j<28;j++)
	{
		printf("%02x",h[j]);
	}
	printf("\n\n");

}
//计算文件的Hash值
printf("计算文件test.txt的摘要值\n");
if((fp=fopen("test.txt","r"))==NULL)
printf("Open File Error！\n");
else
//printf ("文件的长度为：%ld\n",filesize(fp));
{
	filelen=filesize(fp);
	printf("文件的大小为：%ldB\n",filelen);
	for(i=0;i<filelen;i++)
	{
		fread(ml,1,1,fp);
	}
	fclose(fp);
	start=clock();
	Hash(224, ml,filelen*8, h);
	finish=clock();
	duration=(double)(finish-start)/1000;
	printf("文件的摘要值为：\n");
	for(j=0;j<28;j++)
	{
		printf("%02x",h[j]);
	}
	printf("\n");
	printf("计算文件的摘要值所花费的时间为：%f秒\n",duration);
}
system("pause");

}

