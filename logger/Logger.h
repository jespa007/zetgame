#ifndef __LOGGER_H__
#define __LOGGER_H__

typedef struct PartialLogger PartialLogger;
typedef struct Logger Logger;

struct Logger{
	void *data;
};




// STATIC
void 		 Logger_SetLogPath(const char *p);
const char * Logger_GetLogPath(void);
const char * Logger_GetExtraInfoFilename(void);
void Logger_SetPathWrite(const char * _path);

// PUBLIC
Logger * 	Logger_New(void);
bool 		Logger_AddPartialLogger(Logger *_this,PartialLogger *pl);
void 		Logger_SetDelimiter(Logger *_this,char delimiter);
void 		Logger_Delete(Logger *_this);

#endif
