#include "stdafx.h"
#include "BMTK.h"

BMTK::MDPC_MTKD_BMTK(char* data,int lenth)
{	
struct Mikey
 { 
   char Version;
   char Data_type;
   char Next_payload;
   char V_Prf_func;
   char* Csb_id;
   char Cs;
   char Cs_type;
   unsigned int Cs_info;

}MiNum;
MiNum.Version=data[0];
MiNum.Data_type=data[1];
MiNum.Next_payload=data[2];
MiNum.V_Prf_func=data[3];
Csb_id=new char[lenth-]
L=MiNum.Cs;

struct KEMAC
 {
     char Next_payload;
	 char Encr_alg;
	 char Encr_data_len;
	 char Encr_data;
}MacNum;

n_b=MacNum.Encr_data_len;

struct KEY
 {
	char Next_payload;
	unsigned char Type_kv;
	unsigned int Key_data_len;
    char* Key_data_buffer; 
	char Salt_len;
	char* Salt_data_buffer;
	char* Kv_data_buffer;
}KeyNum;

Saltlenth=KeyNum.Salt_len;
Salt_data=KeyNum.Salt_data_buffer;

struct Kv 
 {
    char SPI_len;
    char* SPI_data_buffer;
}KvNum;

struct K
 {
	char VF_lenth;
	char* From;
	char VT_lenth;
	char* To;
}KNum;

if(MiNum.Cs)
{
unsigned char i=0;
i=MiNum.Cs;
	struct Srtp
	{
		char Policy_no_i;
		char* Ssrc_i;
		char* Roc_i;
	}SrtpNum;
	SSRC=SrtpNum.Ssrc_i;
	ROC=SrtpNum.Roc_i;
}

struct Timestamp
 {
	char Next_payload;
	char Ts_type;
	char* Ts_value;
}TimestampNum;

if(TimestampNum.Ts_type=2)
TS=TimestampNum.Ts_value;

char Type=0,Kv=0;
char Name=0;
char SPI,Interval;0
char* Kv_data;
char* Key_data;
 char* Salt_data;

  		switch(Kv) 
		{
		case 0:
			Name=NULL;
			break;
		case 1:
			{
			Name=SPI;
			Kv_data=KeyNum.Kv_data_buffer;
			}
			break;
		case 2:
			{
			Name=Interval;
			if(Updata_time>KNum.From)
			{
				
				if(Updata_time<KNum.To)
				{	
				  if(MiNum.Data_type=1)//verification
					{
						MiNum.Next_payload=20; //key data
						unsigned char TypeBitmap=0xf0;
						unsigned char KvBitmap=0x0f;
						TypeBitmap&= KeyNum.Type_kv;
						Type=TypeBitmap;
						KvBitmap&=KeyNum.Type_kv;
						Kv=KvBitmap;
						int len;
						len=(int)KeyNum.Key_data_len;
				        int i=0;
						switch(Type) 
						{
						case 0:
							{
							Name=TGK;

							 for(i=0;i<len;i++)
							 {
								 Key_data=KeyNum.Key_data_buffer;
								 MSK=KeyNum.Key_data_buffer;
							 }	
							}
							break;
						case 1:
							{
							Name=TGK+SALT;
							  	for(i=0;i<len;i++)
								{
								Key_data=KeyNum.Key_data_buffer;
								Salt_data=KeyNum.Salt_data_buffer;
								}
							}
							break;
						case 2:
							{
							Name=TEK;
							 for(i=0;i<len;i++)
							 {
								Key_data=KeyNum.Key_data_buffer;
								MTK=KeyNum.Key_data_buffer;
								printf("MTK OK");
							 }
							}
							break;
						case 3:
							{
							Name=TEK+SALT;
							 for(i=0;i<len;i++)
							 {
								Key_data=KeyNum.Key_data_buffer;
								Salt_data=KeyNum.Salt_data_buffer;
							 }
							}
							break;
						default:printf("Error");
							break;
						} 
					
					}
				 else
				 {	
					 return 1;
                  printf("Req for new MTK");
				 }
				}
		    	else
				{
				return 1;
		    	printf("Req for new MTK");
				}
			}
			break;
		default:printf("Error");
			break;
		}
	 
	}
}



