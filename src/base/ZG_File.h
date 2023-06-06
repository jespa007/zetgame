#ifndef __ZG_FILE_H__
#define __ZG_FILE_H__

ZG_BufferByte	*	ZG_File_Read(const char *filename);
bool				ZG_File_Write(const char *filename, ZG_BufferByte * buf);
long int			ZG_File_Length(const char *filename);
bool				ZG_File_Exists(const char *filename);

#endif
