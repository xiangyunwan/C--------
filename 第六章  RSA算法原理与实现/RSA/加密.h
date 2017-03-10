//作者：GengKui
//采用RSA加密算法
namespace RSA
{
	void Init(); //建立素数表
	bool IsPrimer(__int64 n); //判断素数
	void Encrypt(char *filename,char *output,char *pub_key_file); //加密
	void DeEncrypt(char *filename,char *output,char *pri_key_file); //解密
	void GetPrimers(__int64 &p,__int64 &q); //生成素数
	void Create_Key(char *pub_key_file,char *pri_key_file);  //生成公钥和私钥
	__int64 Gcd(__int64 a,__int64 b); //求最大公约数,欧几里得算法
	void Ex_Gcd(__int64 a,__int64 b,__int64 &x,__int64 &y); //扩展欧几里得算法
	__int64 Mod(unsigned __int64 a,unsigned __int64 b,unsigned __int64 n);  //a^b%c
	int Encrypt_Int16(char *pub_key_file,unsigned __int64 in);
	int DeEncrypt_Int16(char *pri_key_file,unsigned __int64 in);
};
