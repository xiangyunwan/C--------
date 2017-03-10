//���ߣ�GengKui
//����RSA�����㷨
namespace RSA
{
	void Init(); //����������
	bool IsPrimer(__int64 n); //�ж�����
	void Encrypt(char *filename,char *output,char *pub_key_file); //����
	void DeEncrypt(char *filename,char *output,char *pri_key_file); //����
	void GetPrimers(__int64 &p,__int64 &q); //��������
	void Create_Key(char *pub_key_file,char *pri_key_file);  //���ɹ�Կ��˽Կ
	__int64 Gcd(__int64 a,__int64 b); //�����Լ��,ŷ������㷨
	void Ex_Gcd(__int64 a,__int64 b,__int64 &x,__int64 &y); //��չŷ������㷨
	__int64 Mod(unsigned __int64 a,unsigned __int64 b,unsigned __int64 n);  //a^b%c
	int Encrypt_Int16(char *pub_key_file,unsigned __int64 in);
	int DeEncrypt_Int16(char *pri_key_file,unsigned __int64 in);
};
