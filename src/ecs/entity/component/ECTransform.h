#ifndef _EC_TRANSFORM_H__
#define _EC_TRANSFORM_H__

typedef struct ECTransform 	ECTransform;

#define EC_TRANSFORM_SCALE					0x0001
#define EC_TRANSFORM_TRANSLATE			 	0x0002
#define EC_TRANSFORM_ROTATE				 	0x0004
//#define EC_TRANSFORM_POSITION_RELATIVE_X 	0x0008
//#define EC_TRANSFORM_POSITION_RELATIVE_Y 	0x0010

typedef enum{
	 EC_TRANSFORM_TYPE_LOCAL=0
	,EC_TRANSFORM_TYPE_WORLD
}ECTransformType;

struct ECTransform{
	EComponentHeader 		header;
	Transform				transform; // world coordinates
	Quaternion				quaternion;
	void 					*data;
};

void			ECTransform_Setup(void *_this, ComponentId _id, Entity *_entity);

bool			ECTransform_Attach(ECTransform *_this, ECTransform *node);
bool			ECTransform_Detach(ECTransform *_this, ECTransform *node);
void 			ECTransform_SetTranslate3f(ECTransform *_this,float x, float y, float z);

/**
 * Sets 2d position from pixel position xy
 */
void 			ECTransform_SetPosition2i(ECTransform *_this,int x, int y);

Vector2i		ECTransform_GetPosition2i(ECTransform *_this);

/**
 * Sets 2d displacement from offset position xy
 */
void 			ECTransform_SetDisplacement2i(ECTransform *_this,int offset_x, int offset_y);



ECTransform	*	ECTransform_GetParent(ECTransform *_this);

Transform *		ECTransform_GetTransform(ECTransform *node, ECTransformType ec_transform_type);

void 			ECTransform_SetRotate3f(ECTransform *_this,float x, float y, float z);
void 			ECTransform_SetScale3f(ECTransform *_this,float x, float y, float z);


/**
 * Transform animation
 */
void ECTransform_SetTransformAnimation(ECTransform *_this, ACTransformAnimation * _ac_transform_animation);
ACTransformAnimation * ECTransform_GetTransformAnimation(ECTransform *_this);



void			ECTransform_Update(void *_this);
void 	 		ECTransform_Destroy(void *_this);



#endif
