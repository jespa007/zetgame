#include "zg_logger.h"

typedef struct{
	Logger *logger;
	List 	*columns;

}PartialLoggerData;

PartialLogger * PartialLogger_New(void){
	PartialLogger * pl=NEW(PartialLogger);
	PartialLoggerData * data=NEW(PartialLoggerData);
	pl->data=data;

	data->columns = List_New();

	for(unsigned i=0; i < N_PARTIAL_LOGGER_BLOCKS; i++){
		pl->out[i]=List_New();
	}

	return pl;
}

Logger *PartialLogger_GetLogger(PartialLogger *_this){
	PartialLoggerData *data=_this->data;
	return	data->logger;
}

void 	 PartialLogger_SetLogger(PartialLogger *_this,Logger *logger){
	PartialLoggerData *data=_this->data;
	data->logger=logger;
}

void PartialLogger_AddColumn(PartialLogger *_this,const char *column_name){
	if(_this == NULL) return;
	PartialLoggerData *data=_this->data;
	char *str=NULL;

	size_t len=strlen(column_name);

	if(len == 0){
		Log_Error("Error column_name");
		return;
	}

	str=malloc(len);

	strcpy(str,column_name);
	List_Add(data->columns,str);
}

List * PartialLogger_GetColumns(PartialLogger *_this){
	PartialLoggerData *data=_this->data;
	return data->columns;
}

void PartialLogger_ClearColumns(PartialLogger *_this){
	PartialLoggerData *data=_this->data;
	List_ClearAndFreeAllItems(data->columns);
}

void PartialLogger_SetValues(PartialLogger *_this, List  * lst_values){

	PartialLoggerData *data=NULL;

	if(_this == NULL) return;

	data=_this->data;


	if(lst_values == NULL) {
		Log_Error("lst values NULL");
		return;
	}

	if(lst_values->count != data->columns->count) {
		Log_Error("lst values not match same number partial column names");
		return;
	}

	if(((_this->n_write+1)& MASK_N_PARTIAL_LOGGER_BLOCKS) != _this->n_read){

		_this->out[_this->n_write]=lst_values;//.push_back(str);

		_this->n_write=(_this->n_write+1)& MASK_N_PARTIAL_LOGGER_BLOCKS;
	}
}

void PartialLogger_Delete(PartialLogger *_this){
	PartialLoggerData *data = _this->data;
	if(_this != NULL) return;

	List_DeleteAndFreeAllItems(data->columns);

	FREE(data);
	FREE(_this);

}
