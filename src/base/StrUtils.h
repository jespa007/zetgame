#ifndef __STRING_H__
#define __STRING_H__

typedef enum{
	CHAR_TYPE_CHAR=0,
	CHAR_TYPE_WCHAR,

}CharType;

#define	 STR_MAX			4096


#define 	STR_CAPTURE_BASE(_str_out, _first_param, _str_in)\
{va_list  ap;\
int max_len=(int)((sizeof(_str_out)/sizeof(char))-1);\
va_start(ap, _first_param);\
int n=vsnprintf(_str_out,max_len,_str_in,  ap);\
if(n>=(int)max_len || n==-1){\
	_str_out[max_len-1]=0;\
	_str_out[max_len-2]='.';\
	_str_out[max_len-3]='.';\
	_str_out[max_len-4]='.';\
}\
va_end(ap);}

#define 	STR_CAPTURE_VARGS(_str_out, _str_in)\
STR_CAPTURE_BASE(_str_out,_str_in,_str_in)\


char *			StrUtils_StrClone(const char *in_str);
char *			StrUtils_StrCloneN(const char *in, size_t len);
char * 			StrUtils_ToLower(const char *input);
List * 			StrUtils_StrSplitStr(const char * s, const char *str_delim);
List * 			StrUtils_StrSplit(const char * s, char delim);
void			StrUtils_StrReplace(const char * str_input, char old_char,char new_char);

List * 			StrUtils_WStrSplit(const wchar_t * s, wchar_t delim);
List * 			StrUtils_WStrSplitWStr(const wchar_t * s, const wchar_t *str_delim);

/**
 * Converts input int/float to allocated string. Should be deallocated when not used anymore
 */
char 	* 		StrUtils_IntToStr(int input);
char	*		StrUtils_FloatToStr(float input);
bool 			StrUtils_StrToInt(int * i, const char *s, int base);
bool 			StrUtils_StrToFloat(float * f, const char *s);
/**
 * Converts input float to allocated string. Should be deallocated when not used anymore
 */
bool 			StrUtils_EndsWith(const char * str, const char * ending);
void 			StrUtils_Remove(char * str, char ch_to_remove);
bool			StrUtils_IsDigit(char c);
bool			StrUtils_WStrIsNullOrEmpty(const wchar_t *str);
bool			StrUtils_StrIsNullOrEmpty(const char *str);


wchar_t         *StrUtils_ToWStr(const char *str_in);

unsigned long 	StrUtils_GetCharAndAdvance(void **ptr, CharType char_text);
unsigned long 	StrUtils_GetChar(void *ptr, CharType char_text);
void 			StrUtils_Advance(void **ptr, CharType char_text);


#endif
