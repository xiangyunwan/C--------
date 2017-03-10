#ifndef _BMTK_H
#define _BMTK_H

#include "stdafx.h"
#include<iostream.h>
#include "string.h"
#include "Udp.h"

class BMTK 
{
public:	
     char TGK,TEK,SALT;		
	 char Saltlenth,L,n_b;
	 char* Salt_data;
	 char* Updata_time;
     char* MTK;
	 char* MSK;
	 char* SSRC;
	 char* ROC;
     char* TS;
     
     MDPC_MTKD_BMTK(char* data,int lenth);
	 	 	
protected:
	
};

#endif



















