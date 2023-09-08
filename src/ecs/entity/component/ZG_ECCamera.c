#include "ecs/@zg_ecs.h"

ZG_ECCamera * ZG_ECCamera_New(void){
	ZG_ECCamera * ec_camera = ZG_NEW(ZG_ECCamera);
	// Shape3d it stores transform3d and it can render for debbuging.
	//ec_camera->node = ZG_ECTransform_New();

	ec_camera->projection_mode=ZG_PROJECTION_MODE_PERSPECTIVE;
	return ec_camera;
}

void ZG_ECCamera_Delete(ZG_ECCamera *_this){
	if(_this==NULL) return;

	free(_this);

}
