#ifndef __ZG_LOG_H__
#define __ZG_LOG_H__
	


typedef enum{
	ZG_LOG_LEVEL_ERROR=0,
	ZG_LOG_LEVEL_WARNING,
	ZG_LOG_LEVEL_INFO,
	ZG_LOG_LEVEL_DEBUG
} ZG_LogLevel;


#define ZG_LOG_ERROR(s, ...) 			ZG_Log_Print(__FILE__,__LINE__,ZG_LOG_LEVEL_ERROR		, s, ##__VA_ARGS__)
#define ZG_LOG_ERRORF(s) 				ZG_LOG_ERROR(s, NULL)

#define ZG_LOG_WARNING(s, ...)   		ZG_Log_Print(__FILE__,__LINE__,ZG_LOG_LEVEL_WARNING	, s, ##__VA_ARGS__)
#define ZG_LOG_WARNINGF(s)   			ZG_LOG_WARNING(s,NULL)

#define ZG_LOG_INFO(s, ...)   			ZG_Log_Print(__FILE__,__LINE__,ZG_LOG_LEVEL_INFO		, s, ##__VA_ARGS__)
#define ZG_LOG_INFOF(s)   				ZG_LOG_INFO(s, NULL)

#ifdef __DEBUG__
#define ZG_LOG_DEBUG(s, ...)   		ZG_Log_Print(__FILE__,__LINE__,ZG_LOG_LEVEL_DEBUG		, s, ##__VA_ARGS__)
#else
#define ZG_LOG_DEBUG(s, ...)
#endif

#define ZG_LOG_DEBUGF(s)   			ZG_LOG_DEBUG(s,NULL)




void  ZG_Log_SetAnsiEscape(bool _use);
void  ZG_Log_Print(const  char  *file,  int  line, ZG_LogLevel level, const  char  *string_text,...);




#endif
	
