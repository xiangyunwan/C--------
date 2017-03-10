#include<stdio.h>
#include"windows.h"
#include<stdlib.h>
#include<time.h>
#include"NTRUlib32.h"

#pragma comment(lib,"ntrulib32.lib")

#define N 107
int main(int argc,char *argv[])
{
	int f[N+1];
	int g[N+1];
	int h[N+1];
	int Fp[N+1];
	int Fq[N+1];
	int r[N+1];
	int e[N+1];
	int d[N+1];
	int i;
	
	//int N=107;
	int q=64;
	//int q=32;
	int p=3;
		int m[N+1]={-1,-1,-1,0,1,1,1,1,1,0,1,0,1,1,1,1,-1,-1,-1,1,-1,1,-1,0,-1,-1,-1,\
		-1,0,0,1,-1,-1,0,0,1,-1,1,0,1,0,0,0,1,0,1,1,0,-1,-1,-1,1,0,-1,1,1,-1,1,1,\
		-1,0,0,-1,-1,0,1,0,1,-1,0,-1,0,0,-1,0,1,1,-1,-1,-1,0,0,0,1,-1,1,-1,1,0,0,\
		1,1,0,1,0,0,1,1,1,0,0,-1,0,0,0,-1,0};
	Poly_init(f,N+1);
	Poly_init(g,N+1);
	Poly_init(r,N+1);
	Poly_init(h,N+1);
	Poly_init(Fp,N+1);
	Poly_init(Fq,N+1);
	Poly_init(e,N+1);
	Poly_init(d,N+1);

	RandPoly(f,N,15,14);
	RandPoly(g,N,12,12);
	RandPoly(r,N,5,5);

	CreateKey(N,q,p,f,g,h,Fp,Fq);
	
	printf("公钥：\n");
	for(i=0;i<N;i++)
		printf("%d,",h[i]);
		printf("\n\n");
	printf("明文：\n");
	for(i=0;i<N;i++)
		printf("%d,",m[i]);
		printf("\n\n");
	Encode(N,q,r,m,h,e);
	printf("密文：\n");
	for(i=0;i<N;i++)
		printf("%d,",e[i]);
		printf("\n\n");
	Decode(N,q,p,f,Fp,e,d);
	printf("解密结果：\n");
		for(i=0;i<N;i++)
		printf("%d,",d[i]);
		printf("\n");
		system("pause");
}