#ifndef _EC_TRANSFORM_H__
#define _EC_TRANSFORM_H__

typedef struct ECTransform 	ECTransform;
typedef struct Entity		Entity;

#define EC_TRANSFORM_SCALE			0x0001
#define EC_TRANSFORM_TRANSLATE		0x0002
#define EC_TRANSFORM_ROTATE			0x0004

typedef enum{
	 EC_TRANSFORM_TYPE_LOCAL=0
	,EC_TRANSFORM_TYPE_WORLD
}ECTransformType;

struct ECTransform{
	Entity 			*entity;
	Transform 		transform; // world coordinates
	void 			*data;
};

void			ECTransform_Setup(void *_this);
void			ECTransform_Ini(void *_this,Entity *_entity);

bool			ECTransform_Attach(ECTransform *_this, ECTransform *node);
bool			ECTransform_Detach(ECTransform *_this, ECTransform *node);
void 			ECTransform_SetTranslate3f(ECTransform *_this,float x, float y, float z);

// 2D Stuff
void 			ECTransform_SetPosition2i(ECTransform *_this,int x, int y);
Vector2i		ECTransform_GetPosition2i(ECTransform *_this);

ECTransform	*	ECTransform_GetParent(ECTransform *_this);

Transform *		ECTransform_GetTransform(ECTransform *node, ECTransformType ec_transform_type);

void 			ECTransform_SetRotate3f(ECTransform *_this,float x, float y, float z);
void 			ECTransform_SetScale3f(ECTransform *_this,float x, float y, float z);

void			ECTransform_Update(void *_this);
void 	 		ECTransform_Destroy(void *_this);



#endif
