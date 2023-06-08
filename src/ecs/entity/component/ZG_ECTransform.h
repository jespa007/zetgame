#ifndef _ZG_ECTRANSFORM_H__
#define _ZG_ECTRANSFORM_H__

typedef struct ZG_ECTransform 	ZG_ECTransform;

#define ZG_ECTRANSFORM_SCALE					0x0001
#define ZG_ECTRANSFORM_TRANSLATE			 	0x0002
#define ZG_ECTRANSFORM_ROTATE				 	0x0004
//#define EC_TRANSFORM_POSITION_RELATIVE_X 	0x0008
//#define EC_TRANSFORM_POSITION_RELATIVE_Y 	0x0010

typedef enum{
	 ZG_ECTRANSFORM_TYPE_LOCAL=0
	,ZG_ECTRANSFORM_TYPE_WORLD
}ZG_ECTransformType;

struct ZG_ECTransform{
	ZG_EComponentHeader 		header;
	ZG_Transform				transform; // world coordinates
	ZG_Quaternion				quaternion;
	void 					*data;
};

void			ZG_ECTransform_Setup(void *_this, ZG_ComponentId _id);

bool			ZG_ECTransform_Attach(ZG_ECTransform *_this, ZG_ECTransform *node);
bool			ZG_ECTransform_Detach(ZG_ECTransform *_this, ZG_ECTransform *node);
void 			ZG_ECTransform_SetTranslate3f(ZG_ECTransform *_this,float x, float y, float z);

/**
 * Sets 2d position from pixel position xy
 */
void 			ZG_ECTransform_SetPosition2i(ZG_ECTransform *_this,int x, int y);

ZG_Vector2i		ZG_ECTransform_GetPosition2i(ZG_ECTransform *_this);

/**
 * Sets 2d displacement from offset position xy
 */
void 			ZG_ECTransform_SetDisplacement2i(ZG_ECTransform *_this,int offset_x, int offset_y);



ZG_ECTransform	*	ZG_ECTransform_GetParent(ZG_ECTransform *_this);

ZG_Transform *		ZG_ECTransform_GetTransform(ZG_ECTransform *node, ZG_ECTransformType ec_transform_type);

void 			ZG_ECTransform_SetRotate3f(ZG_ECTransform *_this,float x, float y, float z);
void 			ZG_ECTransform_SetScale3f(ZG_ECTransform *_this,float x, float y, float z);


/**
 * ZG_Transform animation
 */
void ZG_ECTransform_SetTransformAnimation(ZG_ECTransform *_this, ZG_ACTransformAnimation * _ac_transform_animation);
ZG_ACTransformAnimation * ZG_ECTransform_GetTransformAnimation(ZG_ECTransform *_this);



void			ZG_ECTransform_Update(void *_this);
void 	 		ZG_ECTransform_Destroy(void *_this);



#endif
