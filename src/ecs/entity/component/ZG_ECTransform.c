#include "ecs/_zg_ecs_.h"


//------------------------------------------------------------------------------------

void ZG_ECTransform_OnCreate(void *_this){

	ZG_ECTransform * ec_transform = _this;

	ec_transform->transform=ZG_Transform_DefaultValues();

}


void 	 ZG_ECTransform_OnDestroy(void *_this){

}
