#ifndef __PATH_H__
#define __PATH_H__

/**
 * It returns the temp path
 */
const char *  Path_GetTempPath(void);
/**
 * Given absolute path give it return its directory. The resulting string should be deallocated when is not used anymore
 */
char *  Path_GetDirectoryName(const char * _absolute_filename);
/**
 * Given absolute path give it return its filename. The resulting string should be deallocated when is not used anymore
 */
char *  Path_GetFilename(const char * _absolute_filename);

/**
 * Given absolute path give it return its filename without extension. The resulting string should be deallocated when is not used anymore
 */
char *  Path_GetFilenameWithoutExtension(const char * _absolute_filename);


#endif
