#ifndef  __DEFINES_H__
#define  __DEFINES_H__

// Check windows
#if _WIN32 || _WIN64
#if _WIN64
#define ENVIRONMENT64
#else
#define ENVIRONMENT32
#endif
#endif

// Check GCC
#if __GNUC__
#if __x86_64__ || __ppc64__
#define ENVIRONMENT64
#else
#define ENVIRONMENT32
#endif
#endif




#define DEFAULT_FPS 25

//---------------------------------------------------------------------------------------------------------------------------v
// TYPEDEFS
//---------------------------------------------------------------------------------------------------------------------------v

typedef  void  (*  void_function_void)(void  );
typedef  void  (*  void_function_int)(int);
typedef  int  (*  int_function_int)(int);
typedef  int  (*  int_function_void)(void);
typedef  float  (*  float_function_void)(void);
typedef  void  (*  void_function_const_char)(const char *);
typedef  const char  *(*  const_char_function_int)(int );
typedef void  (*void_function_u8)(unsigned char v);
typedef void  (*void_function_void_ptr)(void *ptr);
typedef void  * (*CallbackType)(void *result, void *user_data);
typedef void *tVoidPointer;
typedef unsigned int uint_t;


typedef struct {
	CallbackType  		ptr_function;
	void 				* user_data;
}ZG_Callback;

typedef struct{
	const char *name;
	cJSON **value;
}cJSONAttribute;


#define 	ZG_IS_POW2(x)   		(!(bool)((x) & ((x)-1)))
#define 	ZG_UNUSUED_PARAM(x) 	((void)(x))


#define ZG_VARGS(text_out, text_in)\
{va_list  ap;\
size_t max_len=sizeof(text_out)/sizeof(char);\
va_start(ap,  text_in);\
int n=vsnprintf(text_out,max_len,text_in,  ap);\
if(n==(int)max_len){\
	text_out[max_len-2]='.';\
	text_out[max_len-3]='.';\
	text_out[max_len-4]='.';\
}\
va_end(ap);}


#define ZG_WVARGS(text_out, text_in)\
{va_list  ap;\
size_t max_len=sizeof(text_out)/sizeof(wchar_t);\
va_start(ap,  text_in);\
int n=vswprintf(text_out,max_len,text_in,  ap);\
if(n==(int)max_len){\
	text_out[max_len-2]='.';\
	text_out[max_len-3]='.';\
	text_out[max_len-4]='.';\
}\
va_end(ap);}


#define ZG_ARRAY_SIZE(s) (sizeof(s)/sizeof(s[0]))
#define ZG_STRCMP(str1,op,str2) (strcmp(str1,str2)op(0))
#define ZG_ERROR -1

#endif
