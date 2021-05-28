#ifndef __PARTIAL_LOGGER_H__
#define __PARTIAL_LOGGER_H__

#define N_PARTIAL_LOGGER_BLOCKS				(16)
#define MASK_N_PARTIAL_LOGGER_BLOCKS		(16-1)


typedef struct Logger Logger;
typedef struct PartialLogger PartialLogger;

struct PartialLogger{

	List *out[N_PARTIAL_LOGGER_BLOCKS]; // a list of string values ...
	unsigned char n_write, n_read;

	void  *data;
};


PartialLogger * PartialLogger_New(void);
Logger *PartialLogger_GetLogger(PartialLogger *_this);
void PartialLogger_SetLogger(PartialLogger *_this, Logger *logger);

void PartialLogger_AddColumn(PartialLogger *_this, const char *column_name);
void PartialLogger_ClearVars(PartialLogger *_this);

List * PartialLogger_GetColumns(PartialLogger *_this);


/**
 * lst values is a string array of values to be write in the file. The value is freed by partial logger when is done.
 */
void PartialLogger_SetValues(PartialLogger *_this,List  * lst_values);

void PartialLogger_Delete(PartialLogger *_this);

#endif
