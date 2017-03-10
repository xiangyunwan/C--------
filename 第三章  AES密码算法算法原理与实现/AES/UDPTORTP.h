#ifndef _UDPUDPTORTP_H
#define _UDPUDPTORTP_H

#include<iostream.h>
#include "string.h"
#include "StructDefine.h"

class UDPTORTP
{
public:
    int port;
	int j;
	char* IV;
	char* IW;
	char* stream;
	char* payload_data;
//    unsigned char temp;
    unsigned long temp;
	char* state;
	char* k_e;
    char TGK,TEK,SALT;		
	char Saltlenth,L,n_b;
	char* Salt_data;
	char* Updata_time;
	char* MTK;
	char* MSK;
	char* SSRC;
	char* ROC;
	char* TS;	
	unsigned char* output;
	char *put;
	
	s(int i,char* output);
    void RtpCaculate(int m,const char* key,char* output);
    void keyCaculate(int m,const char* key1,const char* key2,char* output);
	void MDPC_MTKD_DeUDP(char *p,int lenth);
	void MDPC_MTKD_BMTK(char* data,int lenth);
    void MDPC_MTKD_DeRTP(char* buffer,int lenth,char* output);
protected:

};

#endif






























































































































