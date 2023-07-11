#include "ecs/_zg_ecs_.h"

typedef struct{
	short ec_transform_idx;
}ZG_ECTextBoxRendererData;

ZG_ComponentList ZG_ECTextBoxRenderer_RequiredComponents(void){
	ZG_ComponentList list;
	return list;
}

//---

void ZG_ECTextBoxRenderer_OnCreate(void *_this){
	ZG_ECTextBoxRenderer *ec_textbox_renderer=_this;
	ec_textbox_renderer->header.id=_id;
	ec_textbox_renderer->textbox=ZG_TextBox_New();

	ZG_ECTextBoxRendererData *data=ZG_NEW(ZG_ECTextBoxRendererData);


	data->ec_transform_idx=ZG_ENTITY_TYPE_GET_COMPONENT_IDX(ec_textbox_renderer->header.archetype,ECGeometry,_id);//_entity->components[EC_GEOMETRY];


	if(data->ec_transform_idx==ZG_INVALID_IDX){
		ZG_LOG_ERRORF("ZG_ECTextBoxRenderer_OnCreate : EntityType doesn't have transform component");
		return;
	}


	//data->ec_textbox_renderer=_entity->components[EC_TEXTBOX_RENDERER];
	//data->ec_transform=>ZG_ECS_GET_COMPONENT(_this,ECTransform);
	ec_textbox_renderer->data=data;

	//ZG_ECTextBoxRenderer_SetDimensions(ec_textbox_renderer,100,100); // default with/height
}

void ZG_ECTextBoxRenderer_OnUpdate(void *_this){
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

void ZG_ECTextBoxRenderer_OnDestroy(void *_this){
	ZG_ECTextBoxRenderer *ec_textbox_renderer=_this;
	ZG_ECTextBoxRendererData * data= ((ZG_ECTextBoxRenderer *)_this)->data;
	ZG_TextBox_Delete(ec_textbox_renderer->textbox);
	ZG_FREE(data);
}

//---


