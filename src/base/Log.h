#ifndef __LOG_H__
#define __LOG_H__
	


typedef enum{
	LOG_LEVEL_ERROR=0,
	LOG_LEVEL_WARNING,
	LOG_LEVEL_INFO,
	LOG_LEVEL_DEBUG
} LogLevel;


#define Log_Error(s, ...) 			Log_Print(__FILE__,__LINE__,LOG_LEVEL_ERROR		, s, ##__VA_ARGS__)
#define Log_ErrorF(s) 				Log_Error(s, NULL)

#define Log_Warning(s, ...)   		Log_Print(__FILE__,__LINE__,LOG_LEVEL_WARNING	, s, ##__VA_ARGS__)
#define Log_WarningF(s)   			Log_Warning(s,NULL)

#define Log_Info(s, ...)   			Log_Print(__FILE__,__LINE__,LOG_LEVEL_INFO		, s, ##__VA_ARGS__)
#define Log_InfoF(s)   				Log_Info(s, NULL)

#ifdef __DEBUG__
#define Log_Debug(s, ...)   		Log_Print(__FILE__,__LINE__,LOG_LEVEL_DEBUG		, s, ##__VA_ARGS__)
#else
#define Log_Debug(s, ...)
#endif

#define Log_DebugF(s)   			Log_Debug(s,NULL)




void  Log_SetAnsiEscape(bool _use);
void  Log_Print(const  char  *file,  int  line, LogLevel level, const  char  *string_text,...);




#endif
	
