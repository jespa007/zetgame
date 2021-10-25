#include "zg_base.h"

static PackedFileSystem *g_packed_file_system=NULL;

//-----------------------------------------------------------------------------------------------v
// PFGS memory ops.
static bool FileSystem_FileExistsPFS(const char * filename) {

	PackedFile fp;

	if(g_packed_file_system==NULL){
		Log_Error("Error no packed file loaded!");
		return false;
	}

	if(PackedFileSystem_Open(g_packed_file_system,filename,&fp)){
		return true;
	}

	return false;

}
//------------------------------------------------------------------------------------------------
static BufferByte * FileSystem_ReadFilePFS(const char * filename){
	PackedFile fp;

	if(g_packed_file_system==NULL){
		Log_Error("Error no packed file loaded!");
		return NULL;
	}

	if(PackedFileSystem_Open(g_packed_file_system,filename,&fp)){

		BufferByte *buffer = BufferByte_New(fp.size+1);
		PackedFileSystem_Read(g_packed_file_system,fp, buffer->ptr, fp.size);
		return buffer;
	}
	else{
		Log_Error("File \"%s\" not exist",filename);
	}
	return NULL;
}

static int  FileSystem_FileLengthPFS(const char * filename){
	PackedFile fp;

	if(g_packed_file_system==NULL){
		Log_Error("Error no packed file loaded!");
		return -1;
	}


	if(PackedFileSystem_Open(g_packed_file_system,filename,&fp)){
		return fp.size;
	}else{
		Log_Error("File not exist %s",filename);
	}
	return -1;
}
//-----------------------------------------------------------------------------------------------v
// Normal file ops.
bool FileSystem_InitFromFile(const char * filename) {
	if(g_packed_file_system != NULL){
		Log_Error("Filesystem already initialized");
	}

	g_packed_file_system=PackedFileSystem_New();
	return PackedFileSystem_LoadFromFile(g_packed_file_system,filename,0);
}

bool FileSystem_InitFromMemory(BufferByte *buffer){
	if(g_packed_file_system != NULL){
		Log_Error("Filesystem already initialized");
	}

	g_packed_file_system=PackedFileSystem_New();
	return PackedFileSystem_LoadFromMemory(g_packed_file_system,buffer->ptr,buffer->len);
}

time_t FileSystem_GetModificationTime(const  char  * filename){

	if(g_packed_file_system!=NULL){ // not available...
		return 0;
	}

	struct stat attrib;			// create a file attribute structure

	if(stat(filename, &attrib)!=-1){		// get the attributes of afile.txt
		return mktime(localtime(&(attrib.st_mtime)));	// Get the last modified time and put it into the time structure
	}

	return 0; // failed!
}

bool FileSystem_FileExists(const char * filename) {

	if(g_packed_file_system!=NULL){
		return FileSystem_FileExistsPFS(filename);
	}

	return File_Exists(filename);
}

BufferByte * FileSystem_ReadFile(const char * filename){

	if(g_packed_file_system!=NULL){
		return FileSystem_ReadFilePFS(filename);
	}

	return File_Read(filename);
}

int  FileSystem_GetLength(const char * filename)
{
	if(g_packed_file_system!=NULL){
		return FileSystem_FileLengthPFS(filename);
	}

	return File_Length(filename);
}

ZPList *  FileSystem_ListFiles(const char * folder, const char * filter, bool recursive){

	if(g_packed_file_system!=NULL){
		return PackedFileSystem_ListFiles(g_packed_file_system,folder, filter,recursive);
	}

	return ZPIO_ListFiles(folder,filter, recursive);
}


void FileSystem_DeInit(void) {
	if(g_packed_file_system != NULL){
		PackedFileSystem_Delete(g_packed_file_system);
	}

}
