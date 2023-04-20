#include "Transform_GL.c"
/*
void Transform_UpdateRotate(Transform *_this){
	_this->quaternion=Quaternion_FromEulerV3f(_this->rotate);
	//_this->matrix3f=Quaternion_ToMatrix3f(_this->quaternion);
}
*/
Transform	  Transform_DefaultValues(void){
	Transform transform;
	transform.translate=Vector3f_Zeros();
	transform.rotate=Vector3f_Zeros();
	transform.scale=Vector3f_Ones();
	//transform.quaternion=Quaternion_FromEulerV3f(transform.rotate);
	//Transform_UpdateRotate(&transform);
	return transform;
}

Transform  Transform_New(void){

	return Transform_DefaultValues();

}

Matrix4f Transform_GetRotationMatrix(Transform *_this){
	Quaternion q=Quaternion_FromEulerV3f(_this->rotate);
	return Quaternion_ToMatrix4f(q);
}


void Transform_Apply(Transform *_this){

	if(_this == NULL) return;

	switch(Graphics_GetGraphicsApi()){
		default:
			break;
		case GRAPHICS_API_GL:
			Transform_GL_Apply(_this);
			break;
	}
}

//------------------
// TRANSLATE

void Transform_SetTranslate3f(Transform *_this,float x, float y, float z){
	_this->translate=Vector3f_New3f(x,y,z);
	//CLR_MSK(_this->transform_properties,TRANSFORM_PROPERTY_POSITION_RELATIVE_X|TRANSFORM_PROPERTY_POSITION_RELATIVE_Y);
}

void Transform_SetTranslateX(Transform *_this,float x){
	_this->translate.x=x;
	//CLR_MSK(_this->transform_properties,TRANSFORM_PROPERTY_POSITION_RELATIVE_X);
}

void Transform_SetTranslateY(Transform *_this,float y){
	_this->translate.y=y;
	//CLR_MSK(_this->transform_properties,TRANSFORM_PROPERTY_POSITION_RELATIVE_Y);
}

void Transform_SetTranslateZ(Transform *_this,float z){
	_this->translate.z=z;
}

void Transform_SetPosition2i(Transform *_this,int x, int y){
	_this->translate=ViewPort_ScreenToWorld(x,y);
	//CLR_MSK(_this->transform_properties,TRANSFORM_PROPERTY_POSITION_RELATIVE_X|TRANSFORM_PROPERTY_POSITION_RELATIVE_Y);
}

Vector2i Transform_GetPosition2i(Transform *_this){
	return ViewPort_WorldToScreen(_this->translate.x,_this->translate.y);
}
/*
void Transform_SetPositionRelativeX(Transform *_this,int x){
	_this->translate.x=ViewPort_ScreenToWorldWidth(x);
	_this->transform_properties|=TRANSFORM_PROPERTY_POSITION_RELATIVE_X;
}

void Transform_SetPositionRelativeY(Transform *_this,int y){
	_this->translate.y=-ViewPort_ScreenToWorldHeight(y);
	_this->transform_properties|=TRANSFORM_PROPERTY_POSITION_RELATIVE_Y;
}


void Transform_SetPositionRelative2i(Transform *_this,int x, int y){
	Transform_SetPositionRelativeX(_this,x);
	Transform_SetPositionRelativeY(_this,y);
}

Vector2i 	Transform_GetPosition2i(Transform *_this){
	return Vector2i_New(
		 ViewPort_WorldToScreenPosX(_this->translate.x)
		,ViewPort_WorldToScreenPosY(_this->translate.y)
	);
}*/

//------------------
// ROTATION



void Transform_SetRotate3f(Transform *_this,float x, float y, float z){
	if(!(
			   _this->rotate.x==x
			&& _this->rotate.y==y
			&& _this->rotate.z==z
		)){
			_this->rotate.x=x;
			_this->rotate.y=y;
			_this->rotate.z=z;

			//Transform_UpdateRotate(_this);
	}
}

void Transform_SetRotateZ(Transform *_this, float z){
	if(_this->rotate.z!=z){
		_this->rotate.z=z;
		//Transform_UpdateRotate(_this);
	}
}

//------------------
// SCALE

void Transform_SetScale3f(Transform *_this,float x, float y, float z){
	_this->scale=Vector3f_New3f(x,y,z);
}

void Transform_Restore(Transform *transform){

	if(transform == NULL) return;

	switch(Graphics_GetGraphicsApi()){
		default:
			break;
		case GRAPHICS_API_GL:
			Transform_GL_Restore(transform);
			break;
	}
}

void Transform_Delete(Transform *transform){
	if(transform!=NULL){
		ZG_FREE(transform);
	}
}
