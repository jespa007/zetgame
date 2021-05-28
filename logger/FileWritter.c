#include "zg_logger.h"

#define	MAX_BUFFER_BLOCK_LENGTH 4096
#define	MASK_BUFFER_BLOCK_LENGTH (MAX_FW_BLOCK_LENGTH-1),
#define	NUM_BLOCKS 4
#define	MASK_NUM_BLOCKS (NUM_BLOCKS-1)

typedef struct{
	char ptr[MAX_BUFFER_BLOCK_LENGTH];
	int	 len;
}BufferBlock;


typedef struct{
	//------------------------------------------
	BufferBlock bblock[NUM_BLOCKS];


	pthread_t thread;

	int n_block_write;
	int n_block_read; // can write disk or socket (stream)...
	//------------------------------------------
	bool file_ready;
	bool end_loop_mdb;

	char filename[1024];
	FILE *fp;

}FileWritterData;


void * FileWritter_UpdateThread(void *data);

FileWritter * FileWritter_New(const char * filename){
	FileWritter *fw= NEW(FileWritter);
	FileWritterData *data= NEW(FileWritterData);
	fw->data=data;

	strcpy(data->filename, filename);

	//data->thread=NULL;
	//n_block_write = n_block_read =0;
	//memset(&block,0,sizeof(block));
	//fp = NULL;
	//have_to_open = true;
	//end_loop_mdb=false;

	return fw;


}

const char * FileWritter_GetFilename(FileWritter *_this){
	FileWritterData * data = _this->data;

	return data->filename;
}

void  FileWritter_Start(FileWritter *_this){

	if(_this == NULL) return;

	FileWritterData * data = _this->data;

	if(data->thread!=0){
		Log_Warning("Thread already started");
		return;
	}

	data->end_loop_mdb=false;
	pthread_create(&data->thread,NULL,FileWritter_UpdateThread,data);//mainLoop(this));
}

void  FileWritter_Stop(FileWritter *_this){
	if(_this == NULL) return;
	FileWritterData * data = _this->data;
	if(data->thread==0){
		Log_Warning("Thread not started");
		return;
	}


	data->end_loop_mdb=true;
	pthread_join(data->thread,NULL);


	// finish end all blocks
	while(((data->n_block_read) & MASK_NUM_BLOCKS) != data->n_block_write)
	{
		BufferBlock *block_read = &data->bblock[data->n_block_read];
		//---- ready to write :)
		int n = fwrite(block_read->ptr,1,block_read->len,data->fp);
		fflush(data->fp);
		if(n != block_read->len){ // something wrong happened!
			fprintf(stderr,"\nlast bytes ko 2!");
		}

		data->n_block_read= (data->n_block_read+1)&MASK_NUM_BLOCKS;
	}

	// and write last block...
	if(data->bblock[data->n_block_read].len > 0){
		//---- ready to write :)
		BufferBlock *block_read = &data->bblock[data->n_block_read];
		int n = fwrite(block_read->ptr,1,block_read->len,data->fp);
		fflush(data->fp);
		if(n != block_read->len){ // something wrong happened!
			fprintf(stderr,"\nlast bytes ko 3!");
		}
		//printf("\n\n\n\nwrite %i bytes (%i,%i): %s\n",n,n_block_read,n_block_write,block[n_block_read].m_buffer);
	}


	data->n_block_write = data->n_block_read =0;
	memset(&data->bblock,0,sizeof(data->bblock));

	data->thread=0;
}

void FileWritter_Reset(FileWritter *_this){
	FileWritterData *data=_this->data;
	FileWritter_Stop(_this);

	if(data->fp != NULL){

		fflush(data->fp);
		fclose(data->fp);
		data->file_ready=false;
		data->fp=NULL;
	}

}

void * FileWritter_UpdateThread(void *_data){
	FileWritterData *data=_data;
	while(!data->end_loop_mdb)
	{

		// if file not opened, it doesn't have sense to write anything..
		if(data->fp ==NULL) {
			continue;
		}

		if(((data->n_block_read) & MASK_NUM_BLOCKS) != data->n_block_write){

			if(((data->n_block_read+1) & MASK_NUM_BLOCKS) != data->n_block_write){
				//---- ready to write :)
				int n = fwrite(data->bblock[data->n_block_read].ptr,1,MAX_BUFFER_BLOCK_LENGTH,data->fp);

				if(n != MAX_BUFFER_BLOCK_LENGTH){ // something wrong happened!
					fprintf(stderr,"\nko!");
				}

				data->n_block_read= (data->n_block_read+1)&MASK_NUM_BLOCKS;
			}
		}
	}

	return NULL;
}



bool FileWritter_Write(FileWritter *_this,uint8_t * ptr_src, size_t ptr_src_len){
	if(_this == NULL) return false;
	FileWritterData * data = _this->data;
	if(!data->file_ready){
		data->file_ready=true;

		if((data->fp = fopen(data->filename, "wt+")) == NULL){
			Log_Error("Cannot create file \"%s\".",data->filename);
			return false;
		}
	}

	if(data->fp ==NULL ) return false;


	if(((data->n_block_write+1)& MASK_NUM_BLOCKS) != data->n_block_read){

		BufferBlock *write_block=&data->bblock[data->n_block_write];

		size_t to_fill=ptr_src_len;
		if(write_block->len+to_fill >= MAX_BUFFER_BLOCK_LENGTH){
			to_fill = MAX_BUFFER_BLOCK_LENGTH-write_block->len;
		}


		memcpy(&write_block->ptr[write_block->len],ptr_src, to_fill);
		write_block->len+=to_fill;

		if(write_block->len==MAX_BUFFER_BLOCK_LENGTH){ // next block

			int rest_fill = ptr_src_len - to_fill;
			data->n_block_write= (data->n_block_write+1)&MASK_NUM_BLOCKS;
			write_block=&data->bblock[data->n_block_write];

			write_block->len=0;
			if(rest_fill>0){
				write_block->len=rest_fill;
				memcpy(write_block->ptr,&ptr_src[to_fill], rest_fill);
			}
		}
		return true;
	}
	return false;
}


void FileWritter_Delete(FileWritter *_this){

	if(_this == NULL) return;

	FileWritter_Reset(_this);

	FREE(_this->data);
	FREE(_this);

}

