#ifndef __ZG_PATH_H__
#define __ZG_PATH_H__

#define ZG_PATH_MAX 4096


/**
 * It returns the temp path
 */
const char *  ZG_Path_GetTempPath(void);
/**
 * Given absolute path give it return its directory. The resulting string should be deallocated when is not used anymore
 */
char *  ZG_Path_GetDirectoryName(const char * _absolute_filename);
/**
 * Given absolute path give it return its filename. The resulting string should be deallocated when is not used anymore
 */
char *  ZG_Path_GetFilename(const char * _absolute_filename);

/**
 * Given absolute path give it return its filename without extension. The resulting string should be deallocated when is not used anymore
 */
char *  ZG_Path_GetFilenameWithoutExtension(const char * _absolute_filename);


#endif
