#include "@zg_base.h"

//------------------------------------------------------------------------------

static ZG_List  * ZG_String_WSplitCharByAnyWCharInDelim(const wchar_t *str_in, const wchar_t *str_delim) {
	ZG_List *elems=ZG_List_New();

	if(str_in!=0 && wcscmp(str_in,L"")!=0){

	#ifdef __linux__
		wchar_t *ptr;
	#endif
		wchar_t * token = wcstok((wchar_t *)str_in, str_delim
	#ifdef __linux__
				,&ptr
	#endif
				);
	   // loop through the string to extract all other tokens
	   while( token != NULL ) {
		   size_t len=wcslen(token)*sizeof(wchar_t);
		   wchar_t *text=malloc(len+sizeof(wchar_t));
		   wcscpy(text,token);
		   ZG_List_Add(elems,text);
		   token = wcstok(NULL, str_delim
	#ifdef __linux__
				   ,&ptr
	#endif
				   );
	   }
   }

   return elems;
}

static ZG_List * ZG_String_SplitCharByAnyCharInDelim(const char *str_in, const char *str_delim) {
	ZG_List *elems=ZG_List_New();
	//char delim_str[2]={delim,0};
	char * token = strtok((char *)str_in, str_delim);
   // loop through the string to extract all other tokens
   while( token != NULL ) {
	   size_t len=strlen(token)+1;
	   char *text=malloc(len+1);
	   strcpy(text,token);
	   ZG_List_Add(elems,text);
	   token = strtok(NULL, str_delim);
   }

   return elems;
}

//------------------------------------------------------------------------------

char *ZG_String_New(const char *in_str){

	if(in_str==NULL){
		ZG_LOG_WARNINGF("input string as NULL");
		return NULL;
	}

	char *out_str=NULL;
	size_t size=strlen(in_str);
	if(size > 0){
		out_str=malloc(size+1);
		memset(out_str,0,size+1);
		strcpy(out_str,in_str);
	}else{
		ZG_LOG_WARNINGF("input lenght string 0");
	}

	return out_str;
}

char *			ZG_String_CloneN(const char *in_str, size_t len){
	if(in_str==NULL){
		ZG_LOG_WARNINGF("input string as NULL");
		return NULL;
	}

	char *out_str=NULL;
	size_t size=strlen(in_str);
	if(size > 0){
		size_t min_size=ZG_MIN(size,len);
		out_str=malloc(min_size+1);
		memset(out_str,0,size+1);
		memcpy(out_str,in_str,min_size);
	}else{
		ZG_LOG_WARNINGF("input lenght string 0");
	}

	return out_str;
}

char *			ZG_String_Clone(const char *in_str){
	return ZG_String_CloneN(in_str, strlen(in_str));
}



char * ZG_String_ToLower(const char *input){
	char *output=NULL;
	size_t ouput_len=strlen(input);

	if(ouput_len>0){
		output=malloc(ouput_len+1);
		memset(output,0,ouput_len+1);
		for(unsigned i=0; i < ouput_len;i++){
			output[i]=tolower(input[i]);
		}
	}

	return output;
}

ZG_List * ZG_String_WSplitChar(const wchar_t * str_in, wchar_t delim) {

	wchar_t str_delim[2]={delim,0};
	return ZG_String_WSplitCharByAnyWCharInDelim(str_in, str_delim);
}


ZG_List *  ZG_String_SplitChar(const char *str_in, char delim) {
	char str_delim[2]={delim,0};
	return ZG_String_SplitCharByAnyCharInDelim(str_in,str_delim);
}


ZG_List *  ZG_String_SplitString(const char *str_in, const char *delim) {
    ZG_List *elems=ZG_List_New();
    char *str_it = (char *)str_in;
    char *str_found = NULL;
    size_t size_delim=strlen(delim);
    size_t size_end=0;
    char *token=NULL;
    while ((str_found=strstr(str_it,delim))!=NULL){
    	size_t word_len=str_found-str_it;
    	if(word_len>0){
			token=malloc((word_len+1)*sizeof(char));
			memset(token,0,(word_len+1)*sizeof(char));
			strncpy(token,str_it,word_len);
			ZG_List_Add(elems,token);
    	}
        str_it=str_found+size_delim;
    }
    // adds the last word
    size_end=strlen(str_it)+1;
	token=malloc((size_end+1)*sizeof(char));
    memset(token,0,(size_end+1)*sizeof(char));
    memcpy(token,str_it,size_end);
    ZG_List_Add(elems,token);
    return elems;
}

ZG_List *  ZG_String_WSplitString(const wchar_t *str_in, const wchar_t *delim) {
    ZG_List *elems=ZG_List_New();
    wchar_t *str_it = (wchar_t *)str_in;
    wchar_t *str_found = NULL;
    size_t size_delim=wcslen(delim);
    size_t size_end=0;
    wchar_t *token=NULL;
    while ((str_found=wcsstr(str_it,delim))!=NULL){
    	size_t word_len=str_found-str_it;
    	if(word_len>0){
			token=malloc((word_len+1)*sizeof(wchar_t));
			memset(token,0,(word_len+1)*sizeof(wchar_t));
			wcsncpy(token,str_it,word_len);
			ZG_List_Add(elems,token);
    	}
        str_it=str_found+size_delim;
    }
    // adds the last word
    size_end=wcslen(str_it)+1;
	token=malloc((size_end+1)*sizeof(wchar_t));
    memset(token,0,(size_end+1)*sizeof(wchar_t));
    wcsncpy(token,str_it,size_end);
    ZG_List_Add(elems,token);
    return elems;
}

void ZG_String_Replace(const char * str_input, char ch_old,char ch_new){
	char *str = (char *)str_input;
	for (; *str!=0; str++) {
		if (*str == ch_old){
		  *str = ch_new;
		}
	}
}


/**
 * Given an number as a parameter, it returns its equivalent string. Should deallocated when is not used anymore
 */
char * ZG_String_IntToString(int input){
	char number[1000]={0};
	sprintf(number,"%i",input);
	return ZG_String_New((const char *)number);
}

char	*	ZG_String_FloatToString(float input){
	char number[1000]={0};
	sprintf(number,"%f",input);

	return ZG_String_New((const char *)number);
}

/**
 * Given an string as a parameter, it converts in its equivalent number. Return false if conversion failed
 */
bool ZG_String_StringToInt(int * i, const char *s, int base){
	 char *end;
	long  l;
	errno = 0;
	l = strtol(s, &end, base);
	if ((errno == ERANGE && l == LONG_MAX) || l > INT_MAX) {
		ZG_LOG_ERROR("\"%s\" number overflow",s);
		return false;
	}
	if ((errno == ERANGE && l == LONG_MIN) || l < INT_MIN) {
		ZG_LOG_ERROR("\"%s\" number underflow",s);
		return false;

	}
	if (*s == '\0' || *end != '\0') {
		ZG_LOG_ERROR("\"%s\" number inconvertible",s);
		return false;
	}
	*i = l;
	return true;
}

bool ZG_String_StringToFloat(float * f, const char * s){
	 char *end;
	float  l;
	errno = 0;
	l = strtof(s, &end);
	if ((errno == ERANGE && l == FLT_MAX) || l > FLT_MAX) {
		ZG_LOG_ERROR("\"%s\" number overflow",s);
		return false;
	}
	if ((errno == ERANGE && l == FLT_MIN) || l < FLT_MIN) {
		ZG_LOG_ERROR("\"%s\" number underflow",s);
		return false;
	}
	if (*s == '\0' || *end != '\0') {
		ZG_LOG_ERROR("\"%s\" number inconvertible",s);
		return false;
	}
	*f = l;
	return true;
}

void  ZG_String_Remove(char * str, char ch_to_remove){

	while(*str!=0){
		if(*str == ch_to_remove){ // move 1 position left...
			char *aux=str;
			while(*(aux+1)!=0){
				*aux=*(aux+1);
				aux++;
			}
			*aux=0;
		}
		str++;
	}

}

bool ZG_String_EndsWith(const char * str, const char * end_str){
	size_t len_str=strlen(str);
	size_t len_end_str=strlen(end_str);
	if(len_end_str<=len_str){
		return strcmp(str+len_str-len_end_str,end_str)==0;
	}

	return false;
}

bool			ZG_String_IsDigit(char c){

	return (('0' <= c) && (c<='9'));
}

bool			ZG_String_WIsNullOrEmpty(const wchar_t *str){
	if(str == NULL) return true;
	if(*str==0) return true;
	return false;

}

bool			ZG_String_IsNullOrEmpty(const char *str){
	if(str == NULL) return true;
	if(*str==0) return true;
	return false;

}

wchar_t         *ZG_String_ToWString(const char *str_in){
	size_t str_in_len=strlen(str_in);
	wchar_t * str_out=NULL;
	wchar_t * ptr_dst=NULL;
	wchar_t w = 0;
	char *ptr_src=NULL;
	int bytes = 0;
	wchar_t err = L'o';

	str_out=(wchar_t *)malloc(sizeof(wchar_t)*str_in_len);
	memset(str_out,0,sizeof(wchar_t)*str_in_len);

	ptr_src=(char *)str_in;
	ptr_dst=str_out;

	for (size_t i = 0; i < str_in_len; i++){
		unsigned char c = *ptr_src++;
		if (c <= 0x7f){//first byte
			if (bytes){
				*ptr_dst++=err;
				bytes = 0;
			}
			*ptr_dst++=((wchar_t)c);
		}
		else if (c <= 0xbf){//second/third/etc byte
			if (bytes){
				w = ((w << 6)|(c & 0x3f));
				bytes--;
				if (bytes == 0){
					*ptr_dst++=w;
				}
			}
			else{
				*ptr_dst++=err;
			}
		}
		else if (c <= 0xdf){//2byte sequence start
			bytes = 1;
			w = c & 0x1f;
		}
		else if (c <= 0xef){//3byte sequence start
			bytes = 2;
			w = c & 0x0f;
		}
		else if (c <= 0xf7){//3byte sequence start
			bytes = 3;
			w = c & 0x07;
		}
		else{
			*ptr_dst++=err;
			bytes = 0;
		}
	}
	if (bytes){
		*ptr_dst++=err;
	}

	return str_out;
}

unsigned long ZG_String_GetChar(void *ptr, ZG_CharType char_text){
	uint32_t res=0;
	if(char_text==ZG_CHAR_TYPE_WCHAR){
		wchar_t c=*((wchar_t *)ptr);
		res = c;
	}else{
		char c=*((char *)ptr);
		res = c;
	}

	return res;
}

void ZG_String_Advance(void **ptr, ZG_CharType char_text){
	size_t len = sizeof(char);
	if(char_text==ZG_CHAR_TYPE_WCHAR){
		len=sizeof(wchar_t);
	}else{
		len=sizeof(char);
	}

	(*(uint8_t **)ptr)+=len;

}

unsigned long ZG_String_GetCharAndAdvance(void **ptr, ZG_CharType char_text){
	size_t len = sizeof(char);
	uint32_t res=0;
	if(char_text==ZG_CHAR_TYPE_WCHAR){
		wchar_t c=**((wchar_t **)ptr);
		len=sizeof(wchar_t);
		res = c;
	}else{
		char c=**((char **)ptr);
		len=sizeof(char);
		res = c;
	}


	if(res==0) return 0;

	(*(uint8_t **)ptr)+=len;

	return res;
}
