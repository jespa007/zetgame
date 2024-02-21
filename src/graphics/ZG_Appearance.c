#include "ZG_Appearance_GL.c"

typedef struct{
	ZG_Material material_default;
}ZG_AppearanceData;

ZG_Appearance * ZG_Appearance_New(void){
	ZG_Appearance * appearance=ZG_NEW(ZG_Appearance);
	ZG_AppearanceData *data=ZG_NEW(ZG_AppearanceData);

	// Assign default values for material (A thing I miss in C++)
	data->material_default=ZG_Material_DefaultValues();

	appearance->material=&data->material_default;
	appearance->transform_texture=NULL;
	appearance->texture=NULL;//Texture_GetDefault();

	appearance->properties=
				ZG_APPEARANCE_TRANSPARENCY;
			//|   ZG_APPEARANCE_LIGHT;

	appearance->data=data;
	return appearance;
}
/*
void Appearance_SetTexture(ZG_Appearance * _this, ZG_Texture * texure){

}

void Appearance_SetMaterial(ZG_Appearance * _this, ZG_Material * material){

}
*/
void ZG_Appearance_Apply(ZG_Appearance *_this){

	switch(ZG_Graphics_GetGraphicsApi()){
		default:
			break;
		case ZG_GRAPHICS_API_GL:
			ZG_Appearance_GL_Apply(_this);
			break;
	}
}

void ZG_Appearance_SetColor3i(ZG_Appearance * _this, uint8_t r, uint8_t g, uint8_t b){
	ZG_Material_SetColor3i(_this->material,r,g,b);
}

void ZG_Appearance_Restore(ZG_Appearance *_this){

	switch(ZG_Graphics_GetGraphicsApi()){
		default:
			break;
		case ZG_GRAPHICS_API_GL:
			ZG_Appearance_GL_Restore(_this);
			break;
	}
}

void ZG_Appearance_Delete(ZG_Appearance * _this){
	if(_this != NULL){
		ZG_AppearanceData *data=_this->data;
		ZG_FREE(data);
		ZG_FREE(_this);
	}
}
