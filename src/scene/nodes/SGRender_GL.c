#include "scene/zg_scene.h"

void SGRender_GL_SetSGCamera(SGCamera *sg_camera){

	Vector3f rotate,translate;
	Quaternion q_inv;
	Matrix4f m_inv;

	Transform *transform=TransformNode_GetTransform(sg_camera->node,TRANSFORM_NODE_TRANSFORM_TYPE_WORLD);


	translate = transform->translate;
	rotate = transform->rotate;

	// rotate negate...
	q_inv=Quaternion_FromEulerV3f(
			(Vector3f){
				.x=-rotate.x
				,.y=-rotate.y
				,.z=-rotate.z
			}
	);
	m_inv=Quaternion_ToMatrix4f(q_inv);


	glLoadMatrixf(&m_inv.e11);

	glTranslatef(-translate.x, -translate.x,-translate.x);
}
