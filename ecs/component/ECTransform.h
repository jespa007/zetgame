#ifndef __EC_TRANSFORM_H__
#define __EC_TRANSFORM_H__

typedef struct ECTransform 	ECTransform;
typedef struct Entity		Entity;

#define EC_TRANSFORM_SCALE			0x0001
#define EC_TRANSFORM_TRANSLATE		0x0002
#define EC_TRANSFORM_ROTATE			0x0004


struct ECTransform{
	Entity 			*entity;
	Transform 		transform; // world coordinates
	void 			*data;
};

ECTransform *	ECTransform_New(Entity *_entity);
bool			ECTransform_AttachNode(ECTransform *_this, ECTransform *node);
bool			ECTransform_DetachNode(ECTransform *_this, ECTransform *node);
void 			ECTransform_SetTranslate3f(ECTransform *_this,float x, float y, float z);

// 2D Stuff
void 			ECTransform_SetPosition2i(ECTransform *_this,int x, int y);
Vector2i		ECTransform_GetPosition2i(ECTransform *_this);

ECTransform	*	ECTransform_GetParent(ECTransform *_this);

Transform *		ECTransform_GetTransform(ECTransform *node, TransformNodeType sgtransform);

void 			ECTransform_SetRotate3f(ECTransform *_this,float x, float y, float z);
void 			ECTransform_SetScale3f(ECTransform *_this,float x, float y, float z);
void			ECTransform_Update(ECTransform *_this);
void 	 		ECTransform_Delete(ECTransform *_this);


#endif
