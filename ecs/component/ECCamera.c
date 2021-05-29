#include "ecs/zg_ecs.h"

ECCamera * ECCamera_New(void){
	ECCamera * ec_camera = NEW(ECCamera);
	// Shape3d it stores transform3d and it can render for debbuging.
	//ec_camera->node = ECSGNode_New();

	ec_camera->projection_mode=PROJECTION_MODE_PERSPECTIVE;
	return ec_camera;
}

void ECCamera_Delete(ECCamera *_this){
	if(_this==NULL) return;

	free(_this);

}
