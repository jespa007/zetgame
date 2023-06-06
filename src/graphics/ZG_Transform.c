#include "ZG_Transform_GL.c"
/*
void Transform_UpdateRotate(ZG_Transform *_this){
	_this->quaternion=ZG_Quaternion_FromEulerV3f(_this->rotate);
	//_this->matrix3f=Quaternion_ToMatrix3f(_this->quaternion);
}
*/
ZG_Transform	  ZG_Transform_DefaultValues(void){
	ZG_Transform transform;
	transform.translate=Vector3f_Zeros();
	transform.rotate=Vector3f_Zeros();
	transform.scale=Vector3f_Ones();
	//transform.quaternion=ZG_Quaternion_FromEulerV3f(transform.rotate);
	//Transform_UpdateRotate(&transform);
	return transform;
}

ZG_Transform  ZG_Transform_New(void){

	return ZG_Transform_DefaultValues();

}

ZG_Matrix4f ZG_Transform_GetRotationMatrix(ZG_Transform *_this){
	ZG_Quaternion q=ZG_Quaternion_FromEulerV3f(_this->rotate);
	return ZG_Quaternion_ToMatrix4f(q);
}


void ZG_Transform_Apply(ZG_Transform *_this){

	if(_this == NULL) return;

	switch(ZG_Graphics_GetGraphicsApi()){
		default:
			break;
		case ZG_GRAPHICS_API_GL:
			ZG_Transform_GL_Apply(_this);
			break;
	}
}

//------------------
// TRANSLATE

void ZG_Transform_SetTranslate3f(ZG_Transform *_this,float x, float y, float z){
	_this->translate=Vector3f_New3f(x,y,z);
	//CLR_MSK(_this->transform_properties,TRANSFORM_PROPERTY_POSITION_RELATIVE_X|TRANSFORM_PROPERTY_POSITION_RELATIVE_Y);
}

void ZG_Transform_SetTranslateX(ZG_Transform *_this,float x){
	_this->translate.x=x;
	//CLR_MSK(_this->transform_properties,TRANSFORM_PROPERTY_POSITION_RELATIVE_X);
}

void ZG_Transform_SetTranslateY(ZG_Transform *_this,float y){
	_this->translate.y=y;
	//CLR_MSK(_this->transform_properties,TRANSFORM_PROPERTY_POSITION_RELATIVE_Y);
}

void ZG_Transform_SetTranslateZ(ZG_Transform *_this,float z){
	_this->translate.z=z;
}

void ZG_Transform_SetPosition2i(ZG_Transform *_this,int _x, int _y){
	_this->translate=ZG_ViewPort_ScreenToWorld(_x,_y);
	//CLR_MSK(_this->transform_properties,TRANSFORM_PROPERTY_POSITION_RELATIVE_X|TRANSFORM_PROPERTY_POSITION_RELATIVE_Y);
}

void ZG_Transform_SetDisplacement2i(ZG_Transform *_this,int _rel_x, int _rel_y){

	//ZG_Vector3f v=ZG_ViewPort_ScreenToWorldDimension2i(x,y);
	//ZG_Transform_SetPosition2i(&data->transform_local,x,y);
	_this->translate=(ZG_Vector3f){
		.x=ZG_ViewPort_ScreenToWorldWidth(_rel_x)
		,.y=-ZG_ViewPort_ScreenToWorldHeight(_rel_y)
		,.z=0
	};
	//CLR_MSK(_this->transform_properties,TRANSFORM_PROPERTY_POSITION_RELATIVE_X|TRANSFORM_PROPERTY_POSITION_RELATIVE_Y);
}

ZG_Vector2i ZG_Transform_GetPosition2i(ZG_Transform *_this){
	return ZG_ViewPort_WorldToScreen(_this->translate.x,_this->translate.y);
}
/*
void Transform_SetPositionRelativeX(ZG_Transform *_this,int x){
	_this->translate.x=ZG_ViewPort_ScreenToWorldWidth(x);
	_this->transform_properties|=TRANSFORM_PROPERTY_POSITION_RELATIVE_X;
}

void Transform_SetPositionRelativeY(ZG_Transform *_this,int y){
	_this->translate.y=-ZG_ViewPort_ScreenToWorldHeight(y);
	_this->transform_properties|=TRANSFORM_PROPERTY_POSITION_RELATIVE_Y;
}


void Transform_SetPositionRelative2i(ZG_Transform *_this,int x, int y){
	Transform_SetPositionRelativeX(_this,x);
	Transform_SetPositionRelativeY(_this,y);
}

ZG_Vector2i 	ZG_Transform_GetPosition2i(ZG_Transform *_this){
	return Vector2i_New(
		 ZG_ViewPort_WorldToScreenPosX(_this->translate.x)
		,ZG_ViewPort_WorldToScreenPosY(_this->translate.y)
	);
}*/

//------------------
// ROTATION



void ZG_Transform_SetRotate3f(ZG_Transform *_this,float x, float y, float z){
		_this->rotate.x=x;
		_this->rotate.y=y;
		_this->rotate.z=z;
}

void ZG_Transform_SetRotateZ(ZG_Transform *_this, float z){
	_this->rotate.z=z;
}

//------------------
// SCALE

void ZG_Transform_SetScale3f(ZG_Transform *_this,float x, float y, float z){
	_this->scale=Vector3f_New3f(x,y,z);
}

void ZG_Transform_Restore(ZG_Transform *transform){

	if(transform == NULL) return;

	switch(ZG_Graphics_GetGraphicsApi()){
		default:
			break;
		case ZG_GRAPHICS_API_GL:
			ZG_Transform_GL_Restore(transform);
			break;
	}
}

void ZG_Transform_Delete(ZG_Transform *transform){
	if(transform!=NULL){
		ZG_FREE(transform);
	}
}
