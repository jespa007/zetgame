#ifndef __ZG_DIRECTORY_H__
#define __ZG_DIRECTORY_H__

bool 			ZG_Directory_Exists(const char * path);
bool 			ZG_Directory_Create(const char * path);
ZG_List 	*		ZG_Directory_ListFiles(const char * folder, const char * filter, bool recursive);


#endif
