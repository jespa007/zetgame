#ifndef __LOG_H__
#define __LOG_H__
	


typedef enum{
	LOG_LEVEL_ERROR=0,
	LOG_LEVEL_WARNING,
	LOG_LEVEL_INFO,
	LOG_LEVEL_DEBUG
} LogLevel;


#define Log_Error(s, ...) 			Log_Print(__FILE__,__LINE__,LOG_LEVEL_ERROR	, true,s, ##__VA_ARGS__)
#define Log_Warning(s, ...)   		Log_Print(__FILE__,__LINE__,LOG_LEVEL_WARNING	, true,s, ##__VA_ARGS__)
#define Log_Info(s, ...)   			Log_Print(__FILE__,__LINE__,LOG_LEVEL_INFO	, true,s, ##__VA_ARGS__)
#ifdef __DEBUG__
#define Log_Debug(s, ...)   		Log_Print(__FILE__,__LINE__,LOG_LEVEL_DEBUG, true,s, ##__VA_ARGS__)
#else
#define Log_Debug(s, ...)
#endif




void  Log_SetAnsiEscape(bool _use);
void  Log_Print(const  char  *file,  int  line, LogLevel level, bool with_cr, const  char  *string_text,...);




#endif
	
