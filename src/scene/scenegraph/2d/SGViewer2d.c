#include "scene/zg_scene.h"


typedef struct{
	ECTexture 			*	ec_texture;
	ECSpriteRenderer 	*	ec_sprite_renderer;
}SGViewer2dData;


SGViewer2d * SGViewer2d_New(Scene *_scene, Entity *_entity){
	SGViewer2d *sg_image = ZG_NEW(SGViewer2d);

	sg_image->sg_node=SGNode_New(_scene,_entity);
	if(sg_image->sg_node==NULL){
		return NULL;
	}

	SGViewer2dData *data = ZG_NEW(SGViewer2dData);
	data->ec_texture=_entity->components[EC_TEXTURE];
	data->ec_sprite_renderer=_entity->components[EC_SPRITE_RENDERER];
	sg_image->data=data;
	SGViewer2d_SetDimensions(sg_image,100,100);

	return sg_image;

}

void SGViewer2d_SetDimensions(SGViewer2d *_this,uint16_t width, uint16_t height){
	SGViewer2dData * data=_this->data;

	ECSpriteRenderer_SetDimensions(data->ec_sprite_renderer,width, height);
}

void		SGViewer2d_SetTexture(SGViewer2d *_this,Texture *texture){
	SGViewer2dData * data=_this->data;
	ECTexture_SetTexture(data->ec_texture,texture);
}


void	   SGViewer2d_Delete(SGViewer2d *_this){
	if(_this == NULL) return;
	SGViewer2dData * data=_this->data;

	SGNode_Delete(_this->sg_node);

	ZG_FREE(data);
	ZG_FREE(_this);
}
