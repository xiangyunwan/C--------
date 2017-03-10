#ifndef _RTP_H 
#define _RTP_H

#include "stdafx.h"
#include<iostream.h>
#include "string.h"
#include "ctype.h"
#include "Aes.h"
#include "Udp.h"
#include "BMTK.h"

class RTP
{
public:
    char* IV;
	char* IW;
	char* stream;
	char* payload_data;
	char* temp;
    void MDPC_MTKD_DeRTP(char* buffer,int lenth);
	char* s(int i);
	
protected:
	
	
	
};


#endif




















