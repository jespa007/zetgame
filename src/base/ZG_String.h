#ifndef __ZG_STRING_H__
#define __ZG_STRING_H__

typedef enum{
	ZG_CHAR_TYPE_CHAR=0,
	ZG_CHAR_TYPE_WCHAR,

}ZG_CharType;

#define	 ZG_STR_MAX			4096


#define ZG_STR_CONCAT(s1,s2)	s1#s2

#define 	ZG_STR_CAPTURE_BASE(_str_out, _first_param, _str_in)\
{va_list  ap;\
int max_len=(int)((sizeof(_str_out)/sizeof(char))-1);\
va_start(ap, _first_param);\
printf("s:%s max_len:%i %s\n",_str_out,max_len,_str_in);\
int n=vsnprintf(_str_out,max_len,_str_in,  ap);\
if(n>=(int)max_len || n==-1){\
	_str_out[max_len-1]=0;\
	_str_out[max_len-2]='.';\
	_str_out[max_len-3]='.';\
	_str_out[max_len-4]='.';\
}\
va_end(ap);}


#define 	ZG_STR_CAPTURE_VARGS(_str_out, _str_in)\
ZG_STR_CAPTURE_BASE(_str_out,_str_in,_str_in)\


char *			ZG_String_Clone(const char *in_str);
char *			ZG_String_CloneN(const char *in, size_t len);
char * 			ZG_String_ToLower(const char *input);
ZG_List * 		ZG_String_SplitString(const char * s, const char *str_delim);
ZG_List * 		ZG_String_SplitChar(const char * s, char delim);
void			ZG_String_Replace(const char * str_input, char old_char,char new_char);

ZG_List * 			ZG_String_WSplitChar(const wchar_t * s, wchar_t delim);
ZG_List * 			ZG_String_WSplitString(const wchar_t * s, const wchar_t *str_delim);

/**
 * Converts input int/float to allocated string. Should be deallocated when not used anymore
 */
char 	* 		ZG_String_IntToString(int input);
char	*		ZG_String_FloatToString(float input);
bool 			ZG_String_StringToInt(int * i, const char *s, int base);
bool 			ZG_String_StringToFloat(float * f, const char *s);
/**
 * Converts input float to allocated string. Should be deallocated when not used anymore
 */
bool 			ZG_String_EndsWith(const char * str, const char * ending);
void 			ZG_String_Remove(char * str, char ch_to_remove);
bool			ZG_String_IsDigit(char c);
bool			ZG_String_WIsNullOrEmpty(const wchar_t *str);
bool			ZG_String_IsNullOrEmpty(const char *str);


wchar_t         *ZG_String_ToWString(const char *str_in);

unsigned long 	ZG_String_GetCharAndAdvance(void **ptr, ZG_CharType char_text);
unsigned long 	ZG_String_GetChar(void *ptr, ZG_CharType char_text);
void 			ZG_String_Advance(void **ptr, ZG_CharType char_text);


#endif
