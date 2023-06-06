#include "scene/zg_scene.h"

void SGRender_GL_SetSGCamera(SGCamera *sg_camera){

	ZG_Vector3f rotate,translate;
	ZG_Quaternion q_inv;
	ZG_Matrix4f m_inv;

	ZG_Transform *transform=TransformNode_GetTransform(sg_camera->node,TRANSFORM_NODE_TRANSFORM_TYPE_WORLD);


	translate = transform->translate;
	rotate = transform->rotate;

	// rotate negate...
	q_inv=ZG_Quaternion_FromEulerV3f(
			(ZG_Vector3f){
				.x=-rotate.x
				,.y=-rotate.y
				,.z=-rotate.z
			}
	);
	m_inv=ZG_Quaternion_ToMatrix4f(q_inv);


	glLoadMatrixf(&m_inv.e11);

	glTranslatef(-translate.x, -translate.x,-translate.x);
}
