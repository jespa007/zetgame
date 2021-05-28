#include "zg_logger.h"

#define MAX_BUFFER_LAST_OUTPUT 4096

typedef struct{
	char 			last_output[MAX_BUFFER_LAST_OUTPUT];
	PartialLogger  *logger_partial;
}PartialLoggerInfo;

typedef struct{
	FileWritter * file_writter;
	char delimiter;
	List * partial_loggers_info;
	char current_output[1024];
	char user_info_header[MAX_PATH];
	char custom_filename[MAX_PATH];
	time_t created_time;
	char log_name[1024];
	bool header_written;
	pthread_t thread;
	bool end_update;

}LoggerData;


char g_log_path[MAX_PATH]=".";
char g_filename_no_set[1024]="";
char g_extra_info_log_filename[256]="L";


void Logger_SetLogPath(const char * _path){

	if(!Directory_Exists(_path)){
		if(!Directory_Create(_path)){
			return;
		}
	}

	if(Directory_Exists(_path)){

		strcpy(g_log_path, _path);
		Log_Info("* Set log path at %s",g_log_path);
	}else{
		Log_Error("Cannot set log path at %s because is not directory",_path);
	}
}

void  Logger_SetExtraInfoFilename(const char  * _custom_info_filename){

	strcpy(g_extra_info_log_filename, _custom_info_filename);
	Log_Info("* Set custom log info filename as %s",g_extra_info_log_filename);
}

const char * Logger_GetLogPath(void){
	return g_log_path;
}

const char * Logger_GetExtraInfoFilename(void){
	return g_extra_info_log_filename;
}

void * Logger_UpdateThread(void *_data) {

	LoggerData *data = _data;

	while(!data->end_update){

		char aux[MAX_BUFFER_LAST_OUTPUT*4]={0};
		char str_delimiter[2]={data->delimiter,0};
		bool updated=false;


		int n_bytes_write=0;
		bool max_size = false;


		for(unsigned i = 0; i < data->partial_loggers_info->count && !max_size; i++){
			PartialLoggerInfo *pl_info=data->partial_loggers_info->items[i];
			PartialLogger *pl = pl_info->logger_partial;//m_list[i].m_plogger;
			if(((pl->n_read) & MASK_N_PARTIAL_LOGGER_BLOCKS) != pl->n_write){

				//if(((pl->n_read+1) & MASK_N_PARTIAL_LOGGER_BLOCKS) != pl->n_write){
					List *out = pl->out[pl->n_read];

					updated=true;
					//if(i>0)
					//	strcat(aux,str_del);
					memset(pl_info->last_output, 0, sizeof(pl_info->last_output));

					// set block to update...

					for(unsigned j = 0; j < out->count && !max_size; j++){
						if((n_bytes_write+out->count)< MAX_BUFFER_LAST_OUTPUT){
							if(j>0){
								strcat(pl_info->last_output,str_delimiter);
							}
							strcat(pl_info->last_output,out->items[j]);
						}
						else{ // max reached size...
							max_size = true;
							fprintf(stderr,"\nMax size reached!");

						}
					}

					if(!max_size){

						if(i>0){
							PartialLoggerInfo *pl_info_previous=data->partial_loggers_info->items[i-1];
							if(pl_info_previous->last_output[0]==0){ // check that csv is well constructed ...
								updated=false;
								break;
							}
							strcat(aux,str_delimiter);
						}
						strcat(aux, pl_info->last_output);
					}
					pl->n_read = (pl->n_read+1)& MASK_N_PARTIAL_LOGGER_BLOCKS;
				//}
			}

			else{
				if(i>0){
					PartialLoggerInfo *pl_info_previous=data->partial_loggers_info->items[i-1];
					if(pl_info_previous->last_output[0]==0){ // check that csv is well constructed ...
						updated=false;
						break;
					}
					strcat(aux,str_delimiter);
				}
				strcat(aux, pl_info->last_output);

			}
		}

		if(updated) {

			//printf("\nu %s ",aux);

			if(!data->header_written){ // time to write the header...
				char m_cp_header[4096]={0};
				char str_header_column_info[2048]={0};
				/*char m_eyetracker_name[1024]={"Unknow tracker"};
				if(CEyeTracker::getCurrentTracker()){
					strcpy(m_eyetracker_name,CEyeTracker::getCurrentTracker()->getName().c_str());
				}*/

				for(unsigned i = 0; i < data->partial_loggers_info->count; i++){
					PartialLoggerInfo *pl_info = data->partial_loggers_info->items[i];
					List *columns = PartialLogger_GetColumns(pl_info->logger_partial);

					if(i > 0){
						strcat(str_header_column_info,str_delimiter);
					}

					for(unsigned j = 0; j  < columns->count; j++){
						if(j > 0){
							strcat(str_header_column_info,str_delimiter);
						}
						strcat(str_header_column_info,columns->items[j]);
					}
				}

				sprintf(m_cp_header,
									"%s\n"
									"---\n"
									//"\nTracker: %s"
									"%s\n"
									,

									data->user_info_header,
										//m_eyetracker_name,
									str_header_column_info);
				// write columns ...

				//strcat(m_cp_header,"\n");
				FileWritter_Write(data->file_writter,(uint8_t *)m_cp_header,strlen(m_cp_header));
				data->header_written = true;
			}

			strcat(aux,"\n");
			FileWritter_Write(data->file_writter,(uint8_t *)aux,strlen(aux));
		}
	}

	return NULL;
}

const char * Logger_GetFilename(Logger *_this){
	LoggerData *data=_this->data;


	if(data->file_writter!=NULL){
		return FileWritter_GetFilename(data->file_writter);
	}else{
		Log_Error("filename is NOT created!, please do start");
	}

	return g_filename_no_set;
}


Logger * Logger_New(void){
	Logger *logger=NEW(Logger);
	LoggerData *data=NEW(LoggerData);
	logger->data=data;

	data->delimiter=';';
	sprintf(data->log_name,"log");
	return logger;
}

void Logger_SetLogName(Logger *_this,const char * _log_name){
	LoggerData *data=_this->data;

	strcpy(data->log_name, _log_name);
}


void Logger_Start(Logger *_this){
	LoggerData *data=_this->data;

	if(data->thread != 0){

		data->end_update=false;

		if(data->file_writter==NULL){
			char buf[1024];
			data->created_time =time(NULL );
			struct tm *curtime = localtime ( &data->created_time );

			char c_diff='/';
	#ifdef _WIN32
			c_diff='\\';
	#endif

			sprintf(buf,"%s%c%s_%s_%4i_%02i_%02i_%02i_%02i.txt"
					,g_log_path
					,c_diff
					,data->log_name
					,g_extra_info_log_filename
					,curtime->tm_year+1900,curtime->tm_mon+1
					,curtime->tm_mday,curtime->tm_hour
					,curtime->tm_min);


			data->file_writter=FileWritter_New(buf);
		}

		pthread_create(&data->thread,NULL,Logger_UpdateThread,data);

		FileWritter_Start(data->file_writter);


		Log_Info("log \"%s\" started",data->log_name);

	}
	else{
		Log_Error("logger already started");
	}
}

void Logger_Stop(Logger *_this)
{
	LoggerData *data=_this->data;

	if(data->thread == 0){

		// disable partial loggers....
		data->end_update=true;
		pthread_join(data->thread,NULL);

		if(data->file_writter!=NULL){
			FileWritter_Stop(data->file_writter); //stops reader...
		}
		data->thread=0;
		Log_Info("log  stopped");
	}else{
		Log_Error("Log not started");
	}
}

void Logger_SetUserInfoHeader(Logger *_this,const char  *header_str){

	LoggerData *data=_this->data;
	//string aux = StrUtils::Replace(header,'\n','_');

	//strcpy(m_userInfoHeader, aux.c_str());
	strcpy(data->user_info_header, header_str);

}

void Logger_ResetLogFile(Logger *_this){
	LoggerData *data=_this->data;
	if(data->file_writter!=NULL){
		Logger_Stop(_this);
		FileWritter_Reset(data->file_writter);
		data->header_written=false;
	}
}

void Logger_SetDelimiter(Logger *_this,char delimiter){
	LoggerData *data=_this->data;
	data->delimiter = delimiter;
}


bool Logger_AddPartialLogger(Logger *_this,PartialLogger *logger_partial){
	LoggerData *data=_this->data;
	if(logger_partial == NULL) {
		return false;
	}

	if(PartialLogger_GetLogger(logger_partial) != NULL) {
		Log_Error("partial loggoer it has already parented!");
		return false;
	}

	PartialLoggerInfo *info=NEW(PartialLoggerInfo);
	info->logger_partial = logger_partial;

	List_Add(data->partial_loggers_info, info);

	// set logger that manages partial loggers...
	PartialLogger_SetLogger(logger_partial,_this);

	return true;
}



void Logger_Delete(Logger *_this){
	if(_this ==NULL) return;

	LoggerData *data=_this->data;

	Logger_Stop(_this);

	FileWritter_Delete(data->file_writter);

	for(int i=0; i < data->partial_loggers_info->count; i++){
		PartialLoggerInfo *pli=data->partial_loggers_info->items[i];
		PartialLogger_Delete(pli->logger_partial);
	}

	List_DeleteAndFreeAllItems(data->partial_loggers_info);

	FREE(_this->data);
	FREE(_this);


}
