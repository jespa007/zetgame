#include "ecs/zg_ecs.h"


void ECTexture_Setup(void *_this, Entity *_entity){
	ECTexture *ec_texture=_this;
	ec_texture->header.entity=_entity;
	ec_texture->header.id=EC_TEXTURE;
	_entity->components[EC_TEXTURE]=_this;


	ec_texture->texture=NULL;//Texture_New();
}

void ECTexture_SetTexture(ECTexture *_this,Texture *_texture){
	Texture *texture=_texture;
	if(texture==NULL){ // set default
		texture=TextureManager_GetDefaultTexture();
	}
	_this->texture=texture;
}

