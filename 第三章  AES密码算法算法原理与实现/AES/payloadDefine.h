#ifndef __PAYLOADDEFINE_H
#define __PAYLOADDEFINE_H

struct T_Kv_spi
{
    char SPI_len;
    char* SPI_data_buffer;
}Kv_spiNum;

struct T_Kv_in
{
	char VF_lenth;
	char* From;
	char VT_lenth;
	char* To;
}Kv_inNum;

struct T_KEY
{
	char Next_payload;
	unsigned char Type_kv;
	unsigned int Key_data_len;
    char* Key_data_buffer; 
	char Salt_len;
	char* Salt_data_buffer;
	//char* Kv_data_buffer;
	union U_KV
	{
		T_Kv_spi Kv_spiNum;
		T_Kv_in Kv_inNum;
	}KVNum;
}KEYNum;

struct T_KEMAC
{
	char Next_payload;
	char Encr_alg;
	char Encr_data_len;
	//	char Encr_data;
	T_KEY KEYNum;
}KEMACNum;

struct T_Sign
 {
	unsigned int S_sign_len;
	char* Sdata;
}SignNum;

struct T_Timestamp
{
	char Next_payload;
	char Ts_type;
	char* Ts_value;
}TimestampNum;

struct T_ID
 {
	char Next_payload;
	char Itype;
	unsigned int len;
	char* data;
}IDNum;

struct T_Hash
 {
	char Next_payload;
	char Func;
	char* data;
}HashNum;

struct T_V
 {
   char Next_payload;
   char Auth;
   char* data;
}VNum;

struct T_Param
 {
	char Type;
	char Len;
	char* Value;
}ParamNum;

struct T_SP
 {
    char Next_payload;
	char Policy_no;
	char Prot_type;
    unsigned int Param_len;
//	char* Param_data;
    T_Param ParamNum;
}SPNum;

struct T_Rand
 {
	char Next_payload;
	char Rand_Len;
	char* data;
}RandNum;

struct Err
 {
   char Next_payload;
   char Err_no;
   unsigned int Reserved;
}ErrNum;

struct Gen_ext
 {
	char Next_payload;
	char Type;
	unsigned int Len;
	char* data;
}Gen_ext;


#endif