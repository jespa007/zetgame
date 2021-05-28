#ifndef __FILE_WRITTER_H__
#define __FILE_WRITTER_H__

typedef struct FileWritter FileWritter;

struct FileWritter{
	void *data;
};


FileWritter	* FileWritter_New(const char * filename);
const char * FileWritter_GetFilename(FileWritter *_this);
void FileWritter_Start(FileWritter *_this);
void FileWritter_Stop(FileWritter *_this);
bool FileWritter_Write(FileWritter *_this, uint8_t * ptr, size_t len);
void FileWritter_Reset(FileWritter *_this);
void FileWritter_Delete(FileWritter *_this);


#endif
