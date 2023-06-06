#include "_zg_base_.h"

static ZN_PackedFileSystem *g_packed_file_system=NULL;

//-----------------------------------------------------------------------------------------------v
// PFGS memory ops.
static bool ZG_FileSystem_FileExistsPFS(const char * filename) {

	PackedFile fp;

	if(g_packed_file_system==NULL){
		ZG_Log_ErrorF("Error no packed file loaded!");
		return false;
	}

	if(PackedZG_FileSystem_Open(g_packed_file_system,filename,&fp)){
		return true;
	}

	return false;

}
//------------------------------------------------------------------------------------------------
static ZG_BufferByte * ZG_FileSystem_ReadFilePFS(const char * filename){
	PackedFile fp;

	if(g_packed_file_system==NULL){
		ZG_Log_ErrorF("Error no packed file loaded!");
		return NULL;
	}

	if(PackedZG_FileSystem_Open(g_packed_file_system,filename,&fp)){

		ZG_BufferByte *buffer = BufferByte_New(fp.size+1);
		PackedZG_FileSystem_Read(g_packed_file_system,fp, buffer->ptr, fp.size);
		return buffer;
	}
	else{
		ZG_Log_Error("File \"%s\" not exist",filename);
	}
	return NULL;
}

static int  ZG_FileSystem_FileLengthPFS(const char * filename){
	PackedFile fp;

	if(g_packed_file_system==NULL){
		ZG_Log_ErrorF("Error no packed file loaded!");
		return -1;
	}


	if(PackedZG_FileSystem_Open(g_packed_file_system,filename,&fp)){
		return fp.size;
	}else{
		ZG_Log_Error("File not exist %s",filename);
	}
	return -1;
}
//-----------------------------------------------------------------------------------------------v
// Normal file ops.
bool ZG_FileSystem_InitFromFile(const char * filename) {
	if(g_packed_file_system != NULL){
		ZG_Log_ErrorF("Filesystem already initialized");
	}

	g_packed_file_system=PackedZG_FileSystem_New();
	return PackedZG_FileSystem_LoadFromFile(g_packed_file_system,filename,0);
}

bool ZG_FileSystem_InitFromMemory(ZG_BufferByte *buffer){
	if(g_packed_file_system != NULL){
		ZG_Log_ErrorF("Filesystem already initialized");
	}

	g_packed_file_system=PackedZG_FileSystem_New();
	return PackedZG_FileSystem_LoadFromMemory(g_packed_file_system,buffer->ptr,buffer->len);
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

	return File_Length(filename);
}

ZPList *  ZG_FileSystem_ListFiles(const char * folder, const char * filter, bool recursive){

	if(g_packed_file_system!=NULL){
		return PackedZG_FileSystem_ListFiles(g_packed_file_system,folder, filter,recursive);
	}

	return ZPIO_ListFiles(folder,filter, recursive);
}


void ZG_FileSystem_DeInit(void) {
	if(g_packed_file_system != NULL){
		PackedZG_FileSystem_Delete(g_packed_file_system);
	}

}
