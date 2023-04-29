#include "scene/zg_scene.h"

typedef struct{
	TextBox *textbox;
}SGTextBox2dData;


SGTextBox2d * 	SGTextBox2d_New(void){
	SGTextBox2d *sg_textbox = ZG_NEW(SGTextBox2d);
	SGTextBox2dData * data=sg_textbox->data=ZG_NEW(SGTextBox2dData);
	sg_textbox->node=SGNode_New();
	data->textbox=TextBox_New();

	SGTextBox2d_SetDimensions(sg_textbox,100,100);

	//sg_textbox->node->sgnode_type=SGNODE_TYPE_TEXTBOX;

	return sg_textbox;

}

void 	SGTextBox2d_SetDimensions(SGTextBox2d *_this,uint16_t width, uint16_t height){

	SGTextBox2dData * data= _this->data;
	TextBox_SetDimensions(data->textbox,width,height);

}

void 		SGTextBox2d_SetText(SGTextBox2d *_this, const char * in,... ){
	SGTextBox2dData *_data=_this->data;
	char out[4096];

	ZG_VARGS(out,in);

	TextBox_SetText(_data->textbox,out);
}

void 		SGTextBox2d_SetWText(SGTextBox2d *_this, const wchar_t * in,... ){
	SGTextBox2dData *_data=_this->data;
	wchar_t out[4096];

	ZG_WVARGS(out,in);

	TextBox_WSetText(_data->textbox,out);
}


void	   	SGTextBox2d_Delete(SGTextBox2d *_this){
	SGTextBox2dData *_data=_this->data;

	TextBox_Delete(_data->textbox);

	ZG_FREE(_data);
	ZG_FREE(_this);

}
