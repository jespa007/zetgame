#include "../../zg_component_system.h"

ECCamera * ECCamera_New(void){
	ECCamera * ec_camera = ZG_NEW(ECCamera);
	// Shape3d it stores transform3d and it can render for debbuging.
	//ec_camera->node = ECTransform_New();

	ec_camera->projection_mode=PROJECTION_MODE_PERSPECTIVE;
	return ec_camera;
}

void ECCamera_Delete(ECCamera *_this){
	if(_this==NULL) return;

	free(_this);

}
