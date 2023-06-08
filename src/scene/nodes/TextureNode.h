#ifndef __TEXTURE_NODE_H__
#define __TEXTURE_NODE_H__

struct TextureNode{
	ZG_ECTransform 			*ec_transform;
	ZG_ECMaterial 				*ec_material;
	ZG_ECTexture 				*ec_texture;
	ZG_ECSpriteRenderer 		*ec_sprite_renderer;
	void 					*data;
};

TextureNode * 	TextureNode_New(Scene *_scene, ZG_Entity *_entity);
void	   		TextureNode_Delete(TextureNode *_this);

#endif
