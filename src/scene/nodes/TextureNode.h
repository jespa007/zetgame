#ifndef __TEXTURE_NODE_H__
#define __TEXTURE_NODE_H__

struct TextureNode{
	ECTransform 			*ec_transform;
	ECMaterial 				*ec_material;
	ECTexture 				*ec_texture;
	ECSpriteRenderer 		*ec_sprite_renderer;
	void 					*data;
};

TextureNode * 	TextureNode_New(Scene *_scene, Entity *_entity);
void	   		TextureNode_Delete(TextureNode *_this);

#endif
