#include "ecs/zg_ecs.h"

typedef struct{
	void *void_data;
}ECTextBoxRendererData;

static EComponent g_ec_textbox_renderer_required_components[]={
		EC_TRANSFORM
};

EComponentList ECTextBoxRenderer_RequiredComponents(void){
	EComponentList cl;
	cl.components=g_ec_textbox_renderer_required_components;
	cl.n_components=ARRAY_SIZE(g_ec_textbox_renderer_required_components);

	return cl;
}

void ECTextBoxRenderer_Setup(void *_this,ComponentId _id, Entity *_entity){
	ECTextBoxRenderer *ec_textbox_renderer=_this;
	ec_textbox_renderer->header.entity=_entity;
	ec_textbox_renderer->header.id=_id;
	ec_textbox_renderer->textbox=TextBox_New();

	_entity->components[EC_TEXTBOX_RENDERER]=_this;


	ECTextBoxRendererData *data=ZG_NEW(ECTextBoxRendererData);
	//data->ec_textbox_renderer=_entity->components[EC_TEXTBOX_RENDERER];

	ec_textbox_renderer->data=data;

	//ECTextBoxRenderer_SetDimensions(ec_textbox_renderer,100,100); // default with/height
}

void ECTextBoxRenderer_Update(void *_this){
	ECTextBoxRenderer *ec_textbox_renderer=_this;
	Transform *transform = NULL;
	ECTransform *ec_transform=ec_textbox_renderer->header.entity->components[EC_TRANSFORM];


	if(ec_transform){
		transform=&ec_transform->transform;
	}

	if(ec_textbox_renderer){
		TextBox_Draw(ec_textbox_renderer->textbox,transform,NULL);
	}

}

void ECTextBoxRenderer_Destroy(void *_this){
	ECTextBoxRenderer *ec_textbox_renderer=_this;
	ECTextBoxRendererData * data= ((ECTextBoxRenderer *)_this)->data;
	TextBox_Delete(ec_textbox_renderer->textbox);
	ZG_FREE(data);
}


