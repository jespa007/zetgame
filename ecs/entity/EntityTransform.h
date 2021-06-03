#ifndef __ENTITY_TRANSFORM_H__
#define __ENTITY_TRANSFORM_H__


void		EntityTransform_SetTranslate3f(Entity *_this,float x, float y, float z);
void		EntityTransform_SetRotate3f(Entity *_this,float x, float y, float z);

void		EntityTransform_SetPosition2i(Entity *_this,int x, int y);
Vector3f	EntityTransform_GetTranslate(Entity *_this,TransformNodeType transform_type);
void 	 	EntityTransform_Attach(Entity *_this, Entity *_entity);


#endif
