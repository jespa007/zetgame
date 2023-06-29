#include "_zg_base_.h"


const char *  ZG_Path_GetTempPath(void){
	static char folder[MAX_PATH]={0};
#ifdef _WIN32
	GetTempPath(MAX_PATH,folder);
#else
	const char *env_folder = getenv("TMPDIR");
	if (env_folder == 0){
	    env_folder = "/tmp";
	}
	strcpy(folder,env_folder);
#endif
	return folder;

}

char *  ZG_Path_GetDirectoryName(const char * _absolute_filename) {
	char *dst=NULL;
	char * found=strrchr(_absolute_filename,'\\'); // locate the last ocurrence of '\\'
	if(found==NULL){
		found=strrchr(_absolute_filename,'/'); // locate the last ocurrence of '/'
	}

	if(found != NULL){
		size_t len = found-_absolute_filename+1;
		if(len > 0){
			dst=malloc(len+1);
			memset(dst,0,len+1);
			strncpy(dst,_absolute_filename,len);
		}
		else{
			ZG_LOG_ERROR("Reached MAX_ABSOLUTE_FILENAME_LENGTH or 0 ('%s':%i)",_absolute_filename,len);
		}
	}
	else{
		ZG_LOG_ERROR("Cannot get folder name on '%s'",_absolute_filename);
	}
	return dst;
}

char *  ZG_Path_GetFilename(const char * _absolute_filename) {

	char *dst=0;
	char *src=(char *)_absolute_filename;
	size_t len = strlen(_absolute_filename);
	//size_t len=strlen(_absolute_filename);
	char * found=strrchr(_absolute_filename,'\\'); // locate the last ocurrence of '\\'

	if(found==NULL){
		found=strrchr(_absolute_filename,'/'); // locate the last ocurrence of '/'
	}

	if(found != NULL){ // no folder
		src=found+1;
		len = strlen(src);
	}

	dst=malloc(len+1);
	memset(dst,0,len+1);
	strcpy(dst,src);


	return dst;
}

char *  ZG_Path_GetFilenameWithoutExtension(const char * _absolute_filename) {
	char *dst=0;
	char *last_dot_found = NULL;
	char *filename_aux = ZG_Path_GetFilename(_absolute_filename);
	size_t len;

	if(filename_aux == NULL) return NULL;

	last_dot_found=strrchr(filename_aux,'.');

	if(last_dot_found==NULL){
		last_dot_found=(char *)filename_aux+strlen(filename_aux);
	}

	len =last_dot_found-filename_aux;
	if(len > 0){
		dst=malloc(len+1);
		memset(dst,0,len+1);
		strncpy(dst,filename_aux,len);
	}else{
		ZG_LOG_ERROR("Reached MAX_PATH or 0 ('%s':%i)",filename_aux,len);
	}

	free(filename_aux);

	return dst;
}



