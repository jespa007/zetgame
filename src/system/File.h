#ifndef __FILE_H__
#define __FILE_H__

BufferByte	*	File_Read(const char *filename);
bool			File_Write(const char *filename, BufferByte * buf);
long int		File_Length(const char *filename);
bool			File_Exists(const char *filename);

#endif
