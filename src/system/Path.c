#include "zg_system.h"


const char *  Path_GetTempPath(void){
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

const char *  Path_GetDirectoryName(const char * _absolute_filename) {
	static char ss[MAX_PATH]={0};
	char * found=strrchr(_absolute_filename,'\\'); // locate the last ocurrence of '\\'
	if(found==NULL){
		found=strrchr(_absolute_filename,'/'); // locate the last ocurrence of '/'
	}

	if(found != NULL){
		size_t len = found-_absolute_filename;
		if(len < MAX_PATH && len > 0){
			memcpy(ss,_absolute_filename,len);
		}
		else{
			Log_Error("Reached MAX_ABSOLUTE_FILENAME_LENGTH or 0 ('%s':%i)",_absolute_filename,len);
		}
	}
	else{
		Log_Error("Cannot get folder name on '%s'",_absolute_filename);
	}
	return ss;
}

const char *  Path_GetFilename(const char * _absolute_filename) {

	static char ss[MAX_PATH]={0};
	//size_t len=strlen(_absolute_filename);
	char * found=strrchr(_absolute_filename,'\\'); // locate the last ocurrence of '\\'

	if(found==NULL){
		found=strrchr(_absolute_filename,'/'); // locate the last ocurrence of '/'
	}
	if(found == NULL){ // no folder
		strcpy(ss,_absolute_filename);
	}

	if(found!=NULL){
		strcpy(ss,found+1);
	}else{
		Log_Error("cannot get filename from '%s'",_absolute_filename);
	}

	return ss;
}

const char *  Path_GetFilenameWithoutExtension(const char * _absolute_filename) {
	static char ss[MAX_PATH]={0};
	char *last_dot_found = NULL;
	const char *filename_aux = Path_GetFilename(_absolute_filename);
	size_t len;

	if(filename_aux == NULL) return NULL;

	last_dot_found=strrchr(filename_aux,'.');

	if(last_dot_found==NULL){
		last_dot_found=(char *)filename_aux+strlen(filename_aux);
	}

	len =last_dot_found-filename_aux;
	if(len<MAX_PATH && len > 0){
		memcpy(ss,filename_aux,len);
	}else{
		Log_Error("Reached MAX_PATH or 0 ('%s':%i)",filename_aux,len);
	}
	return ss;
}



