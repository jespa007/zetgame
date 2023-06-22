#include "ecs/_zg_ecs_.h"

typedef struct{
	ZG_ECTransform *ec_transform;
}ZG_ECTextBoxRendererData;

void ZG_ECTextBoxRenderer_Setup(ZG_EComponentHeader *_this,ZG_ComponentId _id){
	ZG_ECTextBoxRenderer *ec_textbox_renderer=_this;
	ec_textbox_renderer->header.id=_id;
	ec_textbox_renderer->textbox=ZG_TextBox_New();

	ZG_ECTextBoxRendererData *data=ZG_NEW(ZG_ECTextBoxRendererData);
	//data->ec_textbox_renderer=_entity->components[EC_TEXTBOX_RENDERER];
	data->ec_transform=>ZG_ECS_GET_COMPONENT(_this,ECTransform);
	ec_textbox_renderer->data=data;

	//ZG_ECTextBoxRenderer_SetDimensions(ec_textbox_renderer,100,100); // default with/height
}

void ZG_ECTextBoxRenderer_Update(ZG_EComponentHeader *_this){
	ZG_ECTextBoxRenderer *ec_textbox_renderer=_this;
	ZG_Transform *transform = NULL;
	ZG_ECTransform *ec_transform=ec_textbox_renderer->header.entity->components[EC_TRANSFORM];


	if(ec_transform){
		transform=&ec_transform->transform;
	}

	if(ec_textbox_renderer){
		ZG_TextBox_Draw(ec_textbox_renderer->textbox,transform,NULL);
	}

}

void ZG_ECTextBoxRenderer_Destroy(ZG_EComponentHeader *_this){
	ZG_ECTextBoxRenderer *ec_textbox_renderer=_this;
	ZG_ECTextBoxRendererData * data= ((ZG_ECTextBoxRenderer *)_this)->data;
	ZG_TextBox_Delete(ec_textbox_renderer->textbox);
	ZG_FREE(data);
}


