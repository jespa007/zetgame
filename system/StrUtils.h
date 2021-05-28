#ifndef __STRING_H__
#define __STRING_H__

typedef enum{
	CHAR_TYPE_CHAR=0,
	CHAR_TYPE_WCHAR,

}CharType;


char *			StrUtils_StrClone(const char *in_str);
char *			StrUtils_StrCloneN(const char *in, size_t len);
char * 			StrUtils_ToLower(const char *input);
List * 			StrUtils_StrSplitStr(const char * s, const char *str_delim);
List * 			StrUtils_StrSplit(const char * s, char delim);
void			StrUtils_StrReplace(const char * str_input, char old_char,char new_char);

List * 			StrUtils_WStrSplit(const wchar_t * s, wchar_t delim);
List * 			StrUtils_WStrSplitWStr(const wchar_t * s, const wchar_t *str_delim);

/**
 * Converts input int to allocated string. Should be deallocated when not used anymore
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


#endif
