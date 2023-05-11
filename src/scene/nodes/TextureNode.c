#include "scene/zg_scene.h"


typedef struct{
	void *data;
}TextureNodeData;


TextureNode * TextureNode_New(Entity *_entity){
	TextureNode *texture_node = ZG_NEW(TextureNode);

	texture_node->ec_transform=_entity->components[EC_TRANSFORM];
	texture_node->ec_transform_animation=_entity->components[EC_TRANSFORM_ANIMATION];
	texture_node->ec_transform=_entity->components[EC_MATERIAL];
	texture_node->ec_transform_animation=_entity->components[EC_MATERIAL_ANIMATION];

	texture_node->ec_texture=_entity->components[EC_TEXTURE];
	texture_node->ec_sprite_renderer=_entity->components[EC_SPRITE_RENDERER];

	TextureNodeData *data = ZG_NEW(TextureNodeData);
	texture_node->data=data;


	return texture_node;

}

void	   TextureNode_Delete(TextureNode *_this){
	if(_this == NULL) return;
	TextureNodeData * data=_this->data;
	ZG_FREE(data);
	ZG_FREE(_this);
}
