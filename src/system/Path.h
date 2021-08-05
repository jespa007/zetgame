#ifndef __PATH_H__
#define __PATH_H__


const char *  Path_GetTempPath(void);
const char *  Path_GetDirectoryName(const char * _absolute_filename);
const char *  Path_GetFilename(const char * _absolute_filename);
const char *  Path_GetFilenameWithoutExtension(const char * _absolute_filename);


#endif
