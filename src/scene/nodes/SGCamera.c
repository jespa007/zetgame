#include "../zg_scene.h"

SGCamera * SGCamera_New(void){
	SGCamera * sg_camera = ZG_NEW(SGCamera);
	// Shape3d it stores transform3d and it can render for debbuging.
	sg_camera->node = TransformNode_New();

	sg_camera->projection_mode=PROJECTION_MODE_PERSPECTIVE;
	return sg_camera;
}

void SGCamera_Delete(SGCamera *_this){
	if(_this==NULL) return;

	TransformNode_Delete(_this->node);

	free(_this);

}


