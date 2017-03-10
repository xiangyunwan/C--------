#ifndef LIB_H
#define LIB_H

extern  void __declspec(dllexport)RandPoly(int r[],int n, int NumOnes, int NumNegOnes);
extern  void __declspec(dllexport)Poly_init(int a[],int n);
extern  void __declspec(dllexport)CreateKey(int n,int q,int p,int f[],int g[],int h[],int Fp[],int Fq[]);
extern  void __declspec(dllexport)Encode(int n,int q,int r[],int m[],int h[],int e[]);
extern  void __declspec(dllexport)Decode(int n,int q,int p,int f[],int Fp[],int e[],int d[]);

#endif
