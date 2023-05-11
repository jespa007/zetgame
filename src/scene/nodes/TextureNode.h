#ifndef __TEXTURE_NODE_H__
#define __TEXTURE_NODE_H__

struct TextureNode{
	ECTransform 			*ec_transform;
	ECTransform 			*ec_transform_animation;
	ECMaterial 				*ec_material;
	ECMaterialAnimation 	*ec_material_animation;
	ECTexture 				*ec_texture;
	ECSpriteRenderer 		*ec_sprite_renderer;
	void 					*data;
};

TextureNode * 	TextureNode_New(Entity *_entity);
void	   		TextureNode_Delete(TextureNode *_this);

#endif
