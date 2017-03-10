#include <stdio.h>
#include <stdlib.h> 
#include <time.h>

#include <string.h>
//读入明文的字符最大数目
#define MAX 1024*1024
//读入的明文字符缓冲区
//unsigned char buffer[MAX];



//附录 A: ZUC算法的C语言实现
/* ——————————————————————- */ 
typedef unsigned char u8;//0~255
typedef unsigned int u32;//
//产生的密钥流
u32 Keystream[MAX];
//读入的明文字符缓冲区
u32 buffer[MAX];
/* ——————————————————————- */ 
/* the state registers of LFSR LFSR的16阶寄存器 */ 
u32 LFSR_S0;
u32 LFSR_S1;
u32 LFSR_S2;
u32 LFSR_S3;
u32 LFSR_S4;
u32 LFSR_S5;
u32 LFSR_S6;
u32 LFSR_S7;
u32 LFSR_S8;
u32 LFSR_S9;
u32 LFSR_S10;
u32 LFSR_S11;
u32 LFSR_S12;
u32 LFSR_S13;
u32 LFSR_S14;
u32 LFSR_S15;

/* the registers of F F函数的两个记忆单元 */
u32 F_R1;
u32 F_R2;

/* the outputs of BitReorganization 比特重组模块的输出 */
u32 BRC_X0;
u32 BRC_X1;
u32 BRC_X2;
u32 BRC_X3;

/* the s-boxes  S盒*/ 
	u8 S0[256] = {
	0x3e,0x72,0x5b,0x47,0xca,0xe0,0x00,0x33,0x04,0xd1,0x54,0x98,0x09,0xb9,0x6d,0xcb,
	0x7b,0x1b,0xf9,0x32,0xaf,0x9d,0x6a,0xa5,0xb8,0x2d,0xfc,0x1d,0x08,0x53,0x03,0x90,
	0x4d,0x4e,0x84,0x99,0xe4,0xce,0xd9,0x91,0xdd,0xb6,0x85,0x48,0x8b,0x29,0x6e,0xac,
	0xcd,0xc1,0xf8,0x1e,0x73,0x43,0x69,0xc6,0xb5,0xbd,0xfd,0x39,0x63,0x20,0xd4,0x38,
	0x76,0x7d,0xb2,0xa7,0xcf,0xed,0x57,0xc5,0xf3,0x2c,0xbb,0x14,0x21,0x06,0x55,0x9b,
	0xe3,0xef,0x5e,0x31,0x4f,0x7f,0x5a,0xa4,0x0d,0x82,0x51,0x49,0x5f,0xba,0x58,0x1c,
	0x4a,0x16,0xd5,0x17,0xa8,0x92,0x24,0x1f,0x8c,0xff,0xd8,0xae,0x2e,0x01,0xd3,0xad,
	0x3b,0x4b,0xda,0x46,0xeb,0xc9,0xde,0x9a,0x8f,0x87,0xd7,0x3a,0x80,0x6f,0x2f,0xc8,
	0xb1,0xb4,0x37,0xf7,0x0a,0x22,0x13,0x28,0x7c,0xcc,0x3c,0x89,0xc7,0xc3,0x96,0x56,
	0x07,0xbf,0x7e,0xf0,0x0b,0x2b,0x97,0x52,0x35,0x41,0x79,0x61,0xa6,0x4c,0x10,0xfe,
	0xbc,0x26,0x95,0x88,0x8a,0xb0,0xa3,0xfb,0xc0,0x18,0x94,0xf2,0xe1,0xe5,0xe9,0x5d,
	0xd0,0xdc,0x11,0x66,0x64,0x5c,0xec,0x59,0x42,0x75,0x12,0xf5,0x74,0x9c,0xaa,0x23,
	0x0e,0x86,0xab,0xbe,0x2a,0x02,0xe7,0x67,0xe6,0x44,0xa2,0x6c,0xc2,0x93,0x9f,0xf1,
	0xf6,0xfa,0x36,0xd2,0x50,0x68,0x9e,0x62,0x71,0x15,0x3d,0xd6,0x40,0xc4,0xe2,0x0f,
	0x8e,0x83,0x77,0x6b,0x25,0x05,0x3f,0x0c,0x30,0xea,0x70,0xb7,0xa1,0xe8,0xa9,0x65,
	0x8d,0x27,0x1a,0xdb,0x81,0xb3,0xa0,0xf4,0x45,0x7a,0x19,0xdf,0xee,0x78,0x34,0x60
}; 

	u8 S1[256] =  {
	0x55,0xc2,0x63,0x71,0x3b,0xc8,0x47,0x86,0x9f,0x3c,0xda,0x5b,0x29,0xaa,0xfd,0x77,
	0x8c,0xc5,0x94,0x0c,0xa6,0x1a,0x13,0x00,0xe3,0xa8,0x16,0x72,0x40,0xf9,0xf8,0x42,
	0x44,0x26,0x68,0x96,0x81,0xd9,0x45,0x3e,0x10,0x76,0xc6,0xa7,0x8b,0x39,0x43,0xe1,
	0x3a,0xb5,0x56,0x2a,0xc0,0x6d,0xb3,0x05,0x22,0x66,0xbf,0xdc,0x0b,0xfa,0x62,0x48,
	0xdd,0x20,0x11,0x06,0x36,0xc9,0xc1,0xcf,0xf6,0x27,0x52,0xbb,0x69,0xf5,0xd4,0x87,
	0x7f,0x84,0x4c,0xd2,0x9c,0x57,0xa4,0xbc,0x4f,0x9a,0xdf,0xfe,0xd6,0x8d,0x7a,0xeb,
	0x2b,0x53,0xd8,0x5c,0xa1,0x14,0x17,0xfb,0x23,0xd5,0x7d,0x30,0x67,0x73,0x08,0x09,
	0xee,0xb7,0x70,0x3f,0x61,0xb2,0x19,0x8e,0x4e,0xe5,0x4b,0x93,0x8f,0x5d,0xdb,0xa9,
	0xad,0xf1,0xae,0x2e,0xcb,0x0d,0xfc,0xf4,0x2d,0x46,0x6e,0x1d,0x97,0xe8,0xd1,0xe9,
	0x4d,0x37,0xa5,0x75,0x5e,0x83,0x9e,0xab,0x82,0x9d,0xb9,0x1c,0xe0,0xcd,0x49,0x89,
	0x01,0xb6,0xbd,0x58,0x24,0xa2,0x5f,0x38,0x78,0x99,0x15,0x90,0x50,0xb8,0x95,0xe4,
	0xd0,0x91,0xc7,0xce,0xed,0x0f,0xb4,0x6f,0xa0,0xcc,0xf0,0x02,0x4a,0x79,0xc3,0xde,
	0xa3,0xef,0xea,0x51,0xe6,0x6b,0x18,0xec,0x1b,0x2c,0x80,0xf7,0x74,0xe7,0xff,0x21,
	0x5a,0x6a,0x54,0x1e,0x41,0x31,0x92,0x35,0xc4,0x33,0x07,0x0a,0xba,0x7e,0x0e,0x34,
	0x88,0xb1,0x98,0x7c,0xf3,0x3d,0x60,0x6c,0x7b,0xca,0xd3,0x1f,0x32,0x65,0x04,0x28,
	0x64,0xbe,0x85,0x9b,0x2f,0x59,0x8a,0xd7,0xb0,0x25,0xac,0xaf,0x12,0x03,0xe2,0xf2
	};
	 
/* the constants D ，16个常规字符串*/
u32 EK_d[16] = {
	0x44D7, 0x26BC, 0x626B, 0x135E, 0x5789, 0x35E2, 0x7135, 0x09AF,
	0x4D78, 0x2F13, 0x6BC4, 0x1AF1, 0x5E26, 0x3C4D, 0x789A, 0x47AC
};

/* ——————————————————————- */ 
/* c = a + b mod (2^31 – 1) */
u32 AddM(u32 a, u32 b)
{
	u32 c = a + b;
	return (c & 0x7FFFFFFF) + (c >> 31);  
}

/* LFSR with initialization mode LFSR初始化模式 */
#define MulByPow2(x, k) ((((x) << k) | ((x) >> (31 - k))) & 0x7FFFFFFF)
void LFSRWithInitialisationMode(u32 u)
{
	u32 f, v;
	
	f = LFSR_S0;
	v = MulByPow2(LFSR_S0, 8);
	f = AddM(f, v);
	
	v = MulByPow2(LFSR_S4, 20);
	f = AddM(f, v);
	
	v = MulByPow2(LFSR_S10, 21);
	f = AddM(f, v);
	
	v = MulByPow2(LFSR_S13, 17);
	f = AddM(f, v);
	
	v = MulByPow2(LFSR_S15, 15);
	f = AddM(f, v);
	
	f = AddM(f, u);

	/* update the state 更新寄存器状态 */
	LFSR_S0 = LFSR_S1;
	LFSR_S1 = LFSR_S2;
	LFSR_S2 = LFSR_S3;
	LFSR_S3 = LFSR_S4;
	LFSR_S4 = LFSR_S5;
	LFSR_S5 = LFSR_S6;
	LFSR_S6 = LFSR_S7;
	LFSR_S7 = LFSR_S8;
	LFSR_S8 = LFSR_S9;
	LFSR_S9 = LFSR_S10;
	LFSR_S10 = LFSR_S11;
	LFSR_S11 = LFSR_S12;
	LFSR_S12 = LFSR_S13;
	LFSR_S13 = LFSR_S14;
	LFSR_S14 = LFSR_S15;
	LFSR_S15 = f;
}

/* LFSR with work mode LFSR LFSR的工作模式*/
void LFSRWithWorkMode()
{
	u32 f, v;
	
	f = LFSR_S0;
	v = MulByPow2(LFSR_S0, 8);
	f = AddM(f, v);
	
	v = MulByPow2(LFSR_S4, 20);
	f = AddM(f, v);
	
	v = MulByPow2(LFSR_S10, 21);
	f = AddM(f, v);
	
	v = MulByPow2(LFSR_S13, 17);
	f = AddM(f, v);
	
	v = MulByPow2(LFSR_S15, 15);
	f = AddM(f, v);
	
	/* update the state *///更新寄存器状态
	LFSR_S0 = LFSR_S1;
	LFSR_S1 = LFSR_S2;
	LFSR_S2 = LFSR_S3;
	LFSR_S3 = LFSR_S4;
	LFSR_S4 = LFSR_S5;
	LFSR_S5 = LFSR_S6;
	LFSR_S6 = LFSR_S7;
	LFSR_S7 = LFSR_S8;
	LFSR_S8 = LFSR_S9;
	LFSR_S9 = LFSR_S10;
	LFSR_S10 = LFSR_S11;
	LFSR_S11 = LFSR_S12;
	LFSR_S12 = LFSR_S13;
	LFSR_S13 = LFSR_S14;
	LFSR_S14 = LFSR_S15;
	LFSR_S15 = f;
}

/* BitReorganization */
void BitReorganization()//*********************************比特重组，产生X0，X1,X2.X3
{
	BRC_X0 = ((LFSR_S15 & 0x7FFF8000) << 1) | (LFSR_S14 & 0xFFFF);
	BRC_X1 = ((LFSR_S11 & 0xFFFF) << 16) | (LFSR_S9 >> 15);
	BRC_X2 = ((LFSR_S7 & 0xFFFF) << 16) | (LFSR_S5 >> 15);
	BRC_X3 = ((LFSR_S2 & 0xFFFF) << 16) | (LFSR_S0 >> 15);
}

#define ROT(a, k) (((a) << k) | ((a) >> (32 - k)))

/* L1 */
u32 L1(u32 X)//******************************************线性变换L1
{
	return (X ^ ROT(X, 2) ^ ROT(X, 10) ^ ROT(X, 18) ^ ROT(X, 24));
}

/* L2 */
u32 L2(u32 X)//******************************************线性变换L2
{
	return (X ^ ROT(X, 8) ^ ROT(X, 14) ^ ROT(X, 22) ^ ROT(X, 30));
}

#define MAKEU32(a, b, c, d) (((u32)(a) << 24) | ((u32)(b) << 16)| ((u32)(c) << 8) | ((u32)(d)))//将取出的4个8位数值连接在一起
/* F */
u32 F()//************************************非线性函数F
{
	u32 W, W1, W2, u, v;
	W = (BRC_X0 ^ F_R1) + F_R2;
	W1 = F_R1 + BRC_X1;
	W2 = F_R2 ^ BRC_X2;
	u = L1((W1 << 16) | (W2 >> 16));//将32位的W1和W2的高、低16位重组
	v = L2((W2 << 16) | (W1 >> 16));
	F_R1 = MAKEU32(S0[u >> 24], S1[(u >> 16) & 0xFF],S0[(u >> 8) & 0xFF], S1[u & 0xFF]);//作用是分别取出每两位。例：8a4f07bd ，分别取出8a、4f、07、bd
	F_R2 = MAKEU32(S0[v >> 24], S1[(v >> 16) & 0xFF],S0[(v >> 8) & 0xFF], S1[v & 0xFF]);
	return W;
}

#define MAKEU31(a, b, c) (((u32)(a) << 23) | ((u32)(b) << 8) | (u32)(c))//si=ki||di||ivi。位数31=8+15+8

/* initialize */
void Initialization(u8* k, u8* iv)//将K和iv加载到LFSR中
{
	u32 w, nCount;
	/*int i;
	for (i=0; i<16; i++)
	{
		printf("%d ",k[i]);
	}
	printf("\n");
	
	for (i=0; i<16; i++)
	{
		printf("%d ",iv[i]);
	}
	printf("\n");
	*/

	/* expand key */
	LFSR_S0 = MAKEU31(k[0], EK_d[0], iv[0]);
	LFSR_S1 = MAKEU31(k[1], EK_d[1], iv[1]);
	LFSR_S2 = MAKEU31(k[2], EK_d[2], iv[2]);
	LFSR_S3 = MAKEU31(k[3], EK_d[3], iv[3]);
	LFSR_S4 = MAKEU31(k[4], EK_d[4], iv[4]);
	LFSR_S5 = MAKEU31(k[5], EK_d[5], iv[5]);
	LFSR_S6 = MAKEU31(k[6], EK_d[6], iv[6]);
	LFSR_S7 = MAKEU31(k[7], EK_d[7], iv[7]);
	LFSR_S8 = MAKEU31(k[8], EK_d[8], iv[8]);
	LFSR_S9 = MAKEU31(k[9], EK_d[9], iv[9]);
	LFSR_S10 = MAKEU31(k[10], EK_d[10], iv[10]);
	LFSR_S11 = MAKEU31(k[11], EK_d[11], iv[11]);
	LFSR_S12 = MAKEU31(k[12], EK_d[12], iv[12]);
	LFSR_S13 = MAKEU31(k[13], EK_d[13], iv[13]);
	LFSR_S14 = MAKEU31(k[14], EK_d[14], iv[14]);
	LFSR_S15 = MAKEU31(k[15], EK_d[15], iv[15]);

	/* set F_R1 and F_R2 to zero */
	F_R1 = 0;
	F_R2 = 0;
	nCount = 32;
	while (nCount > 0)
	{
	BitReorganization();
	w = F();
	LFSRWithInitialisationMode(w >> 1);
	nCount --;
	}
}




void bin_out(u32 num)//以二进制显示方式输出,自己加的函数
{
char bin[64];  
itoa(num,bin,2); 
printf("%032s\n",bin); 
}

void bin_out_8(unsigned char a)
{
	unsigned char t;
	int i;
	for(i=8; i>=1; i--)
	{
		t=(a>>(i-1))&0x01;

		if (t)
			printf("1");
		else
			printf("0");
	}
	
}



void GenerateKeystream(u32* pKeystream, int KeystreamLen)//****************生成密钥流
{
	int i;
	int cnt1=0;
	u32 f_out;
	
	FILE *plaintext, *ciphertext;
	//u32 temp;
	u32 cipher;
	//plaintext=fopen("plaintext.txt","w");
	//ciphertext=fopen("ciphertext.txt","w");
	plaintext=fopen("plaintext.txt","wb");
	ciphertext=fopen("ciphertext.txt","wb");


	{

	BitReorganization();
	F(); 			/* discard the output of F */  
	LFSRWithWorkMode();
	}
	
	for (i = 0; i < KeystreamLen; i ++)
	{
	BitReorganization();

	printf("BRC_X3:     %08x   ",BRC_X3);//打印出X3的值
	bin_out(BRC_X3);//二进制方式显示X3的值

    f_out=F();
	printf("f_out:      %08x   ", f_out);//打印出F函数的输出
	bin_out(f_out);

	pKeystream[i] = f_out ^ BRC_X3;
	printf("pKeystream: %08x   ",pKeystream[i]);//*******************这是我自己添加的，目的是打印出输出的密钥流
    bin_out(pKeystream[i]);
	//printf("\n");
	printf("plain:      %08x   ",buffer[cnt1]);
	
	fprintf(plaintext,"%x",buffer[cnt1]);

	bin_out(buffer[cnt1]);
	cipher=pKeystream[i]^buffer[cnt1++];
	/*
	temp=0;
	printf("plain:      ");
	for (j=4; j>=1; j--)
	{
	printf("%2x",buffer[cnt1]);
	temp^=((pKeystream[i]>>(8*(j-1)))^(u32)buffer[cnt1++])&0x00ff;
	if(1!=j)
		temp=temp<<8;
	}
	printf("   ");

	for (j=4; j>=1; j--)
	{
		bin_out_8(buffer[cnt1-j]);
	}
	printf("\n");

	pKeystream[i]=temp;
	*/
	printf("cipher:     %08x   ",cipher);
	fprintf(ciphertext,"%x ",cipher);

	bin_out(cipher);
	printf("\n");
	printf("\n");

	LFSRWithWorkMode();
	}
	fclose(plaintext);
	fclose(ciphertext);
}




void decrypt(u32* pKeystream, int KeystreamLen, int last_read,int option)//****************生成密钥流
{
	int i,j;
	int cnt1=0;
	u32 f_out;
	
	FILE *plaintext, *ciphertext;
	//u32 temp;
	u32 cipher;//此时为解密后的明文
	unsigned char tt;
	//plaintext=fopen("plaintext.txt","w");
	//ciphertext=fopen("ciphertext.txt","w");
	plaintext=fopen("xx_plaintext.txt","wb");
	//ciphertext=fopen("ciphertext.txt","rb");


	{

	BitReorganization();
	F(); 			/* discard the output of F */  
	LFSRWithWorkMode();
	}
	if(option==1){
		for (i = 0; i < KeystreamLen; i ++)
			{
			BitReorganization();

			printf("BRC_X3:     %08x   ",BRC_X3);//打印出X3的值
			bin_out(BRC_X3);//二进制方式显示X3的值

			f_out=F();
			printf("f_out:      %08x   ", f_out);//打印出F函数的输出
			bin_out(f_out);

			pKeystream[i] = f_out ^ BRC_X3;
			printf("pKeystream: %08x   ",pKeystream[i]);//*******************这是我自己添加的，目的是打印出输出的密钥流
			bin_out(pKeystream[i]);
			//printf("\n");
			//printf("plain:      %08x   ",buffer[cnt1]);
			printf("cipher:     %08x   ",buffer[cnt1]);
			
			//fprintf(plaintext,"%x",buffer[cnt1]);


			bin_out(buffer[cnt1]);
			cipher=pKeystream[i]^buffer[cnt1++];
			//printf("cipher:     %08x   ",cipher);
			printf("plain:      %08x   ",cipher);
			//fprintf(ciphertext,"%x",cipher);
			//fprintf(plaintext,"%x",cipher);
			for(j=3; j>=0; j--)
			{
				tt=(unsigned char)((cipher>>(i*8))&0x0ff);
				fputc(tt,plaintext);

			}

			bin_out(cipher);
			printf("\n");
			printf("\n");

			LFSRWithWorkMode();
	}
	}
	else if(option==2){
		for (i = 0; i < KeystreamLen; i ++)
			{
			BitReorganization();

			printf("BRC_X3:     %08x   ",BRC_X3);//打印出X3的值
			bin_out(BRC_X3);//二进制方式显示X3的值

			f_out=F();
			printf("f_out:      %08x   ", f_out);//打印出F函数的输出
			bin_out(f_out);

			pKeystream[i] = f_out ^ BRC_X3;
			printf("pKeystream: %08x   ",pKeystream[i]);//*******************这是我自己添加的，目的是打印出输出的密钥流
			bin_out(pKeystream[i]);
			//printf("\n");
			printf("plain:      %08x   ",buffer[cnt1]);
			
			fprintf(plaintext,"%x",buffer[cnt1]);

			bin_out(buffer[cnt1]);
			cipher=pKeystream[i]^buffer[cnt1++];
			printf("cipher:     %08x   ",cipher);
			fprintf(ciphertext,"%x ",cipher);

			bin_out(cipher);
			printf("\n");
			printf("\n");

			LFSRWithWorkMode();
			}


	}
	else
		printf("option 不存在！！！\n");
	fclose(plaintext);
	//fclose(ciphertext);
}




/*****************************主函数****************************************/

void main()
{    
	//密钥K和初始矢量iv
	//u8 k[16]={15,5,56,1,8,25,6,9,8,36,41,22,71,12,24,10};//因为是8位的，所以每个数字都是在0~255之间
	//u8 iv[16]={8,4,16,1,6,45,7,9,3,36,42,1,32,17,5,23};

    //u8 k[16]={87,4,95,13,161,32,199,61,20,147,56,84,126,205,165,148};//因为是8位的，所以每个数字都是在0~255之间
	//u8 iv[16]={116,166,112,38,192,214,34,211,170,25,18,71,4,135,68,5};

	u8 k[16];
	u8 iv[16];
	int t, cnt=0,last_read=0;
    clock_t t1,t2;
	int logg;
	//unsigned char buffer[MAX];

	
	FILE *in_key, *in_iv, *plain, *cipher;
	int i;
	//int KeystreamLen=5;//这里可以修改输出密钥字的个数
	int KeystreamLen;
    //u32 Keystream[1024*128];
    //t1=clock(); 
	unsigned char tt[9];
	//unsigned char b1,b2,b3,b4;
	unsigned char bbbb[4];

	int option;

	printf("输入为汉字请输入1，输入为十六进制字符请输入2!\n");
	scanf("%d",&option);

	in_key=fopen("key.txt","r");
	for(i=0; i<16; i++)
	{
		fscanf(in_key,"%u",&t);
		k[i]=t;
	}
	fclose(in_key);
		
	in_iv=fopen("iv.txt","r");
	for(i=0; i<16; i++)
	{
		fscanf(in_iv,"%u",&t);
		iv[i]=t;
	}

	fclose(in_iv);
	
	//plain=fopen("plain.txt","r");
	plain=fopen("plain.txt","rb");
	cnt=0;
	
	//依据option选择处理方式

	switch(option)
	{

	case 1:
		{
			while (fscanf(plain,"%c",&bbbb[0])!=EOF)
			{
				last_read=1;

				if (fscanf(plain,"%c",&bbbb[1])==EOF)
				{
					bbbb[1]=0;
					last_read=2;

				}
				if (fscanf(plain,"%c",&bbbb[2])==EOF)
				{
					bbbb[2]=0;
					last_read=3;

				}
				if (fscanf(plain,"%c",&bbbb[3])==EOF)
				{
					bbbb[3]=0;
					last_read=4;
				}
				
				buffer[cnt]=0;
				for (i=0; i<4; i++)
				{
					buffer[cnt]=buffer[cnt]|bbbb[i];
					if(i!=3)
						buffer[cnt]=buffer[cnt]<<8;
				}
				cnt++;


			}
		}
		break;
	case 2:
		{
		i=0;
		memset(tt,'0',9);
		while(EOF!=(t=fgetc(plain)))
		{
			//buffer[cnt]=t;
			tt[i++]=t;
			//cnt++;
			if (8==i)
			{
				tt[i]='\0';
				if(tt[0]>='a' && tt[0]<='f')
				{
					tt[0]='0'+(tt[0]-'a'+10)-8;
					buffer[cnt++]=(u32)strtol(tt,NULL,16)+(u32)(1<<31);
				}
				else if (tt[0]-'0'>=8)
				{
					tt[0]=tt[0]-8;
					buffer[cnt++]=(u32)strtol(tt,NULL,16)+(u32)(1<<31);

				}
				else
					buffer[cnt++]=(u32)strtol(tt,NULL,16);
				memset(tt,'0',9);
				i=0;
			}
		}
			fclose(plain);

		if (0!=i)
		{
			tt[8]='\0';
			//buffer[cnt++]=(u32)strtol(tt,NULL,16);
			
			if(tt[0]>='a' && tt[0]<='f')
			{
				tt[0]='0'+(tt[0]-'a'+10)-8;
				buffer[cnt++]=(u32)strtol(tt,NULL,16)+(u32)(1<<31);
			}
			else if (tt[0]-'0'>=8)
			{
				tt[0]=tt[0]-8;
				buffer[cnt++]=(u32)strtol(tt,NULL,16)+(u32)(1<<31);
				
			}
			else
					buffer[cnt++]=(u32)strtol(tt,NULL,16);
				
		}
		}
		break;
	default:
		{
			printf("option doesn't exsit!!!\n");
			exit(0);
		}
		break;
		
	}

	/*
	len=4-cnt%4;
	for (i=1; i<=len; i++)
	{
		buffer[cnt]=0;
		cnt++;
	}
	KeystreamLen=cnt/4;
	*/

	KeystreamLen=cnt;

	logg=cnt;
	
	printf("开始加密！！！\n");
	t1=clock();
	Initialization(k, iv);//初始化阶段
	//LFSRWithWorkMode();//工作阶段
	GenerateKeystream(Keystream,KeystreamLen);//输出密钥流
	t2=clock();

	printf("\n加密速度为%2.4fbit/毫秒\n",32*((float)cnt/(t2-t1)));


	printf("开始解密！！！\n");
	cipher=fopen("ciphertext.txt","rb");

	for (i=0; i<KeystreamLen; i++)
	{
		fscanf(cipher,"%x",&buffer[i]);
	}
	fclose(cipher);


	t1=clock();
	Initialization(k, iv);//初始化阶段
	////LFSRWithWorkMode();//工作阶段
	decrypt(Keystream,KeystreamLen,last_read,option);//输出密钥流
	t2=clock();
	
	printf("\n解密速度为%2.4fbit/毫秒\n",32*((float)cnt/(t2-t1)));
	
}

