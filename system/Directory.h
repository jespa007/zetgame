#ifndef __DIRECTORY_H__
#define __DIRECTORY_H__

bool 			Directory_Exists(const char * path);
bool 			Directory_Create(const char * path);
List 	*		Directory_ListFiles(const char * folder, const char * filter, bool recursive);


#endif
