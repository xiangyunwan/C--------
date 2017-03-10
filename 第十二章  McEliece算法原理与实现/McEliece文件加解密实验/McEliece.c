//附录A  McEliece公钥密码体制实现代码
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>
#include <string.h>

//类型代替
//#include "mceliece.h"
typedef unsigned char		u_int8_t;
typedef unsigned int		u_int32_t;

typedef int bool;
#define true 1
#define false 0

#ifndef MCELIECE_TEST
#define MCELIECE_TEST 1
#endif

#if	MCELIECE_TEST
#define	xmalloc(size) malloc(size)
#define	xcalloc(num,size) calloc(num,size)
#define	xfree(ptr) if (NULL != ptr) free(ptr)
#define	LOGFILE	logfile

void hexdump(u_int8_t *data, size_t size);
int die(int sig, const char *fmt, ...);
int info (const char *fmt, ...);
#endif

//定义宏参数
#define	PARAM_N	10
#define	PARAM_T	37	/* try 41, 43, 47, 53, 59, 61, 67 ... */
#define	POLY	0xed       //PLOY :  11101101
#define	PARAM_O	(1 << PARAM_N)       //PARAM_O=1024	
#define	PARAM_O8	(PARAM_O / 8)        //PARAM_O8=1024/8
#define	PARAM_K	(PARAM_O - PARAM_N * PARAM_T)    //PARAM_K=1024-370=654
#define	PARAM_L	((PARAM_K + 7) / 8)  //PARAM_L=661/8

#define	LOG(level,msg)
#define	LOGS(level,section,msg)
#define	FUN(name)

typedef struct goppa_s 
{  //定义了一个结构体
	u_int32_t tv[PARAM_O];      //u_int32_t  就是   unsigned int
	u_int32_t tu[PARAM_O];
	int sy[PARAM_O];
	int sx[PARAM_O];
	int sc[PARAM_O];
	int si[PARAM_O];
	int ss[PARAM_O];
}	goppa_t;

typedef struct mceliece_s 
{    //定义结构体
	/* generator */
	u_int32_t g[PARAM_T + 1];   //t=37
	u_int32_t sq[PARAM_T * PARAM_T];

	/* permutator */
	int pe[PARAM_O];

	/* scrambler */
	char st[PARAM_K * PARAM_L];
	char su[PARAM_K * PARAM_L];
	char sv[PARAM_K * PARAM_L];

	/* public key */
	u_int32_t x[PARAM_O * PARAM_T];
	char y[PARAM_O * PARAM_O8];
	int z[PARAM_O];
	u_int32_t w[PARAM_O];
}	mceliece_t;

//全局变量
goppa_t *g_gop = NULL;   //结构体指针值初始化为空

/* sf(a,b) = (sc[a^b] ^ sx[sc[a^sx[b]]] ^ sy[sc[a^sy[b]]]) */

//算法函数。
static int sf(int a, int b)
{
	register int *sc = g_gop->sc;    //g_gop结构体里面的三个变量赋值到3个int型变量中
	register int *sx = g_gop->sx;
	register int *sy = g_gop->sy;
#if	0 // MCELIECE_TEST   //#if 0用于注释掉代码
	register int c, d, e, f, g, h, i, j;
	FUN("sf");

	c = sc[a ^ b];
	d = a ^ sx[b];
	e = a ^ sy[b];
	f = sc[d];
	g = sc[e];
	h = sx[f];
	i = sy[g];
	j = c ^ h ^ i;
	LOG(L_DEBUG,("sf(%04x,%04x) -> %04x\n", a, b, j));
	LOG(L_DEBUGX,("\tsc[a ^ b] -> c: %04x\n", c));
	LOG(L_DEBUGX,("\ta ^ sx[b] -> d: %04x\n", d));
	LOG(L_DEBUGX,("\ta ^ sy[b] -> e: %04x\n", e));
	LOG(L_DEBUGX,("\tsc[%04x]  -> f: %04x\n", d, f));
	LOG(L_DEBUGX,("\tsc[%04x]  -> g: %04x\n", e, g));
	LOG(L_DEBUGX,("\tsx[%04x]  -> f: %04x\n", f, h));
	LOG(L_DEBUGX,("\tsy[%04x]  -> g: %04x\n", g, i));
	LOG(L_DEBUGX,("\tc ^ h ^ i -> j: %04x\n", j));
	return j;
#else
	return sc[a ^ b] ^ sx[sc[a ^ sx[b]]] ^ sy[sc[a ^ sy[b]]];
#endif
}

//mceliece初始化用子函数
static int sm(int a, int b)
{
	register int c, d, e;

	c = (a * (b & 0x001)) ^
		(a * (b & 0x002)) ^
		(a * (b & 0x004)) ^
		(a * (b & 0x008)) ^
		(a * (b & 0x010)) ^
		(a * (b & 0x020)) ^
		(a * (b & 0x040)) ^
		(a * (b & 0x080)) ^
		(a * (b & 0x100)) ^
		(a * (b & 0x200));
	d = (c >> 10) ^ (((c >> 10) % PARAM_O) << 3) ^ (c % PARAM_O);
	e = (d >> 10) ^ (((d >> 10) % PARAM_O) << 3) ^ (d % PARAM_O);
	return e;	
}

//解密用子函数
static int natsex(u_int32_t *h, int a)
{
	register int i, b, c;

	b = 0;
	c = g_gop->ss[a];
	for (i = PARAM_T; i >= 0; --i)
		b = h[i] ^ sf(b, c);
	return b;
}

//mce_generate初始化用子函数及解密用
static int recsex(mceliece_t *mce, u_int32_t *h, int a)
{
	u_int32_t *g = mce->g;
	register int i, b, c;

	b = g[PARAM_T];
	c = g_gop->ss[a];
	for (i = PARAM_T - 1; i >= 0; --i) 
	{
		h[i] = b;
		b = g[i] ^ sf(b, c);
	}
	return b;
}

//mce_generate初始化用子函数及解密用
static void snmsex(mceliece_t *mce, u_int32_t *h,
				   const u_int32_t *f, const u_int32_t *q)
{
	int *ss = g_gop->ss;
	u_int32_t *g = mce->g;
	u_int32_t t[PARAM_T + PARAM_T + PARAM_T];
	register int i, j;

	for (i = 0; i < PARAM_T + PARAM_T + PARAM_T; ++i)
		t[i] = 0;

	for (i = 0; i < PARAM_T; ++i)
		for (j = 0; j < PARAM_T; ++j)
			t[i + j] ^= sf(f[j], ss[q[i]]);

	for (j = PARAM_T + PARAM_T; j >= PARAM_T; --j)
		if (t[j])
			for (i = 1; i <= PARAM_T; ++i)
				t[j - i] ^= sf(g[PARAM_T - i], ss[t[j]]);

	for (i = 0; i < PARAM_T; ++i)
		h[i] = t[i];
}

//mce_generate初始化用子函数
static void sqshsex(mceliece_t *mce, u_int32_t *h,
					const u_int32_t *f)
{
	u_int32_t t[PARAM_T];
	register int i, j;

	for (i = 0; i < PARAM_T; ++i)
		t[i] = f[i];

	for (j = 0; j < PARAM_O - 1 - PARAM_K; ++j) 
	{
		snmsex(mce, h, t, t);
		for (i = 0; i < PARAM_T; ++i)
			t[i] = h[i];
	}
}

//解密用子函数
static void sqrtsex(mceliece_t *mce, u_int32_t *h,
					const u_int32_t *f)
{
	int *ss = g_gop->ss;
	u_int32_t *sq = mce->sq;
	register int i, j, d;

	for (i = 0; i < PARAM_T; ++i)
		h[i] = 0;

	for (j = 0; j < PARAM_T; ++j) 
	{
		d = ss[ss[f[j]]];
		for (i = 0; i < PARAM_T; ++i)
			h[i] ^= sf(sq[j * PARAM_T + i], d);
	}
}

//解密用子函数
static void funsex(mceliece_t *mce, u_int32_t *h, u_int32_t *f, int m)
{
	static u_int32_t v[2 * PARAM_T + 1];
	static u_int32_t q[2 * PARAM_T];
	static u_int32_t w[2 * PARAM_T];
	register int *ss = g_gop->ss;
	register int *si = g_gop->si;
	register u_int32_t *g = mce->g;
	register int i, j, k, d;
	register u_int32_t x;

	/* zero f, except for the first element which is set to 1 */
	for (i = 0; i < PARAM_T; ++i)
		f[i] = !i;

	/* zero q */
	for (i = 0; i < PARAM_T; ++i)
		q[i] = 0;

	/* copy v from g */
	for (i = 0; i < PARAM_T; ++i)
		v[i] = g[i];

	/* make i the index of the last non zero element of h */
	for (i = PARAM_T - 1; i > 0; --i)
		if (0 != h[i])
			break;

	d = ss[si[h[i]]];

	for (j = PARAM_T; j >= PARAM_T - i; --j)
		v[j] ^= sf(h[j - PARAM_T + i], d);

	q[PARAM_T - i] = sf(1, d);

	for (;;) {
		for (i = PARAM_T - 1; i > 0; --i)
			if (v[i])
				break;

		if (i < m)
			break;
		d = ss[si[v[i]]];

		for (k = PARAM_T - 1 - i; k >= 0; --k) 
		{
			w[k] = sf(h[i + k], d);
			if (0 != w[k]) {
				for (j = PARAM_T - 1; j >= k; --j) 
				{
					h[j] ^= sf(v[j - k], ss[w[k]]);
					f[j] ^= sf(q[j - k], ss[w[k]]);
				}
			}
		}

		/* swap q with f and v with w array (PARAM_T entries) */
		for (i = 0; i < PARAM_T; ++i) 
		{
			x = q[i]; q[i] = f[i]; f[i] = x;
			x = v[i]; v[i] = h[i]; h[i] = x;
		}
	}

	/* copy resulting v and q vectors to h and f */
	for (i = 0; i < PARAM_T; ++i)
		h[i] = v[i];

	for (i = 0; i < PARAM_T; ++i)
		f[i] = q[i];
}

static int rn(int m)    //根据时间种子产生随机数  用于生成随机的矩阵
{
	static bool bSeed = false;
	int RANGE_MIN = 0;
	int RANGE_MAX = (1 << m);
	int rand1024;

	if (!bSeed)
	{	// Seed the random-number generator with current time so that the numbers will be different every time we run.
		srand( (unsigned)time( NULL ) );
		bSeed = true;
	}
	
	rand1024 = (int)(((double) rand() / (double) RAND_MAX) * RANGE_MAX + RANGE_MIN);

	return rand1024;

}

//加密程序
int mce_encode(u_int8_t *key, const u_int8_t *pt, u_int8_t *ct, size_t size)
{
	u_int8_t e[PARAM_O8];
	u_int8_t l[PARAM_O8];
	char m[PARAM_O8];
	char *y = NULL;
	register u_int32_t *tv = g_gop->tv;
	register int d, i, j, k;
	register int accu, mask, bits, inbits, outbits;
	FUN("mce_encode");

	/*
	* if ct is NULL return the required size of a 'ct' buffer
	* to hold 'size' encoded bytes of plain text from 'pt'
	*/
	if (NULL == ct) 
	{
		inbits = size * 8;
		outbits = ((inbits / PARAM_K) + 1) * PARAM_O;
		LOG(L_DEBUG,("encoding 0x%x bytes requires 0x%x bytes buffer\n",
			(unsigned)size, (unsigned)((outbits + 7) / 8)));
		return (outbits + 7) / 8;
	}

	y = (char*)xcalloc(PARAM_O * PARAM_O8, sizeof(char));

	for (i = 0; i < PARAM_K; ++i) 
	{
		for (j = 0; j < PARAM_O8; ++j) 
		{
			y[i * PARAM_O8 + j] = *key;
			key++;
		}
	}

	accu = 0;
	bits = 0;
	inbits = 0;
	outbits = 0;
	while (size > 0) {
		for (outbits = 0, i = 0; i < PARAM_L && outbits < PARAM_K; /* */) 
		{
			k = PARAM_K - outbits > 8 ? 8 : PARAM_K - outbits;
			while (bits < k) 
			{
				if (size > 0) 
				{
					accu |= *pt << bits;
					inbits += 8;
					pt++;
					size--;
				}
				bits += 8;
			}
			mask = (1 << k) - 1;
			l[i++] = accu & mask;
			accu >>= k;
			bits -= k;
			outbits += k;
		}

		for (j = 0; j < PARAM_O8; ++j)
			e[j] = m[j] = 0;

		for (i = 0; i < PARAM_K; ++i) 
		{
			d = !!((l[i / 8] & tv[i]));
			if (d)
				for (j = 0; j < PARAM_O8; ++j)
					m[j] ^= y[i * PARAM_O8 + j];
		}

		/* create a PARAM_T bit error vector */
		for (i = 0; i < PARAM_T; ++i) 
		{
			do {
				j = rn(PARAM_N);
			} while (e[j / 8] & tv[j]);
			e[j / 8] |= tv[j];
		}

		/* copy out the encoded block with errors */
		for (j = 0; j < PARAM_O8; ++j) 
		{
			*ct = m[j] ^ e[j];
			ct++;
		}
	}

	xfree(y);
	return 0;
}

//解密程序。
int mce_decode(void *ctx, const u_int8_t *ct, u_int8_t *pt, size_t size)
{
	mceliece_t *mce = (mceliece_t *)ctx;
	u_int8_t l[PARAM_O8];
	char m[PARAM_O8];
	u_int32_t f[PARAM_T];
	u_int32_t h[PARAM_T];
	u_int32_t q[PARAM_T+1];
	u_int32_t s[PARAM_T];
	int *ss = g_gop->ss;
	int *si = g_gop->si;
	u_int32_t *tv = g_gop->tv;
	int *pe = mce->pe;
	char *su = mce->su;
	u_int32_t *w = mce->w;
	register int a, b, d, i, j, k;
	register int accu, mask, bits, inbits, outbits;
	FUN("mce_decode");

	if (NULL == pt) 
	{
		inbits = size * 8;
		outbits = (inbits / PARAM_O) * PARAM_K;
		LOG(L_DEBUG,("decoding 0x%x bytes requires 0x%x bytes buffer\n",
			(unsigned)size, (unsigned)((outbits + 7) / 8)));
		return (outbits + 7) / 8;
	}

	accu = 0;
	bits = 0;
	inbits = 0;
	outbits = 0;

	while (size > 0) 
	{
		for (j = 0; j < PARAM_O8; ++j)
			l[j] = m[j] = 0;

		for (j = 0; size > 0 && j < PARAM_O8; ++j) 
		{
			l[j] = *ct;
			ct++;
			size--;
		}

		for (a = 0; a < PARAM_O; ++a) 
		{
			if (l[pe[a] / 8] & tv[pe[a]])
				m[a / 8] |= tv[a];
		}

		for (i = 0; i < PARAM_T; ++i)
			s[i] = 0;

		for (a = 0; a < PARAM_O; ++a) 
		{
			if (m[a / 8] & tv[a]) 
			{
				b = ss[si[recsex(mce, h, a)]];
				for (i = 0; i < PARAM_T; ++i)
					s[i] ^= sf(h[i], b);
			}
		}

		for (i = 0; i < PARAM_T; ++i) 
		{
			if (s[i]) 
			{
				funsex(mce, s, h, 1);

				d = ss[si[s[0]]];

				for (i = 0; i < PARAM_T; ++i)
					h[i] = sf(h[i], d);
				h[1] ^= 1;

				for (i = 0; i < PARAM_T; ++i)
					if (0 != h[i])
						break;

				if (PARAM_T == i) {
					m[0] ^= tv[0];
#if	MCELIECE_TEST

//   #ifndef x   先测试x是否被宏定义过   
//   #define x 程序段1 
//   如果x没有被宏定义过，定义x，并编译程序段1 
//   #endif 
//   程序段2 如果x已经定义过了则编译程序段2的语句，"忽视"程序段1。
//   条件指示符#ifndef 的最主要目的是防止头文件的重复包含和编译

					//printf("bit error %04x, ", 0);

#endif
					LOG(L_DEBUGX,("bit error %04x\n", 0));
					break;
				}

				sqrtsex(mce, f, h);
				funsex(mce, f, h, (PARAM_T / 2) + 1);

				for (i = (PARAM_T + 1) / 2; i < PARAM_T; ++i)
					if (0 != h[i])
						break;

				if (i < PARAM_T) 
				{
#if	MCELIECE_TEST
					printf("too many errors in block\n");
#endif
					LOG(L_ERROR,("too many errors in block\n"));
					errno = EINVAL;
					return -1;
				}

				q[PARAM_T] = 0;
				snmsex(mce, q, f, f);
				snmsex(mce, f, h, h);

				for (i = 1; i <= PARAM_T; ++i)
					q[i] ^= f[i - 1];

				for (a = 0; a < PARAM_O; ++a) 
				{
					if (0 == natsex(q, a)) 
					{
						m[a / 8] ^= tv[a];
#if	MCELIECE_TEST
						//printf("bit error %04x, ", a);
#endif
						LOG(L_DEBUGX, ("bit error %04x\n", a));
					}
				}
				break;
			}
		}

		for (i = 0; i < PARAM_L; ++i)
			l[i] = 0;

		for (i = 0; i < PARAM_K; ++i) 
		{
			if (m[w[i] / 8] & tv[w[i]]) 
			{
				for (b = 0; b < PARAM_L; ++b)
					l[b] ^= su[i * PARAM_L + b];
			}
		}

		for (inbits = 0, i = 0; inbits < PARAM_K; /* */) 
		{
			k = PARAM_K - inbits > 8 ? 8 : PARAM_K - inbits;
			mask = (1 << k) - 1;
			accu |= (l[i++] & mask) << bits;
			bits += k;
			inbits += k;
			while (bits >= 8) 
			{
				*pt++ = (u_int8_t)accu;
				accu >>= 8;
				bits -= 8;
				outbits += 8;
			}
		}
	}
#if	MCELIECE_TEST
	printf("outbits: %d\n", outbits);
#endif

	return 0;
}

//生成公钥矩阵
int mce_get_public_key(void *ctx, u_int8_t *key, size_t size)
{
	mceliece_t *mce = (mceliece_t *)ctx;
	char *y = mce->y;
	register int i, j;
	FUN("mce_get_public_key");

	/* if key is NULL return the required size of a buffer */
	if (NULL == key) {
		LOG(L_DEBUG,("public key requires 0x%x bytes buffer\n", PARAM_K * PARAM_O8));
		return PARAM_K * PARAM_O8;
	}

	for (i = 0; i < PARAM_K; ++i) 
	{
		for (j = 0; j < PARAM_O8; ++j) 
		{
			if (size == 0) 
			{
				errno = EINVAL;
				return -1;
			}
			*key = y[i * PARAM_O8 + j];
			key++;
			size--;
		}
	}

	return 0;
}

//加密所用变量初始化，及创建各种矩阵
int mce_generate(void **pctx)
{
	mceliece_t *mce = NULL;
	u_int32_t f[PARAM_T];
	u_int32_t h[PARAM_T];
	char m[PARAM_O8];
	u_int32_t *tu = g_gop->tu;
	u_int32_t *tv = g_gop->tv;
	int *ss = g_gop->ss;
	int *si = g_gop->si;
	u_int32_t *g;
	u_int32_t *sq;
	int *pe;
	char *st;
	char *su;
	char *sv;
	u_int32_t *w;
	u_int32_t *x;
	char *y;
	int *z;
	char *_y = (char*)xcalloc(PARAM_O * PARAM_O8, sizeof(char));
	int pass;
	register int a, b, c, d, i, j;
	FUN("mce_generate");

	mce = (mceliece_t *)xcalloc(1, sizeof(mceliece_t));

	g = mce->g;
	sq = mce->sq;
	pe = mce->pe;
	st = mce->st;
	su = mce->su;
	sv = mce->sv;
	w = mce->w;
	x = mce->x;
	y = mce->y;
	z = mce->z;

	/* generate key */
	g[PARAM_T] = 1;

	LOG(L_MINOR,("creating generator code sequence\n"));
	for (;;) {
		/* get PARAM_T random words of length PARAM_N bits */
		for (i = 0; i < PARAM_T; ++i)
			g[i] = rn(PARAM_N);

		for (a = 0; a < PARAM_O; ++a)
			if (0 == recsex(mce, h, a))
				break;

		if (a < PARAM_O) 
		{
#if	MCELIECE_TEST
			//printf("again: %04x < %04x, ", a, PARAM_O);        //前面的<400
#endif
			LOG(L_DEBUG,("again: %04x < %04x\n", a, PARAM_O));
			continue;
		}

		for (i = 0; i < PARAM_T; ++i)
			f[i] = 0;

		f[1] = 1;
		for (i = 0; i < PARAM_T * PARAM_N; ++i) 
		{
			snmsex(mce, h, f, f);
			for (j = 0; j < PARAM_T; ++j)
				f[j] = h[j];
		}

		for (i = 0; i < PARAM_T; ++i)
			if (f[i] != (i == 1))
				break;

		if (PARAM_T == i)
			break;
	}
#if	MCELIECE_TEST
	printf("\n");
#endif
#if	MCELIECE_TEST
	//printf("created generator code sequence\n");
	printf("创建生成代码序列\n");
#endif
	LOG(L_MINOR,("created generator code sequence\n"));

	LOG(L_MINOR,("creating scrambler matrix\n"));
	x = mce->x;
	for (a = 0; a < PARAM_O; ++a) {
		b = ss[si[recsex(mce, &x[a * PARAM_T], a)]];

		for (i = 0; i < PARAM_T; ++i)
			x[a * PARAM_T + i] = sf(x[a * PARAM_T + i], b);

		for (i = 0; i < PARAM_O8; ++i)
			y[a * PARAM_O8 + i] = 0;

		y[a * PARAM_O8 + a / 8] |= tv[a];
	}
#if	MCELIECE_TEST
	//printf("created scrambler matrix\n");
	printf("创建干扰矩阵\n");
#endif
	LOG(L_MINOR,("created scrambler matrix\n"));

	LOG(L_MINOR,("finding the inverse scrambler matrix\n"));
	for (a = 0; a < PARAM_O; ++a)
		z[a] = -1;

	d = 0;
	for (a = 0; a < PARAM_O; ++a) 
	{
		i = 0;
		for (;;) {
			while (0 == x[a * PARAM_T + i / PARAM_N]) 
			{
				i++;
				if (PARAM_T * PARAM_N == i)
					break;
			}

			if (PARAM_T * PARAM_N == i) 
			{
				w[d++] = a;
				break;
			}

			while (0 == (x[a * PARAM_T + i / PARAM_N] & tu[i]))
				i++;

			if (-1 == (c = z[i])) 
			{
				z[i] = a;
				break;
			}

			for (j = 0; j < PARAM_T; ++j)
				x[a * PARAM_T + j] ^= x[c * PARAM_T + j];

			for (j = 0; j < PARAM_O8; ++j)
				y[a * PARAM_O8 + j] ^= y[c * PARAM_O8 + j];
		}

		if (PARAM_K == d)
			break;
	}
#if	MCELIECE_TEST
	//printf("found the inverse scrambler matrix\n");
	printf("找到干扰矩阵的逆\n");
#endif
	LOG(L_MINOR,("found the inverse scrambler matrix\n"));

	LOG(L_MINOR,("building generator matrix\n"));
	for (i = 0; i < PARAM_T; ++i)
		sq[0 * PARAM_T + i] = !i;

	for (j = 0; j < PARAM_T; ++j)
		h[j] = (j == 1);

	sqshsex(mce, &sq[1 * PARAM_T], h);

	for (i = 2; i < PARAM_T; ++i)
		snmsex(mce, &sq[i * PARAM_T], &sq[(i - 1) * PARAM_T], &sq[1 * PARAM_T]);

#if	MCELIECE_TEST
	//printf("built the generator matrix\n");
	printf("找到生成矩阵\n");
#endif
	LOG(L_MINOR,("built the generator matrix\n"));

	LOG(L_MINOR,("creating permutation sequence\n"));
	for (i = 0; i < PARAM_O; ++i)
		pe[i] = i;

	for (i = 0; i < PARAM_O; ++i) 
	{
		j = i + (rn(PARAM_N) % (PARAM_O - i));
		d = pe[j]; pe[j] = pe[i]; pe[i] = d;
	}
#if	MCELIECE_TEST
	//printf("created the permutation sequence\n");
    printf("创建置换序列\n");
#endif
	LOG(L_MINOR,("created the permutation sequence\n"));

	LOG(L_MINOR,("building permutation matrix\n"));
	for (i = 0; i < PARAM_K; ++i) {

		for (j = 0; j < PARAM_O8; ++j)
			m[j] = 0;

		for (a = 0; a < PARAM_O; ++a)
			if (y[w[i] * PARAM_O8 + a / 8] & tv[a])
				m[pe[a] / 8] |= tv[pe[a]];

		for (j = 0; j < PARAM_O8; ++j)
			y[w[i] * PARAM_O8 + j] = m[j];
	}
#if	MCELIECE_TEST
	//printf("built the permutation matrix\n");
	printf("找到置换矩阵\n");
#endif
	LOG(L_MINOR,("built the permutation matrix\n"));

	LOG(L_MINOR,("building public key matrix\n"));
	pass = 0;
	for (;;) {
		pass++;
		LOG(L_MINOR,("pass #%d\n", pass));
		/* hmm.. let some entropy slip into the PRNG */
		//Sleep(1000);
		for (i = 0; i < PARAM_K; ++i)
			for (j = 0; j < PARAM_L; ++j)
				sv[i * PARAM_L + j] = st[i * PARAM_L + j] = (char)rn(9);

		for (i = 0; i < PARAM_K; ++i)
			for (j = 0; j < PARAM_L; ++j)
				su[i * PARAM_L + j] = 0;

		for (i = 0; i < PARAM_K; ++i)
			su[i * PARAM_L + i / 8] |= tv[i];

		for (i = 0; i < PARAM_K; ++i) 
		{
			for (j = i; j < PARAM_K; ++j) 
			{
				if (st[j * PARAM_L + i / 8] & tv[i]) 
				{
					if (i != j) {
						for (d = 0; d < PARAM_L; ++d) 
						{
							c = su[j * PARAM_L + d];
							su[j * PARAM_L + d] = su[i * PARAM_L + d];
							su[i * PARAM_L + d] = c;

							c = st[j * PARAM_L + d];
							st[j * PARAM_L + d] = st[i * PARAM_L + d];
							st[i * PARAM_L + d] = c;
						}
					}

					for (d = 0; d < PARAM_K; ++d) 
					{
						if (d != i) {
							if (st[d * PARAM_L + i / 8] & tv[i]) 
							{
								for (c = 0; c < PARAM_L; ++c) 
								{
									su[d * PARAM_L + c] ^= su[i * PARAM_L + c];
									st[d * PARAM_L + c] ^= st[i * PARAM_L + c];
								}
							}
						}
					}
					break;
				}
			}
			if (PARAM_K == j)
				break;
		}
		if (PARAM_K == i)
			break;
	}

	for (i = 0; i < PARAM_K; ++i) 
	{
		for (j = 0; j < PARAM_O8; ++j)
			m[j] = 0;
		for (b = 0; b < PARAM_K; ++b) 
		{
			if (sv[i * PARAM_L + b / 8] & tv[b]) 
			{
				for (j = 0; j < PARAM_O8; ++j)
					m[j] ^= y[w[b] * PARAM_O8 + j];
			}
		}
		for (j = 0; j < PARAM_O8; ++j)
			_y[i * PARAM_O8 + j] = m[j];
	}

	for (i = 0; i < PARAM_K; ++i)
		for (j = 0; j < PARAM_O8; ++j)
			y[i * PARAM_O8 + j] = _y[i * PARAM_O8 + j];
#if	MCELIECE_TEST
	//printf("created public key matrix\n");
	printf("创建公钥矩阵\n");
#endif
	LOG(L_MINOR,("created public key matrix\n"));

	*pctx = mce;
	xfree(_y);

	return 0;
}

//结束后释放申请的空间。
int mce_destroy(void *ctx)
{
	mceliece_t *mce = (mceliece_t *)ctx;
	FUN("mce_destroy");

	if (NULL == mce) 
	{
		errno = EINVAL;
		return -1;
	}

	/* PARANOID: overwrite memory used in this context */
	memset(mce, 0x55, sizeof(*mce));
	memset(mce, 0xaa, sizeof(*mce));
	memset(mce, 0xff, sizeof(*mce));
	memset(mce, 0x00, sizeof(*mce));

	xfree(mce);

	return 0;
}

//初始化，申请全局变量
int mceliece(void)
{
	int *sc;
	int *ss;
	int *si;
	int *sx;
	int *sy;
	u_int32_t *tu;
	u_int32_t *tv;
	register int a, b, c, d;
	FUN("mceliece");

	LOG(L_NORMAL,("GF(2) PARAM_N:2**%d GEN:%X AG(%d,%d,%d)\n",
		PARAM_N, POLY, PARAM_O, PARAM_K, PARAM_T));

	info("GF(2) PARAM_N:2**%d GEN:%X AG(%d,%d,%d) ",
		PARAM_N, POLY, PARAM_O, PARAM_K, PARAM_T);

	//申请全局变量
	g_gop = (goppa_t *)xcalloc(1, sizeof(goppa_t));

	LOG(L_MINOR,("init ss\n"));
	ss = g_gop->ss;
	for (a = 0; a < PARAM_O; ++a)
		ss[sm(a, a)] = a;

	LOG(L_MINOR,("zero si\n"));
	si = g_gop->si;
	for (a = 0; a < PARAM_O; ++a)
		si[a] = 0;

	LOG(L_MINOR,("init tu\n"));
	tu = g_gop->tu;
	for (a = 0; a < PARAM_O; ++a)
		tu[a] = 1 << (a % PARAM_N);

	LOG(L_MINOR,("init si\n"));
	for (a = 1; a < PARAM_O; ++a) {
		if (0 == si[a]) {
			for (b = a; b < PARAM_O; ++b)
				if (0 == si[b] && 1 == sm(a, b))
					break;
			if (b >= PARAM_O) {
				LOG(L_ERROR,("polynome returns out of bounds b:%d\n", b));
				return -1;
			}
			c = d = 1;
			do {
				c = sm(a, c);
				d = sm(b, d);
				if (c >= PARAM_O || d >= PARAM_O) 
				{
					LOG(L_ERROR,("polynome returns out of bounds c:%d, d:%d\n",
						c, d));
					return -1;
				}
				si[c] = d;
			} while (c - 1);
		}
	}

	LOG(L_MINOR,("init tv\n"));
	tv = g_gop->tv;
	for (a = 0; a < PARAM_O; ++a)
		tv[a] = 1 << (a % 8);

	LOG(L_MINOR,("init sc\n"));
	sc = g_gop->sc;
	for (a = 0; a < PARAM_O; ++a)
		sc[a] = sm(a, sm(a, a));

	LOG(L_MINOR,("init sx,sy\n"));
	sx = g_gop->sx;
	sy = g_gop->sy;
	for (a = 0; a < PARAM_O; ++a) 
	{
		b = sm(POLY, a);
		sx[a] = b;
		sy[b] = a;
	}

	return 0;
}



#if	MCELIECE_TEST
//按16进制方式输出原文及密文信息。
void hexdump(u_int8_t *data, size_t size)
{
	size_t i, j;
	for (i = 0; i < size; ++i) 
	{
		if (0 == (i % 16)) 
		{
			printf("%04x:", (unsigned)i);
		}
		printf(" %02x", data[i]);
		if (15 == (i % 16)) 
		{
			printf(" - ");
			for (j = i - (i % 16); j <= i; ++j) 
			{
				int c = data[j];
				printf("%c", c < 32 || c > 126 ? '.' : c);
			}
			printf("\n");
		}
	}
	if (0 != (i % 16)) 
	{
		for (j = i % 16; j < 16; ++j) 
		{
			printf("   ");
		}
		printf(" - ");
		for (j = i - (i % 16); j < i; ++j) 
		{
			int c = data[j];
			printf("%c", c < 32 || c > 126 ? '.' : c);
		}
		printf("\n");
	}
}

//出错处理及退出。
int die(int sig, const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	vfprintf(stderr, fmt, ap);
	va_end(ap);
	exit(sig);
}

//输出变量信息
int info (const char *fmt, ...)
{
	va_list ap;
	int len;

	va_start(ap, fmt);
	len = vfprintf(stderr, fmt, ap);
	len += fprintf(stderr, "\n");
	va_end(ap);

	return len;
}
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



int main(int argc, char **argv)
{
	char fin_name[40],fout1_name[40],apt[16];
	char fout2_name[40];
	FILE *fp,*fq,*fr;
	clock_t t_start,t_end;
	int i_len;
	// 这里是要加密的内容，用英语字母； 可以随意换； 也可以扩展程序 从控制台输入；
	char plain[1000000];
	//char plain[] = "Mary had a little lamb.\nThe quick brown fox jumps over the lazy dog.\nKaufen sie jede Woche vier gute bequeme Pelze xy.\nFour score and seven years ago our fathers brought forth on this continent, a new nation, conceived in Liberty, and dedicated to the proposition that all men are created equal. Now we are engaged in a great civil war, testing whether that nation or any nation so conceived and so dedicated, can long endure.\nTo be or not to be that is the question. Whether it is nobler in the mind to suffer the stings and arrows of outrageous fortune, or to take up arms against as sea of troubles, and by opposing them, end them.\nThe word question is derived from the Latin quarrier (to seek) which is the same root as the word for quest. A creative life is a continued quest, and good questions can be very useful guides. Most useful are open-ended questions; they allow for fresh unanticipated answers to reveal themselves.\nQuestions focus our thinking. Ask empowering questions like: What's good about this? What's not perfect about it yet? What am I going to do next time? How can I do this and have fund doing it? (Charles Connolly)\n";
	void *ctx;
	u_int8_t *key;
	u_int8_t *encode;
	u_int8_t *decode;
	u_int8_t b;
	int pk_size;
	int plain_size;
	//int plain_size = strlen(plain) + 1;
	int encode_size;
	int decode_size;
	int ones;
	int i;

	clock_t start,end;
	clock_t start1,end1;

	(void)argc;
	(void)argv;

//初始化函数，申请全局变量空间。
	if (0 != mceliece()) 
	{
		xfree( g_gop );
		die(1, "mceliece() call failed\n");
	}

//加密所用变量初始化，及创建各种矩阵
	if (0 != mce_generate(&ctx)) 
	{
		mce_destroy(ctx);
		xfree( g_gop );
		die(1, "mce_generate() call failed\n");
	}

//生成公钥矩阵
	pk_size = mce_get_public_key(ctx, NULL, 0);
	//printf("public key size is %d bytes\n", pk_size);
	printf("公钥矩阵大小是  %d 字节\n", pk_size);

//申请公钥矩阵空间
	key = (u_int8_t*)xcalloc(pk_size, sizeof(char));
//创建公钥矩阵
	if (0 != mce_get_public_key(ctx, key, pk_size)) 
	{
		mce_destroy(ctx);
		xfree( g_gop );
		die(1, "mce_get_public_key() call failed\n");
	}
//---------------------------------

	//-----------------------------------加密文件开始
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
	fp=fopen(fin_name,"r");
	fq=fopen(fout1_name,"w");
	plain_size=filesize(fp);
	fread(plain,plain_size,1,fp);
	


	

//---------------------------------------


	//printf("plain text:\n");
	printf("加密前明文信息:\n");
	//hexdump((u_int8_t*)plain, plain_size);

//计算加密后所用大小。
	encode_size = mce_encode(key, NULL, NULL, plain_size);
	//printf("encoding %d bytes needs %d bytes buffer\n",plain_size, encode_size);
	printf("加密后密文信息:\n");

//记录加密开始时间
start=clock();
//申请加密后内容所用空间
	encode = (u_int8_t*)xcalloc(encode_size, sizeof(char));
//加密过程
	if (0 != mce_encode(key, (const u_int8_t*)plain, encode, plain_size)) 
	{
		mce_destroy(ctx);
		xfree( g_gop );
		die(1, "mce_encode() call failed\n");
	}
	//hexdump(encode, encode_size);
	//---------------------------------
	fwrite(encode,encode_size,1,fq);
	puts("加密文件已经创建，加密已经完成.\n");
    puts("--------------------------------------------------------");
	fclose(fp);
	fclose(fq);
	//-------------------------------------

//记录加密结束时间
end=clock();

//计算权值Weight
	for (i = 0, ones = 0; i < encode_size; i++) 
	{
		b = encode[i];
		ones +=
			((b >> 0) & 1) +
			((b >> 1) & 1) +
			((b >> 2) & 1) +
			((b >> 3) & 1) +
			((b >> 4) & 1) +
			((b >> 5) & 1) +
			((b >> 6) & 1) +
			((b >> 7) & 1);
	}
	printf("weigth: %d/%d (%f)\n",ones, encode_size * 8, 1.0 * ones / encode_size / 8);

//----------------------------------------------

	puts("请输入解密后的文件的存储路径,(eg:c\\test\\a_decode.txt):");        //解密文件的存储路径
	gets(fout2_name);
	fr=fopen(fout2_name,"wb+");
	if(fr==NULL)
	{
		puts("解密文件正在创建...\n");
		getchar();
	}
	fclose(fr);
	fq=fopen(fout1_name,"r");
	encode_size=filesize(fp);
	fread(encode,encode_size,1,fq);
	fr=fopen(fout2_name,"w");
//----------------------------------------------
	decode_size = mce_decode(ctx, NULL, NULL, encode_size);
	//printf("decoding %d bytes needs %d bytes buffer\n",encode_size, decode_size);

//解密时间统计
start1=clock();
	decode = (u_int8_t*)xcalloc(decode_size, sizeof(char));
//解密过程
	if (0 != mce_decode(ctx, encode, decode, encode_size)) 
	{
		mce_destroy(ctx);
		xfree( g_gop );
		die(1, "mce_decode() call failed\n");
	}
end1=clock();

	printf("解密后明文:\n");
	//-------------------------------
	fwrite(decode,decode_size,1,fr);
	fclose(fp);
	fclose(fr);
	//--------------------------
	//hexdump(decode, decode_size);
    printf("加密时间：%f秒\n",(end-start)/1000.0);
	printf("加密效率：%f字/秒\n",201/((end-start)/1000.0));
	printf("解密时间：%f秒\n",(end1-start1)/1000.0);
	printf("解密效率：%f字/秒\n",201/((end1-start1)/1000.0));

	mce_destroy(ctx);
	xfree( g_gop );
	system("pause");

	return 0;
}
#endif


