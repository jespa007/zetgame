#ifndef __FILESYSTEM__
#define __FILESYSTEM__


bool FileSystem_InitFromFile(const char * file);
bool FileSystem_InitFromMemory(BufferByte *buffer);
bool FileSystem_DirectoryExists(const char * dirname);
bool FileSystem_FileExists(const char * filename);
BufferByte * FileSystem_ReadFile(const char * filename);
time_t FileSystem_GetModificationTime(const char * filename);
int  FileSystem_GetLength(const char * filename);
List *FileSystem_GetFiles(const char * folder, const char * filter);
void FileSystem_DeInit(void);

#endif
