#include "stdafx.h"
#include "Aes.h"
#include "UDPTORTP.h"

#define SRTP_PORT 0
#define MIKEY_PORT 2269
#define RTP_PORT 2


//////////////////////////////////////////////////////////////////////////
//解UDP包
///////////////////////////////////////////////////////////////////////////
void UDPTORTP::MDPC_MTKD_DeUDP(char* p,int lenth)
{
	char *put;
	put=new char[lenth];
	memcpy(put,p,lenth);
	char *out;
	out=new char[lenth];
	
	
	switch(port) 
	{
	case SRTP_PORT:
		{
			MDPC_MTKD_BMTK(put,lenth);
			MDPC_MTKD_DeRTP(put,lenth,out);
		}
		break;
	case MIKEY_PORT: //2269 MIKEY 
		{	   
			MDPC_MTKD_BMTK(put,lenth);
		}
		break;
	case RTP_PORT:
		MDPC_MTKD_DeRTP(put,lenth,out);
        break;
	default:
		printf("error\n");
		break;
	}
}
////////////////////////////////////////////////////////////////////////////////
//解MIKEY消息 
///////////////////////////////////////////////////////////////////////////////
void UDPTORTP::MDPC_MTKD_BMTK(char* data,int lenth)
{
	unsigned long i=0;
    int value=0;
	int	keylen=0,saltlen=0,Len=0;
	
	//初始化MIKEY包
	MikeyNum.Version=data[0];
	MikeyNum.Data_type=data[1];
	MikeyNum.Next_payload=data[2];
	MikeyNum.HDRNum.Version=data[3];
	MikeyNum.HDRNum.Data_type=data[4];
	MikeyNum.HDRNum.Next_payload=data[5];
	MikeyNum.HDRNum.V_Prf_func=data[6];
	temp=data[10];
	MikeyNum.HDRNum.Csb_id=temp<<24;
	temp=0;
	temp=data[9];
	MikeyNum.HDRNum.Csb_id += temp<<16;
	temp=0;
	temp=data[8];
	MikeyNum.HDRNum.Csb_id += temp<<8;
	temp=0;
	temp=data[8];
	MikeyNum.HDRNum.Csb_id +=temp;
	//MikeyNum.HDRNum.Csb_id=data[7]+data[8]<<8+data[9]<<16+data[10]<<24;
	MikeyNum.HDRNum.Cs=data[11];
	MikeyNum.HDRNum.Cs_type=data[12];
	for(i=0;i<(unsigned long)MikeyNum.HDRNum.Cs;i++)
	{
		MikeyNum.HDRNum.SRTP_IDNum.Policy_no_i=data[13];
		temp=0;
		temp=data[16];
		MikeyNum.HDRNum.SRTP_IDNum.SSRC_i=temp<<24;
		temp=0;
		temp=data[15];
		MikeyNum.HDRNum.SRTP_IDNum.SSRC_i+=temp<<16;
		temp=0;
		temp=data[14];
		MikeyNum.HDRNum.SRTP_IDNum.SSRC_i+=temp<<8;
		temp=0;
		temp=data[13];
		MikeyNum.HDRNum.SRTP_IDNum.SSRC_i+=temp;
		//MikeyNum.HDRNum.p_srtp_id->SSRC_i=data[13]+data[14]<<8+data[15]<<16+data[16]<<24;
		temp=0;
		temp=data[16];
		MikeyNum.HDRNum.SRTP_IDNum.ROC_i=temp<<24;
		temp=0;
		temp=data[15];
		MikeyNum.HDRNum.SRTP_IDNum.ROC_i+=temp<<16;
		temp=0;
		temp=data[14];
		MikeyNum.HDRNum.SRTP_IDNum.ROC_i+=temp<<8;
		temp=0;
		temp=data[13];
		MikeyNum.HDRNum.SRTP_IDNum.ROC_i+=temp;
		//MikeyNum.HDRNum.p_srtp_id->ROC_i=data[17]+data[18]<<8+data[19]<<16+data[20]<<24;
	}
	//类似循环链表//////////////////////////////////////////
	value=data[5];
	for(i=20;value;)
	{
		switch(value) 
		{
		case 1:
			{
				MikeyNum.PayloadNum.KEMACNum.Next_payload=data[i+1];
				MikeyNum.PayloadNum.KEMACNum.Encr_alg=data[i+2];
				MikeyNum.PayloadNum.KEMACNum.Encr_data_len=data[i+3];
				MikeyNum.PayloadNum.KEMACNum.KEYSUBNum.Next_payload=data[i+4];
				MikeyNum.PayloadNum.KEMACNum.KEYSUBNum.Type_kv=data[i+5];
				temp=0;
				temp=data[i+7];
				MikeyNum.PayloadNum.KEMACNum.KEYSUBNum.Key_data_len=temp<<8;
				temp=0;
				temp=data[i+6];
				MikeyNum.PayloadNum.KEMACNum.KEYSUBNum.Key_data_len+=temp;
				//MikeyNum.U_Payload.KEMACNum.KEYSUBNum.Key_data_len=data[i+6]+data[i+7]<<8;
				keylen=MikeyNum.PayloadNum.KEMACNum.KEYSUBNum.Key_data_len;
				memcpy(MikeyNum.PayloadNum.KEMACNum.KEYSUBNum.Key_data_buffer,data+i+8,keylen);
				MikeyNum.PayloadNum.KEMACNum.KEYSUBNum.Salt_len=data[i+8+keylen];
				saltlen=data[i+8+keylen];
				memcpy(MikeyNum.PayloadNum.KEMACNum.KEYSUBNum.Salt_data_buffer,data+i+9+keylen,saltlen);
				Len=keylen+saltlen;
				int Len_spi=0,Len_in=0;
				switch(data[i+5]&0x0f) 
				{
				case 0:
					i=Len+9+i;
					break;
				case 1:
					{
						MikeyNum.PayloadNum.KEMACNum.KEYSUBNum.KVNum.Kv_spiNum.SPI_len=data[i+10+Len];
						Len_spi=data[i+10+Len];
						memcpy(MikeyNum.PayloadNum.KEMACNum.KEYSUBNum.KVNum.Kv_spiNum.SPI_data_buffer,data+i+11+Len,Len_spi);
						i=Len_spi+Len+11+i;
					}
					break;
				case 3:
					{
						MikeyNum.PayloadNum.KEMACNum.KEYSUBNum.KVNum.Kv_inNum.VF_lenth=data[i+10+Len];
						Len_in=data[i+10+Len];
						memcpy(MikeyNum.PayloadNum.KEMACNum.KEYSUBNum.KVNum.Kv_inNum.From,data+i+11+Len,Len_in);
						Len=Len_in+Len;
						MikeyNum.PayloadNum.KEMACNum.KEYSUBNum.KVNum.Kv_inNum.VT_lenth=data[i+12+Len];
						Len_in=data[i+12+Len];
						memcpy(MikeyNum.PayloadNum.KEMACNum.KEYSUBNum.KVNum.Kv_inNum.To,data+i+13+Len,Len_in); 
						i=Len+Len_in+13+i;					 
					}
					break;
				default:
					{
						i=Len+9+i;
						printf("Error");
					}				
					break;
				}
				value=MikeyNum.PayloadNum.KEMACNum.Next_payload;
			}
			break;
		case 4:
			{
				temp=0;
				temp=data[i+2];
				MikeyNum.PayloadNum.SignNum.S_sign_len=temp<<8;
				temp=0;
				temp=data[i+1];
				MikeyNum.PayloadNum.SignNum.S_sign_len=temp+MikeyNum.PayloadNum.SignNum.S_sign_len;
				//MikeyNum.U_Payload.SignNum.S_sign_len=data[i+1]+data[i+2]<<8;
				Len=MikeyNum.PayloadNum.SignNum.S_sign_len;
				memcpy(MikeyNum.PayloadNum.SignNum.Sdata,data+i+3,Len);
				i=Len+3+i;
				value=0;
			}
			break;
		case 5:
			{
				MikeyNum.PayloadNum.TimestampNum.Next_payload=data[i+1];
				MikeyNum.PayloadNum.TimestampNum.Ts_type=data[i+2];
				if(data[i+2]==2)
				{
					memcpy(MikeyNum.PayloadNum.TimestampNum.Ts_value,data+i+3,4);
					i=i+3+4;
				}
				if(data[i+2]==1)
				{
					memcpy(MikeyNum.PayloadNum.TimestampNum.Ts_value,data+i+3,8);
					i=i+3+8;
				}
				if(data[i+2]==0)
				{
					memcpy(MikeyNum.PayloadNum.TimestampNum.Ts_value,data+i+3,8);
					i=i+3+8;
				}
				value=MikeyNum.PayloadNum.TimestampNum.Next_payload;
			}
			break;
		case 6:
			{
				MikeyNum.PayloadNum.IDNum.Next_payload=data[i+1];
				MikeyNum.PayloadNum.IDNum.Itype=data[i+2];
				temp=0;
				temp=data[i+4];
				MikeyNum.PayloadNum.IDNum.len=temp<<8;
				temp=0;
				temp=data[i+3];
				MikeyNum.PayloadNum.IDNum.len=temp+MikeyNum.PayloadNum.IDNum.len;
				//MikeyNum.U_Payload.IDNum.len=data[i+3]+data[i+4]<<8;
				Len=MikeyNum.PayloadNum.IDNum.len;
				memcpy(MikeyNum.PayloadNum.IDNum.data,data+i+5,Len);
				i=Len+i+5;
				value=MikeyNum.PayloadNum.IDNum.Next_payload;
			}
			break;
		case 7:
			{
				MikeyNum.PayloadNum.CertNum.Next_payload=data[i+1];
				MikeyNum.PayloadNum.CertNum.Itype=data[i+2];
				temp=0;
				temp=data[i+4];
				MikeyNum.PayloadNum.CertNum.len=temp<<8;
				temp=0;
				temp=data[i+3];
				MikeyNum.PayloadNum.CertNum.len=temp+MikeyNum.PayloadNum.CertNum.len;
				//MikeyNum.U_Payload.CertNum.len=data[i+3]+data[i+4]<<8;
				Len=MikeyNum.PayloadNum.CertNum.len;
				memcpy(MikeyNum.PayloadNum.CertNum.data,data+i+5,Len);
				i=Len+i+5;
				value=MikeyNum.PayloadNum.CertNum.Next_payload;
			}
			break;
		case 8:
			{
				MikeyNum.PayloadNum.HashNum.Next_payload=data[i+1];
				MikeyNum.PayloadNum.HashNum.Func=data[i+2];
				if (data[i+2]==0)
				{
					memcpy(MikeyNum.PayloadNum.HashNum.data,data+i+3,20);
					i=i+3+20;
				}
				else
					if(data[i+2]==1)
					{
						memcpy(MikeyNum.PayloadNum.HashNum.data,data+i+3,16);
						i=i+3+16;
					}
					value=MikeyNum.PayloadNum.HashNum.Next_payload;
            }
			break;
		case 9:
			{
				MikeyNum.PayloadNum.VNum.Next_payload=data[i+1];
				MikeyNum.PayloadNum.VNum.Auth=data[i+2];
				Len=data[i+2];
				memcpy(MikeyNum.PayloadNum.VNum.data,data+i+3,Len);
				i=i+3+Len;
				value=MikeyNum.PayloadNum.VNum.Next_payload;
			}
			break;
		case 10:
			{
				MikeyNum.PayloadNum.SPNum.Next_payload=data[i+1];
				MikeyNum.PayloadNum.SPNum.Policy_no=data[i+2];
				MikeyNum.PayloadNum.SPNum.Prot_type=data[i+3];
				temp=0;
				temp=data[i+5];
				MikeyNum.PayloadNum.SPNum.Param_len=temp<<8;
				temp=0;
				temp=data[i+4];
				MikeyNum.PayloadNum.SPNum.Param_len=temp+MikeyNum.PayloadNum.SPNum.Param_len;
				//MikeyNum.U_Payload.SPNum.Param_len=data[i+4]+data[i+5]<<8;
				MikeyNum.PayloadNum.SPNum.ParamNum.Type=data[i+6];
				MikeyNum.PayloadNum.SPNum.ParamNum.Len=data[i+7];
				Len=data[i+7];
				memcpy(MikeyNum.PayloadNum.SPNum.ParamNum.Value,data+i+8,Len);
				i=i+8+Len;
				value=MikeyNum.PayloadNum.SPNum.Next_payload;
			}
			break;
		case 11:
			{
				MikeyNum.PayloadNum.RandNum.Next_payload=data[i+1];
				MikeyNum.PayloadNum.RandNum.Rand_Len=data[i+2];
				Len=data[i+2];
				memcpy(MikeyNum.PayloadNum.RandNum.data,data+i+2,Len);
				i=i+2+Len;
				value=MikeyNum.PayloadNum.RandNum.Next_payload;
			}
			break;
		case 12:
			{
				MikeyNum.PayloadNum.ErrNum.Next_payload=data[i+1];
				MikeyNum.PayloadNum.ErrNum.Err_no=data[i+2];
				temp=0;
				temp=data[i+4];
				MikeyNum.PayloadNum.ErrNum.Reserved=temp<<8;
				temp=0;
				temp=data[i+3];
				MikeyNum.PayloadNum.ErrNum.Reserved=temp+MikeyNum.PayloadNum.ErrNum.Reserved;
				//MikeyNum.U_Payload.ErrNum.Reserved=data[i+3]+data[i+4]<<8;
				i=i+4;
				value=MikeyNum.PayloadNum.ErrNum.Next_payload;
			}
			break;
		case 20:
			{
				MikeyNum.PayloadNum.KEYNum.Next_payload=data[i+4];
				MikeyNum.PayloadNum.KEYNum.Type_kv=data[i+5];
				temp=0;
				temp=data[i+7];
				MikeyNum.PayloadNum.KEYNum.Key_data_len=temp<<8;
				temp=0;
				temp=data[i+6];
				MikeyNum.PayloadNum.KEYNum.Key_data_len=temp+MikeyNum.PayloadNum.KEYNum.Key_data_len;
				//MikeyNum.U_Payload.KEYNum.Key_data_len=data[i+6]+data[i+7]<<8;
				keylen=MikeyNum.PayloadNum.KEYNum.Key_data_len;
				memcpy(MikeyNum.PayloadNum.KEYNum.Key_data_buffer,data+i+8,keylen);
				MikeyNum.PayloadNum.KEYNum.Salt_len=data[i+8+keylen];
				saltlen=data[i+8+keylen];
				memcpy(MikeyNum.PayloadNum.KEYNum.Salt_data_buffer,data+i+9+keylen,saltlen);
				Len=keylen+saltlen;
				int Len_spi=0,Len_in=0;
				switch(data[i+5]&0x0f) 
				{
				case 0:
					i=Len+9+i;
					break;
				case 1:
					{
						MikeyNum.PayloadNum.KEYNum.KVNum.Kv_spiNum.SPI_len=data[i+10+Len];
						Len_spi=data[i+10+Len];
						memcpy(MikeyNum.PayloadNum.KEYNum.KVNum.Kv_spiNum.SPI_data_buffer,data+i+11+Len,Len_spi);
						i=Len_spi+Len+11+i;
					}
					break;
				case 3:
					{
						MikeyNum.PayloadNum.KEYNum.KVNum.Kv_inNum.VF_lenth=data[i+10+Len];
						Len_in=data[i+10+Len];
						memcpy(MikeyNum.PayloadNum.KEYNum.KVNum.Kv_inNum.From,data+i+11+Len,Len_in);
						Len=Len_in+Len;
						MikeyNum.PayloadNum.KEYNum.KVNum.Kv_inNum.VT_lenth=data[i+12+Len];
						Len_in=data[i+12+Len];
						memcpy(MikeyNum.PayloadNum.KEYNum.KVNum.Kv_inNum.To,data+i+13+Len,Len_in); 
						i=Len+Len_in+13+i;					 
					}
					break;
				default:printf("Error");
					break;
				}
				value=MikeyNum.PayloadNum.KEYNum.Next_payload;
			}
			break;			
		case 21:
			{
				MikeyNum.PayloadNum.Gen_ext.Next_payload=data[i+1];
				MikeyNum.PayloadNum.Gen_ext.Type=data[i+2];
				temp=0;
				temp=data[i+4];
				MikeyNum.PayloadNum.Gen_ext.Len=temp<<8;
				temp=0;
				temp=data[i+3];
				MikeyNum.PayloadNum.Gen_ext.Len=temp+MikeyNum.PayloadNum.Gen_ext.Len;
				//MikeyNum.U_Payload.Gen_ext.Len=data[i+3]+data[i+4]<<8;
				Len=MikeyNum.PayloadNum.Gen_ext.Len;
				memcpy(MikeyNum.PayloadNum.Gen_ext.data,data+i+5,Len);
				i=i+5+Len;
				value=MikeyNum.PayloadNum.Gen_ext.Next_payload;
			}
			break;
		default:
			{
				i=20;
				printf("Error");
			}
			break;
		}
}
////////////////////////////////////////////////////////
temp=0;
temp=data[i+4];
MikeyNum.Mac_Signature=temp<<24;
temp=0;
temp=data[i+3];
MikeyNum.Mac_Signature+=temp<<16;
temp=0;
temp=data[i+2];
MikeyNum.Mac_Signature+=temp<<8;
temp=0;
temp=data[i+1];
MikeyNum.Mac_Signature+=temp;
//MikeyNum.Mac_Signature=data[i+1]+data[i+2]<<8+data[i+3]<<16+data[i+4]<<24;

//////////////////////////////////////////////////////////////////////////////////
//MIKEY包初始化完成
//////////////////////////////////////////////////////////////////////////////////
/*
MiNum.Version=data[0];
MiNum.Data_type=data[1];
MiNum.Next_payload=data[2];
MiNum.V_Prf_func=data[3];
Csb_id=new char[lenth-]
*/

L=MikeyNum.HDRNum.Cs;   //MiNum.Cs;
n_b=MikeyNum.PayloadNum.KEMACNum.Encr_data_len;//MacNum.Encr_data_len;
Saltlenth=MikeyNum.PayloadNum.KEMACNum.KEYSUBNum.Salt_len; //KeyNum.Salt_len;
Salt_data=MikeyNum.PayloadNum.KEMACNum.KEYSUBNum.Key_data_buffer; 
//KeyNum.Salt_data_buffer
/*
if(MiNum.Cs
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
*/
if(TimestampNum.Ts_type=2)
TS=MikeyNum.PayloadNum.TimestampNum.Ts_value;
char Type=0,Kv=0;
char Name=0;
char SPI=0,Interval=0;
char* Kv_data;
char* Key_data;
char* Salt_data;
Kv=MikeyNum.PayloadNum.KEMACNum.KEYSUBNum.Type_kv&0x0f;
switch(Kv) 
{
case 0:
	Name=NULL;
	break;
case 1:
	{
		Name=SPI;
		Kv_data=MikeyNum.PayloadNum.KEMACNum.KEYSUBNum.KVNum.Kv_spiNum.SPI_data_buffer; //KeyNum.Kv_data_buffer;
	}
	break;
case 2:
	{
		Name=Interval;
		if(Updata_time>MikeyNum.PayloadNum.KEMACNum.KEYSUBNum.KVNum.Kv_inNum.From)//KNum.From
		{
			
			if(Updata_time<MikeyNum.PayloadNum.KEMACNum.KEYSUBNum.KVNum.Kv_inNum.To)//KNum.To
			{	
				if(MikeyNum.Data_type=0)//verification  //MiNum.Data_type
				{
					//	MikeyNum.Next_payload=1; //key data
					unsigned char TypeBitmap=0xf0;
					unsigned char KvBitmap=0x0f;
					TypeBitmap&=MikeyNum.PayloadNum.KEMACNum.KEYSUBNum.Type_kv; //  KeyNum.Type_kv
					Type=TypeBitmap>>4;
					KvBitmap&=MikeyNum.PayloadNum.KEMACNum.KEYSUBNum.Type_kv;
					Kv=KvBitmap;
					Len=MikeyNum.PayloadNum.KEMACNum.KEYSUBNum.Key_data_len;  //KeyNum.Key_data_len;
					switch(Type) 
					{
					case 0:
						{
							Name=TGK;
							for(i=0;i<(unsigned long)Len;i++)
							{
								Key_data=MikeyNum.PayloadNum.KEMACNum.KEYSUBNum.Key_data_buffer; //KeyNum.Key_data_buffer;
								MSK=Key_data;  //KeyNum.Key_data_buffer;
							}	
						}
						break;
					case 1:
						{
							Name=TGK+SALT;
							for(i=0;i<(unsigned long)Len;i++)
							{
								Key_data=MikeyNum.PayloadNum.KEMACNum.KEYSUBNum.Salt_data_buffer; //KeyNum.Key_data_buffer
								Salt_data=Key_data;
							}
						}
						break;
					case 2:
						{
							Name=TEK;
							for(i=0;i<(unsigned long)Len;i++)
							{
								Key_data=MikeyNum.PayloadNum.KEMACNum.KEYSUBNum.Key_data_buffer;  //KeyNum.Key_data_buffer;
								MTK=Key_data;
								printf("MTK OK");
							}
						}
						break;
					case 3:
						{
							Name=TEK+SALT;
							for(i=0;i<(unsigned long)Len;i++)
							{
								Key_data=MikeyNum.PayloadNum.KEMACNum.KEYSUBNum.Salt_data_buffer;
								Salt_data=Key_data;
							}
						}
						break;
					default:printf("Error");
						break;
					} 
					
				}
				else
				{	
					return;
					printf("Req for new MTK");
				}
			}
			else
			{
				return;
				printf("Req for new MTK");
			}
		}
		break;
default:printf("Error");
	break;
	}
	
}
}
////////////////////////////////////////////////////////////////////////////////
//解SRTP包为RTP 
////////////////////////////////////////////////////////////////////////////////
void UDPTORTP::MDPC_MTKD_DeRTP(char* buffer,int lenth,char* output)
{
	ISD_Aes	*MyAes;
	output=new char[lenth];
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
	char* k_s;
	int i=0,j=0;
	/////////////////////////////////////////////////////////////////////////////////
	//SRTP消息初始化 
	////////////////////////////////////////////////////////////////////////////////
	int Len=0;
	SRTPNum.V_CC=buffer[0];
	SRTPNum.M_TP=buffer[1];
    temp=0;
	temp=buffer[3];
	SRTPNum.SeqNum=temp<<8;
	temp=0;
	temp=buffer[2];
	SRTPNum.SeqNum+=temp;
	//SRTPNum.SeqNum=buffer[2]+buffer[3]<<8;
	temp=0;
	temp=buffer[7];
	SRTPNum.Timestamp=temp<<24;
	temp=0;
	temp=buffer[6];
	SRTPNum.Timestamp+=temp<<16;
	temp=0;
	temp=buffer[5];
	SRTPNum.Timestamp+=temp<<8;
	temp=0;
	temp=buffer[4];
	SRTPNum.Timestamp+=temp;
	//SRTPNum.Timestamp=buffer[4]+buffer[5]<<8+buffer[6]<<16+buffer[7]<<24;
	SRTPNum.SSRC=SRTPNum.Timestamp;
	temp=0;
	temp=buffer[13];
	SRTPNum.HeadExtNum.Profile=temp<<8;
	temp=0;
	temp=buffer[12];
	SRTPNum.HeadExtNum.Profile+=temp;
	//SRTPNum.HeadExtNum.Profile=buffer[12]+buffer[13]<<8;
	temp=0;
	temp=buffer[15];
	SRTPNum.HeadExtNum.Len=temp<<8;
	temp=0;
	temp=buffer[14];
	SRTPNum.HeadExtNum.Len+=temp;
	//SRTPNum.HeadExtNum.Len=buffer[14]+buffer[15]<<8;
	Len=SRTPNum.HeadExtNum.Len;
	memcpy(SRTPNum.HeadExtNum.Ext,buffer+16,Len);
	memcpy(SRTPNum.Payload,buffer+17+Len,buffer[1]&0x7f);
	
	
	/////////////////////////////////////////////////////////////////////////////////
	//初始化完成
	/////////////////////////////////////////////////////////////////////////////////
	SEQ=SRTPNum.SeqNum;
	SRTPpacket=SRTPNum.Payload;
	unsigned char TypeBitmap=0x7f;
	unsigned char KvBitmap=0x80;
	M=(SRTPNum.M_TP&TypeBitmap)>>7;
	PT=SRTPNum.M_TP&KvBitmap;
	master_salt=Salt_data;   //mtk.Salt_data
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
		MyAes = new ISD_Aes(32,(unsigned char*)master_key);
		//MyAes.Aes();
		MyAes->ISD_Cipher((unsigned char*)keyinput,(unsigned char*)output); //加密
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
		MyAes = new ISD_Aes(32,(unsigned char*)master_key);
		// MyAes.Aes();
		MyAes->ISD_Cipher((unsigned char*)keyinput,(unsigned char*)output); 
		char* p=0;
		int p_lenth=0;
		p=output;
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
	for(i=0;i<L;i++)
	{
		IV_temp[i]=TS[i]||ROC[i]||SSRC[i];
	}
	IV_lenth=strlen(IV_temp);
	IV_temp[IV_lenth-2]=IV_temp[IV_lenth-2]||(half&0xff);
	IV_temp[IV_lenth-1]=IV_temp[IV_lenth-1]||((half>>8)&0xff);
	strcpy(IV,IV_temp);
	IV=IV_temp;
	//IV
	   // mtk.TS||mtk.SSRC||mtk.ROC;
	for(i=0;i<Saltlenth;i++)   
		m[i]=(k_s[i]||0x55)^k_e[i]; 
	strcpy(k_e_m,m);
	//k_e_m=k_e^m;
	MyAes=new ISD_Aes(32,(unsigned char*)k_e_m);
	MyAes->ISD_Cipher((unsigned char*)IV,(unsigned char*)output);  
	memcpy(IW,output,L);
	//L=N/n_b;   
    for(j=0;j<L;j++)
	{
		s(j,output);
		RtpCaculate(j,put,output);
	}
	//buffer=SRTPNum.Payload;
}
///////////////////////////////////////////////////////////////////////////////////
//SRTP包解为RTP包解包结束
///////////////////////////////////////////////////////////////////////////////////

void UDPTORTP::RtpCaculate(int m,const char* key,char* output)
{
	int i=0;
	for(i=0;i<m;i++)
		RTPNum.payload[i]=SRTPNum.Payload[i]^key[i];
	output=SRTPNum.Payload;
    RTPNum.length=m+RTPNum.length;
}


void UDPTORTP::keyCaculate(int m,const char* key1,const char* key2,char* output)
{
    int i=0;
	for(i=0;i<m;i++)
		stream[i]=key2[i]^key1[i];
	stream[m-2]=stream[m-2]||(m&0xff);
	stream[m-1]=stream[m-1]||((m>>8)&0xff);
	output=stream;
}

// S(j) = E(k_e, IV' XOR j XOR S(j-1))

UDPTORTP::s(int i,char* output)
{
	ISD_Aes	*MyAes;
	if (i==-1)
		state=0;
	put=new char[i];
	keyCaculate(i,IW,output,put);
    //stream=j^IW^s(j-1);
	//MyAes.Aes(24,k_e);
	MyAes=new ISD_Aes(32,(unsigned char*)k_e);
	MyAes->ISD_Cipher((unsigned char*)stream,(unsigned char*)put);
	//	RtpCaculate(i,put,);
}































































