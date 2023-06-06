#ifndef __ZG_FILESYSTEM__
#define __ZG_FILESYSTEM__


bool ZG_FileSystem_InitFromFile(const char * file);
bool ZG_FileSystem_InitFromMemory(ZG_BufferByte *buffer);
bool ZG_FileSystem_DirectoryExists(const char * dirname);
bool ZG_FileSystem_FileExists(const char * filename);
ZG_BufferByte * ZG_FileSystem_ReadFile(const char * filename);
time_t ZG_FileSystem_GetModificationTime(const char * filename);
int  ZG_FileSystem_GetLength(const char * filename);
ZG_List *ZG_FileSystem_GetFiles(const char * folder, const char * filter);
void ZG_FileSystem_DeInit(void);

#endif
