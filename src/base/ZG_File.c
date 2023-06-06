#include "_zg_base_.h"

ZG_BufferByte * File_Read(const char *filename){
	long int  file_length=0;
	size_t  readed_elements=0;
	ZG_BufferByte *buffer=NULL;
	FILE  *fp;


	if((fp  =  fopen(filename,"rb"))  !=  NULL)
	{
		if((file_length = ZG_File_Length(filename)) != -1) {

			buffer=ZG_BufferByte_New(file_length+1); // we reserve +1 for end string just in case is a text (it always works!)
			readed_elements = fread(buffer->ptr, 1, file_length, fp);

			if((long int)readed_elements != file_length) {
				ZG_Log_Warning("number elements doesn't match with length file (%s)",filename);
				ZG_BufferByte_Delete(buffer);
				return NULL;
			}
			fclose(fp);

			return buffer;
		}
		else  ZG_Log_Error("I can't read file \"%s\"",filename);
	}
	else  ZG_Log_Error("I can't open file \"%s\" for reading",filename);


	return NULL;
}

bool	File_Write(const char *filename, ZG_BufferByte * buf){

	size_t wrote_elements=0;
	FILE  *fp;
	bool ok = false;

	if(buf->ptr==NULL || buf->len==0){
		return false;
	}


	if((fp  =  fopen(filename,"w"))  !=  NULL)
	{

		wrote_elements = fwrite(buf->ptr, 1, buf->len, fp);

		if(wrote_elements == buf->len) {
			ok=true;
		}else{
			ZG_Log_Warning("number elements wrote doesn't match with length file (%s)",filename);
		}

		fclose(fp);
	}
	else  {
		ZG_Log_Error("I can't open file \"%s\" for writting",filename);
	}


	return ok;
}

long int  File_Length(const char * filename)
{

	long int length=-1;
	FILE  *fp=NULL;

	if((fp  =  fopen(filename,"rb"))  !=  NULL)
	{

		long int  ini,  end;

		fseek(fp,  0,SEEK_SET);
		ini  =  ftell(fp);
		fseek(fp,  0,SEEK_END);
		end  =  ftell(fp);

		length= (end  -  ini);
		fclose(fp);
	}

	return    length;
}


bool File_Exists(const char *filename) {

#ifdef _WIN32
	FILE *fp = fopen(filename,"r");

	if(fp == NULL) return false;

	fclose(fp);

	return true;
#else

	if(Directory_Exists(filename)) return false;


	struct stat buffer;
	  return (stat (filename, &buffer) == 0);
#endif


}


