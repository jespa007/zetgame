#include "ecs/_zg_ecs_.h"

typedef struct{
	void *void_data;
}ZG_ECTextBoxRendererData;

static const char *g_ec_textbox_renderer_required_components[]={
		ZG_NAMEOF(ZG_ZG_ECTransform)
};

ZG_EComponentList ZG_ECTextBoxRenderer_RequiredComponents(void){
	ZG_EComponentList cl;
	cl.components=g_ec_textbox_renderer_required_components;
	cl.n_components=ZG_ARRAY_SIZE(g_ec_textbox_renderer_required_components);

	return cl;
}

void ZG_ECTextBoxRenderer_Setup(void *_this,ZG_ComponentId _id, ZG_Entity *_entity){
	ZG_ECTextBoxRenderer *ec_textbox_renderer=_this;
	ec_textbox_renderer->header.entity=_entity;
	ec_textbox_renderer->header.id=_id;
	ec_textbox_renderer->textbox=ZG_TextBox_New();

	_entity->components[EC_TEXTBOX_RENDERER]=_this;


	ZG_ECTextBoxRendererData *data=ZG_NEW(ZG_ECTextBoxRendererData);
	//data->ec_textbox_renderer=_entity->components[EC_TEXTBOX_RENDERER];

	ec_textbox_renderer->data=data;

	//ZG_ECTextBoxRenderer_SetDimensions(ec_textbox_renderer,100,100); // default with/height
}

void ZG_ECTextBoxRenderer_Update(void *_this){
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

void ZG_ECTextBoxRenderer_Destroy(void *_this){
	ZG_ECTextBoxRenderer *ec_textbox_renderer=_this;
	ZG_ECTextBoxRendererData * data= ((ZG_ECTextBoxRenderer *)_this)->data;
	ZG_TextBox_Delete(ec_textbox_renderer->textbox);
	ZG_FREE(data);
}


