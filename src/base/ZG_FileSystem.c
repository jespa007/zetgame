#include "@zg_base.h"

static ZP_PackedFileSystem *g_packed_file_system=NULL;

//-----------------------------------------------------------------------------------------------v
// PFGS memory ops.
static bool ZG_FileSystem_FileExistsPFS(const char * filename) {

	PackedFile fp;

	if(g_packed_file_system==NULL){
		ZG_LOG_ERRORF("Error no packed file loaded!");
		return false;
	}

	if(ZP_PackedFileSystem_Open(g_packed_file_system,filename,&fp)){
		return true;
	}

	return false;

}
//------------------------------------------------------------------------------------------------
static ZG_BufferByte * ZG_FileSystem_ReadFilePFS(const char * filename){
	PackedFile fp;

	if(g_packed_file_system==NULL){
		ZG_LOG_ERRORF("Error no packed file loaded!");
		return NULL;
	}

	if(ZP_PackedFileSystem_Open(g_packed_file_system,filename,&fp)){

		ZG_BufferByte *buffer = ZG_BufferByte_New(fp.size+1);
		ZP_PackedFileSystem_Read(g_packed_file_system,fp, buffer->ptr, fp.size);
		return buffer;
	}
	else{
		ZG_LOG_ERROR("File \"%s\" not exist",filename);
	}
	return NULL;
}

static int  ZG_FileSystem_FileLengthPFS(const char * filename){
	PackedFile fp;

	if(g_packed_file_system==NULL){
		ZG_LOG_ERRORF("Error no packed file loaded!");
		return -1;
	}


	if(ZP_PackedFileSystem_Open(g_packed_file_system,filename,&fp)){
		return fp.size;
	}else{
		ZG_LOG_ERROR("File not exist %s",filename);
	}
	return -1;
}
//-----------------------------------------------------------------------------------------------v
// Normal file ops.
bool ZG_FileSystem_InitFromFile(const char * filename) {
	if(g_packed_file_system != NULL){
		ZG_LOG_ERRORF("Filesystem already initialized");
	}

	g_packed_file_system=ZP_PackedFileSystem_New();
	return ZP_PackedFileSystem_LoadFromFile(g_packed_file_system,filename,0);
}

bool ZG_FileSystem_InitFromMemory(ZG_BufferByte *buffer){
	if(g_packed_file_system != NULL){
		ZG_LOG_ERRORF("Filesystem already initialized");
	}

	g_packed_file_system=ZP_PackedFileSystem_New();
	return ZP_PackedFileSystem_LoadFromMemory(g_packed_file_system,buffer->ptr,buffer->len);
}

time_t ZG_FileSystem_GetModificationTime(const  char  * filename){

	if(g_packed_file_system!=NULL){ // not available...
		return 0;
	}

	struct stat attrib;			// create a file attribute structure

	if(stat(filename, &attrib)!=-1){		// get the attributes of afile.txt
		return mktime(localtime(&(attrib.st_mtime)));	// Get the last modified time and put it into the time structure
	}

	return 0; // failed!
}

bool ZG_FileSystem_FileExists(const char * filename) {

	if(g_packed_file_system!=NULL){
		return ZG_FileSystem_FileExistsPFS(filename);
	}

	return ZG_File_Exists(filename);
}

ZG_BufferByte * ZG_FileSystem_ReadFile(const char * filename){

	if(g_packed_file_system!=NULL){
		return ZG_FileSystem_ReadFilePFS(filename);
	}

	return ZG_File_Read(filename);
}

int  ZG_FileSystem_GetLength(const char * filename)
{
	if(g_packed_file_system!=NULL){
		return ZG_FileSystem_FileLengthPFS(filename);
	}

	return ZG_File_Length(filename);
}

ZP_List *  ZG_FileSystem_ListFiles(const char * folder, const char * filter, bool recursive){

	if(g_packed_file_system!=NULL){
		return ZP_PackedFileSystem_ListFiles(g_packed_file_system,folder, filter,recursive);
	}

	return ZP_IO_ListFiles(folder,filter, recursive);
}


void ZG_FileSystem_DeInit(void) {
	if(g_packed_file_system != NULL){
		ZP_PackedFileSystem_Delete(g_packed_file_system);
	}

}
