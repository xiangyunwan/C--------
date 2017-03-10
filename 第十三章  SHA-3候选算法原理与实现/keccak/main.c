// main.c
// 19-Nov-11  Markku-Juhani O. Saarinen <mjos@iki.fi>

#include <stdio.h>
#include <string.h>
#include <time.h>
#include "keccak.h"

// test each length

typedef struct {
    int mdlen;
    char *msgstr;
    uint8_t md[64];
} test_triplet_t;

// returns zero on success, nonzero + stderr messages on failure

int keccak_test()
{
    test_triplet_t testvec[4] = {   
        {
            28, "Keccak-224 Test Hash", {
                0x30, 0x04, 0x5B, 0x34, 0x94, 0x6E, 0x1B, 0x2E, 
                0x09, 0x16, 0x13, 0x36, 0x2F, 0xD2, 0x2A, 0xA0, 
                0x8E, 0x2B, 0xEA, 0xFE, 0xC5, 0xE8, 0xDA, 0xEE, 
                0x42, 0xC2, 0xE6, 0x65 }
        }, {
            32, "Keccak-256 Test Hash", {
                0xA8, 0xD7, 0x1B, 0x07, 0xF4, 0xAF, 0x26, 0xA4, 
                0xFF, 0x21, 0x02, 0x7F, 0x62, 0xFF, 0x60, 0x26, 
                0x7F, 0xF9, 0x55, 0xC9, 0x63, 0xF0, 0x42, 0xC4, 
                0x6D, 0xA5, 0x2E, 0xE3, 0xCF, 0xAF, 0x3D, 0x3C }
        }, {
            48, "Keccak-384 Test Hash", {
                0xE2, 0x13, 0xFD, 0x74, 0xAF, 0x0C, 0x5F, 0xF9, 
                0x1B, 0x42, 0x3C, 0x8B, 0xCE, 0xEC, 0xD7, 0x01, 
                0xF8, 0xDD, 0x64, 0xEC, 0x18, 0xFD, 0x6F, 0x92, 
                0x60, 0xFC, 0x9E, 0xC1, 0xED, 0xBD, 0x22, 0x30, 
                0xA6, 0x90, 0x86, 0x65, 0xBC, 0xD9, 0xFB, 0xF4, 
                0x1A, 0x99, 0xA1, 0x8A, 0x7D, 0x9E, 0x44, 0x6E }
        }, {
            64, "Keccak-512 Test Hash", {
                0x96, 0xEE, 0x47, 0x18, 0xDC, 0xBA, 0x3C, 0x74, 
                0x61, 0x9B, 0xA1, 0xFA, 0x7F, 0x57, 0xDF, 0xE7, 
                0x76, 0x9D, 0x3F, 0x66, 0x98, 0xA8, 0xB3, 0x3F, 
                0xA1, 0x01, 0x83, 0x89, 0x70, 0xA1, 0x31, 0xE6, 
                0x21, 0xCC, 0xFD, 0x05, 0xFE, 0xFF, 0xBC, 0x11, 
                0x80, 0xF2, 0x63, 0xC2, 0x7F, 0x1A, 0xDA, 0xB4, 
                0x60, 0x95, 0xD6, 0xF1, 0x25, 0x33, 0x14, 0x72, 
                0x4B, 0x5C, 0xBF, 0x78, 0x28, 0x65, 0x8E, 0x6A }
        }
    };
    int i, fails;
    uint8_t md[64];

    fails = 0;
    for (i = 0; i < 4; i++) {
    
        keccak((uint8_t *) testvec[i].msgstr, 
            strlen(testvec[i].msgstr),
            md, testvec[i].mdlen);

        if (memcmp(md, testvec[i].md, testvec[i].mdlen)) {
            fails++;
            fprintf(stderr, "Keccak-%d FAILED.", testvec[i].mdlen * 8);
        }
    }

    return fails;
}

// main

/*int main(int argc, char **argv)
{
    if (keccak_test() == 0)
        printf("Keccak self-test OK!\n");
 
    return 0;

}*/

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

uint8_t h[64];//消息摘要
uint8_t ml[1024000];//1000kB长消息
puts("================================================================\n");
puts("                      Keccak算法演示\n");
puts("================================================================\n");
for(i=0;i<1024000;i++)
{
	ml[i]=0x00;
}
for(i=100;i<=500;i+=200)
{
	start=clock();
	keccak(ml, 1024*i,h,32);
//	keccak(ml, 1024*i,h,64);
	finish=clock();
	duration=(double)(finish-start)/1000;
	printf("计算长度为%dKB消息的摘要值所需的时间为：%f秒\n",i,duration);
	printf("摘要值为：\n");
	for(j=0;j<32;j++)
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
{
	filelen=filesize(fp);
	printf("文件的大小为：%ldB\n",filelen);
	for(i=0;i<filelen;i++)
	{
		fread(ml,1,1,fp);
	}
	fclose(fp);
	start=clock();
	keccak(ml, filelen,h,32);
	finish=clock();
	duration=(double)(finish-start)/1000;
	printf("文件的摘要值为：\n");
	for(j=0;j<32;j++)
	{
		printf("%02x",h[j]);
	}
	printf("\n");
	printf("计算文件的摘要值所花费的时间为：%f秒\n",duration);
}
system("pause");
}
