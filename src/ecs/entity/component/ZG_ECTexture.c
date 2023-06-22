#include "ecs/_zg_ecs_.h"


void ZG_ECTexture_Setup(void *_this, ZG_ComponentId _id){
	ZG_ECTexture *ec_texture=_this;
	ec_texture->header.entity=_entity;
	ec_texture->header.id=_id;
	_entity->components[EC_TEXTURE]=_this;


	ec_texture->texture=ZG_TextureManager_GetDefaultTexture();//ZG_Texture_New();
}

void ZG_ECTexture_SetTexture(ZG_ECTexture *_this,ZG_Texture *_texture){
	ZG_Texture *texture=_texture;
	if(texture==NULL){ // set default
		texture=ZG_TextureManager_GetDefaultTexture();
	}
	_this->texture=texture;
}

