#include "scene/zg_scene.h"


typedef struct{
		void *data;
}SGText2dData;


SGText2d * SGText2d_New(Scene *_scene, Entity *_entity){
	SGText2d *sg_text_2d = ZG_NEW(SGText2d);

	sg_text_2d->sg_node=SGNode_New(_scene,_entity);
	if(sg_text_2d->sg_node==NULL){
		return NULL;
	}

	SGText2dData *data = ZG_NEW(SGText2dData);
	sg_text_2d->data=data;
	sg_text_2d->textbox=((ECTextBoxRenderer *)_entity->components[EC_TEXTURE])->textbox;

	return sg_text_2d;

}

void	   SGText2d_Delete(SGText2d *_this){
	if(_this == NULL) return;
	SGText2dData * data=_this->data;

	SGNode_Delete(_this->sg_node);

	ZG_FREE(data);
	ZG_FREE(_this);
}
