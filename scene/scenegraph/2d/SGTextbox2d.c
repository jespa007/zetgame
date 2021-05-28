#include "core/scene/zg_scene.h"

typedef struct{
	Textbox *textbox;
}SGTextbox2dData;


SGTextbox2d * 	SGTextbox2d_New(void){
	SGTextbox2d *sg_textbox = NEW(SGTextbox2d);
	SGTextbox2dData * data=sg_textbox->data=NEW(SGTextbox2dData);
	sg_textbox->node=SGNode_New();
	data->textbox=Textbox_New();

	SGTextbox2d_SetDimensions(sg_textbox,100,100);

	//sg_textbox->node->sgnode_type=SGNODE_TYPE_TEXTBOX;

	return sg_textbox;

}

void 	SGTextbox2d_SetDimensions(SGTextbox2d *_this,uint16_t width, uint16_t height){

	SGTextbox2dData * data= _this->data;
	Textbox_SetDimensions(data->textbox,width,height);

}

void 		SGTextbox2d_SetText(SGTextbox2d *_this, const char * in,... ){
	SGTextbox2dData *_data=_this->data;
	char out[4096];

	ZG_VARGS(out,in);

	Textbox_SetText(_data->textbox,out);
}

void 		SGTextbox2d_SetWText(SGTextbox2d *_this, const wchar_t * in,... ){
	SGTextbox2dData *_data=_this->data;
	wchar_t out[4096];

	ZG_WVARGS(out,in);

	Textbox_WSetText(_data->textbox,out);
}


void	   	SGTextbox2d_Delete(SGTextbox2d *_this){
	SGTextbox2dData *_data=_this->data;

	Textbox_Delete(_data->textbox);

	FREE(_data);
	FREE(_this);

}
