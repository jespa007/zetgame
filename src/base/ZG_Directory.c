#include "_zg_base_.h"

bool ZG_Directory_Exists(const char * path){
	int status;
	struct stat st_buf;
	//int ret_stat;

	status = stat (path, &st_buf);
	if (status != 0) {
		//Log_Error ("Error, errno = %d\n", errno);
		return false;
	}

	return S_ISDIR (st_buf.st_mode) != 0;


}

ZG_List * ZG_Directory_ListFilesBuiltIn(const char * folder, ZG_List *list_attribs, bool recursive){
	ZG_List *list_file=ZG_List_New();
	bool ok=false;
	DIR *dir;
	struct dirent *ent;
	if ((dir = opendir (folder)) != NULL) {
	  while ((ent = readdir (dir)) != NULL) {

		  if((strcmp(ent->d_name,".")!=0) && (strcmp(ent->d_name,"..")!=0)){ // invalid dirs...

				  char data[MAX_PATH]={0};
				  sprintf(data,"%s/%s",folder,ent->d_name);
				  if(ZG_Directory_Exists(data)){

					  if(recursive){
						  ZG_List * r = ZG_Directory_ListFilesBuiltIn(data,list_attribs,true);
						  // add all resulting item elements to current list...
						  ZG_List_Concat(list_file,r);

						  // r is not used any more, so delete it...
						  ZG_List_Delete(r);
					  }
				  }
				  else{
					  ok=false;

					  for(unsigned i = 0; i < list_attribs->count && !ok; i++){

						  if((strcmp((char *)list_attribs->items[i],"*")==0) || ZG_String_EndsWith(ent->d_name,(char *)list_attribs->items[i])) {
							  char *filename=malloc(strlen(data)+1); // 1 end string
							  memset(filename,0,strlen(data)+1);
							  strcpy(filename,data);
							  ZG_List_Add(list_file,filename);
							  ok=true;
						  }
					  }
				  }
			  }
	  }
	  closedir (dir);
	} else {
		fprintf(stderr,"could not open directory %s\n",folder);
	}

	return list_file;
}

ZG_List * ZG_Directory_ListFiles(const char * folder, const char * filter, bool recursive){
	ZG_List * list_file=NULL;
	ZG_List * list_attribs = ZG_String_SplitChar(filter==NULL?"*":filter, '|');

	for(unsigned i = 0; i < list_attribs->count; i++){
		ZG_String_Remove((char *)list_attribs->items[i],' ');
		if(strcmp((char *)list_attribs->items[i],"*")!=0){
			ZG_String_Remove((char *)list_attribs->items[i],'*');
		}
	}

	list_file=ZG_Directory_ListFilesBuiltIn(folder,list_attribs,recursive);

	ZG_List_DeleteAndFreeAllItems(list_attribs);

	return list_file;

}

int ZG_Directory_CreateDirectoryRecursive(const char *dir) {
        char tmp[1024];
        char *p = NULL;
        size_t len;
        int status;


        snprintf(tmp, sizeof(tmp),"%s",dir);
        len = strlen(tmp);
        if(tmp[len - 1] == '/')
                tmp[len - 1] = 0;
        for(p = tmp + 1; *p; p++)
                if(*p == '/') {
                        *p = 0;
                        if(!ZG_Directory_Exists(tmp)) {// dir not exist, create -->
                        	status=mkdir(tmp
#ifdef __unix__
                        			, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH
#endif
									);
                        	if(status!=0)return status;
                        }
                        *p = '/';
                }

        if(!ZG_Directory_Exists(tmp)) {// dir not exist, create -->
        	status=mkdir(tmp
#ifdef __unix__
        			, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH
#endif
			);
        	if(status!=0)return status;
        }

        return 0;
}



bool ZG_Directory_Create(const char * dir){

	if(ZG_Directory_Exists(dir)){ // already exists
		return true;
	}

	if(ZG_String_IsNullOrEmpty(dir)){
		ZG_Log_ErrorF("empty entry");
		return false;
	}

	//if(!directoryExists(filename)){

		int status;

		status = ZG_Directory_CreateDirectoryRecursive(dir);//, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);

		if(status == 0) return true;

		perror("stat");
		switch(status){
		case EACCES:
			ZG_Log_ErrorF("Search permission is denied on a component of the path prefix, or write permission is denied on the parent directory of the directory to be created.");
			break;
		case EEXIST:
			ZG_Log_ErrorF("The named file exists.");
			break;
		case ELOOP:
			ZG_Log_ErrorF("A loop exists in symbolic links encountered during resolution of the path argument.");
			break;
		case EMLINK:
			ZG_Log_ErrorF("The link count of the parent directory would exceed {LINK_MAX}.");
			break;
		case ENAMETOOLONG:
			ZG_Log_ErrorF("The length of the path argument exceeds {PATH_MAX} or a pathname component is longer than {NAME_MAX}.");
			break;
		case ENOENT:
			ZG_Log_ErrorF("A component of the path prefix specified by path does not name an existing directory or path is an empty std::string.");
			break;
		case ENOSPC:
			ZG_Log_ErrorF("The file system does not contain enough space to hold the contents of the new directory or to extend the parent directory of the new directory.");
			break;
		case ENOTDIR:
			ZG_Log_ErrorF("A component of the path prefix is not a directory.");
			break;
		case EROFS:
			ZG_Log_ErrorF("The parent directory resides on a read-only file system.");
			break;
		}

	return false;


}

