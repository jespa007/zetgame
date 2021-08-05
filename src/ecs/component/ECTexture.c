#include "ecs/zg_ecs.h"


void ECTexture_Setup(void *_this, Entity *_entity){
	ECTexture *ec_texture=_this;
	ec_texture->entity=_entity;
	ec_texture->id=EC_TEXTURE;
	_entity->components[EC_TEXTURE]=_this;


	ec_texture->texture=NULL;//Texture_New();
}

void ECTexture_SetTexture(ECTexture *_this,Texture *_texture){
	Texture *texture=_texture;
	if(texture==NULL){ // set default
		texture=Texture_GetDefault();
	}
	_this->texture=texture;
}

void ECTexture_Destroy(void *_this){
	FREE(_this);
}
