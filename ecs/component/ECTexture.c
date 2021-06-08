#include "ecs/zg_ecs.h"


void ECTexture_Setup(void *_this){
	ECTexture *ec_texture=_this;
	ec_texture->texture=Texture_New();
}

void ECTexture_Destroy(void *_this){
	ECTexture *ec_texture=_this;
	Texture_Delete(ec_texture->texture);

}
