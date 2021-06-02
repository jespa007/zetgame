#ifndef __E_TRANSFORM_H__
#define __E_TRANSFORM_H__


void ETransform_SetTranslate3f(Entity *_this,float x, float y, float z);
Vector3f ETransform_GetTranslate(Entity *_this,TransformNodeType transform_type);


#endif
