#include "stdafx.h"
#include "Rtp.h"
/*
extern char* Key_data;
extern char* Salt_data;
extern char PT,TS,ROC;
extern char M,SSRC,L;
extern char SEQ,IV,IV';
extern char* k_s;
extern char* k_e,MTK;

extern char Saltlenth;
extern char key[25];
extern char* SRTPpacket,RTPpacket;
*/
BMTK mtk;
Aes *MyAes;

struct SRTP
{
    char V_CC;
	unsigned char M_TP;
	unsigned int SeqNum;
    long int Timestamp;
	char* SSRC;
	long int RTPExt;
	char* Payload;
	long int SRTP_MKI;
	long int Auth_tag;
}SRTPNum;

struct RTPPacket	 
{ 
    char *putout;	
	char* payload;
    int length;	 
	
	//结构体有问题
	
}RTPPacketNum;

unsigned char M;
unsigned int SEQ;
unsigned char PT;
char* SRTPpacket;
char*master_key=0;
char* master_salt=0;
char r=0,key_derivation_rate=0;
//char index=0,a=0;

char* keyinput=0;
char* x=0; 
char* k_e;
char* k_s;
int i=0,j=0;


void RTP::MDPC_MTKD_DeRTP(char *buffer,int lenth)
 { 
  SEQ=SRTPNum.SeqNum;
  SRTPpacket=SRTPNum.Payload;
  unsigned char TypeBitmap=0x7f;
  unsigned char KvBitmap=0x80;
  M=SRTPNum.M_TP&TypeBitmap;
  PT=(SRTPNum.M_TP&KvBitmap)>>7;
  master_salt=mtk.Salt_data;
  r=key_derivation_rate;
   char put[12];  
  for(i=0;i<12;i++)
	  put[i]=0;
  //i = 2^16 * ROC + SEQ.
  //a=index/r;
 
  char* x_half=0;  
  int put_lenth=0,x_lenth=0;
  if(k_e=NULL)
  {
    char label[3]="00";
	strcat(label,put);
	put_lenth=strlen(label);
	x_lenth=strlen(master_salt);
	 for(i=0;i<put_lenth;i++)
	  x_half[i]=put[i]^master_salt[i];
	 for(i=put_lenth;i<x_lenth;i++)
      x[i]=master_salt[i];
	  strcat(x,x_half);
      char k[5]="0000";
	 strcat(x,k);
	 //keyinput=(unsigned char*)x;
	strcpy(keyinput,x);
     // keyinput=x*y;
	 MyAes = new Aes(32,(unsigned char*)master_key);
	  //MyAes.Aes();
	 MyAes->Cipher((unsigned char*)keyinput,output); //加密
	 k_e=(char*)output;
  }
  else
printf("No master_key!");

   if(k_s=NULL)
  {
	   char label[3]="02";
	   strcat(label,put);
	   put_lenth=strlen(label);
	   x_lenth=strlen(master_salt);
	   for(i=0;i<put_lenth;i++)
		   x_half[i]=put[i]^master_salt[i];
	   for(i=put_lenth;i<x_lenth;i++)
		   x[i]=master_salt[i];
	   strcat(x,x_half);
	   char* k="0000";
	   strcat(x,k);
       //keyinput=x*y;
	   MyAes = new Aes(32,(unsigned char*)master_key);
	  // MyAes.Aes();
	   MyAes->Cipher((unsigned char*)keyinput,output); 
	   char* p=0;
	   int p_lenth=0;
	   p=(char*)output;
	   p_lenth=strlen(p);
	   for(int i=4;i<p_lenth;i++)
	   k_s[i]=p[i];
  }
  else
	  printf("No master_salt!");
	  

    char* m=0;
    char* k_e_m=0;
	char* IV_temp=0;
	unsigned int half;
	int IV_lenth;
	half=0x00||M||PT||SEQ;
	for(i=0;i<mtk.L;i++)
	{
	  IV_temp[i]=mtk.TS[i]||mtk.ROC[i]||mtk.SSRC[i];
	}
	IV_lenth=strlen(IV_temp);
	IV_temp[IV_lenth-2]=IV_temp[IV_lenth-2]||(half&0xff);
	IV_temp[IV_lenth-1]=IV_temp[IV_lenth-1]||((half>>8)&0xff);
	strcpy(IV,IV_temp);
	IV=IV_temp;
   //IV
	   // mtk.TS||mtk.SSRC||mtk.ROC;
   for(i=0;i<mtk.Saltlenth;i++)   
    m[i]=(k_s[i]||0x55)^k_e[i]; 
   strcpy(k_e_m,m);
   //k_e_m=k_e^m;
   MyAes=new Aes(32,(unsigned char*)k_e_m);
   MyAes->Cipher((unsigned char*)IV,output);  
   strcpy(IW,(char*)output);
   //L=N/n_b;   
    for(j=0;j<mtk.L;j++)
		s(j);
	lenth=mtk.L;
	buffer=SRTPNum.Payload;
}	

char* RtpCaculate(const char* key)
{
	for(i=0;i<j;i++)
	RTPPacketNum.payload[i]=SRTPNum.Payload[i]^key[i];
}

 
char* keyCaculate(int m,const char* key1,const char* key2)
{
   for(i=0;i<j;i++)
  stream[i]=state[i]^IW[i];
  stream[j-2]=stream[j-2]||(j&0xff);
  stream[j-1]=stream[j-1]||((j>>8)&0xff);
}

char* RTP::s(int i)
{
	char res;
	if (i==-1){return 0;}
	stream = keyCaculate(j,IW,s(j-1));
    payload_data=RtpCaculate(s(j-1));
	//stream=j^IW^s(j-1);
	//MyAes.Aes(24,k_e);
	MyAes=new Aes(32,(unsigned char*)k_e);
	MyAes->Cipher((unsigned char*)stream,output);
	strcpy(state,(char*)output);
	return(output);
}















 




















 