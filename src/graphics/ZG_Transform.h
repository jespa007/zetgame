#ifndef __ZG_TRANSFORM_H__
#define __ZG_TRANSFORM_H__

typedef struct ZG_Transform ZG_Transform;


typedef enum{
	ZG_TRANSFORM_COMPONENT_TRANSLATE_X	=0,
	ZG_TRANSFORM_COMPONENT_TRANSLATE_Y	,
	ZG_TRANSFORM_COMPONENT_TRANSLATE_Z	,
	ZG_TRANSFORM_COMPONENT_ROTATE_X    ,
	ZG_TRANSFORM_COMPONENT_ROTATE_Y  	,
	ZG_TRANSFORM_COMPONENT_ROTATE_Z  	,
	ZG_TRANSFORM_COMPONENT_SCALE_X  	,
	ZG_TRANSFORM_COMPONENT_SCALE_Y  	,
	ZG_TRANSFORM_COMPONENT_SCALE_Z  	,
	ZG_TRANSFORM_COMPONENT_MAX
}ZG_TransformComponent;

struct ZG_Transform{
	//----------------------------
	// DON'T MOVE! THESE ORDER IS NEEDED FOR PASS ANIMATION COMPONENTS
	ZG_Vector3f translate;
	ZG_Vector3f rotate;	// euler angles
	ZG_Vector3f scale;
	// DON'T MOVE!
	//----------------------------
};

ZG_Transform	ZG_Transform_New(void);
ZG_Transform	ZG_Transform_DefaultValues(void);

void 	  	  	ZG_Transform_Apply(ZG_Transform *_this);

ZG_Matrix4f	  	ZG_Transform_GetRotationMatrix(ZG_Transform *_this);

// Translate
void 			ZG_Transform_SetTranslate3f(ZG_Transform *_this, float x,float y, float z);
void 			ZG_Transform_SetTranslateX(ZG_Transform *_this,float x);
void 			ZG_Transform_SetTranslateY(ZG_Transform *_this,float y);
void 			ZG_Transform_SetTranslateZ(ZG_Transform *_this,float z);

// For 2D position
void 			ZG_Transform_SetPosition2i(ZG_Transform *_this, int x,int y);
void 			ZG_Transform_SetDisplacement2i(ZG_Transform *_this, int x,int y);

// GetPosition: it should be done by setPosition2i or absolute translate done in scenegraph. (Warning it doesn't work for set position relative)
ZG_Vector2i 	ZG_Transform_GetPosition2i(ZG_Transform *_this);

// Rotation
void 			ZG_Transform_SetRotate3f(ZG_Transform *_this, float x,float y, float z);
void 			ZG_Transform_SetRotateZ(ZG_Transform *_this, float z);

// Scale
void 			ZG_Transform_SetScale3f(ZG_Transform *_this, float x,float y, float z);
void 			ZG_Transform_Restore(ZG_Transform *_this);
void 			ZG_Transform_Delete(ZG_Transform *_this);



#endif
