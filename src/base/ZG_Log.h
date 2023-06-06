#ifndef __ZG_LOG_H__
#define __ZG_LOG_H__
	


typedef enum{
	ZG_LOG_LEVEL_ERROR=0,
	ZG_LOG_LEVEL_WARNING,
	ZG_LOG_LEVEL_INFO,
	ZG_LOG_LEVEL_DEBUG
} ZG_LogLevel;


#define ZG_Log_Error(s, ...) 			ZG_Log_Print(__FILE__,__LINE__,ZG_LOG_LEVEL_ERROR		, s, ##__VA_ARGS__)
#define ZG_Log_ErrorF(s) 				ZG_Log_Error(s, NULL)

#define ZG_Log_Warning(s, ...)   		ZG_Log_Print(__FILE__,__LINE__,ZG_LOG_LEVEL_WARNING	, s, ##__VA_ARGS__)
#define ZG_Log_WarningF(s)   			ZG_Log_Warning(s,NULL)

#define ZG_Log_Info(s, ...)   			ZG_Log_Print(__FILE__,__LINE__,ZG_LOG_LEVEL_INFO		, s, ##__VA_ARGS__)
#define ZG_Log_InfoF(s)   				ZG_Log_Info(s, NULL)

#ifdef __DEBUG__
#define ZG_Log_Debug(s, ...)   		ZG_Log_Print(__FILE__,__LINE__,ZG_LOG_LEVEL_DEBUG		, s, ##__VA_ARGS__)
#else
#define ZG_Log_Debug(s, ...)
#endif

#define ZG_Log_DebugF(s)   			ZG_Log_Debug(s,NULL)




void  ZG_Log_SetAnsiEscape(bool _use);
void  ZG_Log_Print(const  char  *file,  int  line, ZG_LogLevel level, const  char  *string_text,...);




#endif
	
