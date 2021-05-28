#ifndef __TRANSFORM_H__
#define __TRANSFORM_H__

typedef enum{
	TRANSFORM_CHANNEL_TRANSLATE_X	=0,
	TRANSFORM_CHANNEL_TRANSLATE_Y	,
	TRANSFORM_CHANNEL_TRANSLATE_Z	,
	TRANSFORM_CHANNEL_ROTATE_X    ,
	TRANSFORM_CHANNEL_ROTATE_Y  	,
	TRANSFORM_CHANNEL_ROTATE_Z  	,
	TRANSFORM_CHANNEL_SCALE_X  	,
	TRANSFORM_CHANNEL_SCALE_Y  	,
	TRANSFORM_CHANNEL_SCALE_Z  	,
	TRANSFORM_CHANNEL_MAX
}TransformChannel;

typedef struct Transform Transform;

struct Transform{
	//----------------------------
	// DON'T MOVE! THESE ORDER IS NEEDED FOR PASS ANIMATION COMPONENTS
	Vector3f translate;
	Vector3f rotate;	// euler angles
	Vector3f scale;
	// DON'T MOVE!
	//----------------------------

	Quaternion 	quaternion;	// keep to perform fast interpolations between rotations
	//Matrix3f	matrix3f; 	// keep to perform fast transformations as well as flush into gl
	uint32_t	transform_properties;
};

Transform * Transform_New(void);
Transform	  Transform_ResetValues(void);
void 		  Transform_Update(Transform *_this);

void 		Transform_Apply(Transform *_this);

// Translate
void Transform_SetTranslate3f(Transform *_this, float x,float y, float z);
void Transform_SetTranslateX(Transform *_this,float x);
void Transform_SetTranslateY(Transform *_this,float y);
void Transform_SetTranslateZ(Transform *_this,float z);

// For 2D position
void 		Transform_SetPosition2i(Transform *_this, int x,int y);
//void 		Transform_SetPositionRelativeX(Transform *_this,int x);
//void 		Transform_SetPositionRelativeY(Transform *_this,int y);
//void 		Transform_SetPositionRelative2i(Transform *_this, int x,int y);
// GetPosition: it should be done by setPosition2i or absolute translate done in scenegraph. (Warning it doesn't work for set position relative)
Vector2i 	Transform_GetPosition2i(Transform *_this);

// Rotation
void Transform_SetRotate3f(Transform *_this, float x,float y, float z);
void Transform_SetRotateZ(Transform *_this, float z);

// Scale
void Transform_SetScale3f(Transform *_this, float x,float y, float z);
void Transform_Restore(Transform *_this);
void Transform_Delete(Transform *_this);



#endif
