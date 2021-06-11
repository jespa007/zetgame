#include "Appearance_GL.c"

Appearance * Appearance_New(void){
	Appearance * appearance=NEW(Appearance);
	appearance->material=NULL;//appearance->material_default=Material_New(0);
	appearance->transform_texture=NULL;
	appearance->texture=NULL;//Texture_GetDefault();

	appearance->properties=
				APPEARANCE_TRANSPARENCY;
			//|   APPEARANCE_LIGHT;

	return appearance;
}

void Appearance_SetTexture(Appearance * _this, Texture * texure){

}

void Appearance_SetMaterial(Appearance * _this, Material * material){

}

void Appearance_Apply(Appearance *_this){

	switch(Graphics_GetGraphicsApi()){
		default:
			break;
		case GRAPHICS_API_GL:
			Appearance_GL_Apply(_this);
			break;
	}
}

void Appearance_SetColor3i(Appearance * _this, uint8_t r, uint8_t g, uint8_t b){
	Material_SetColor3i(_this->material,r,g,b);
}

void Appearance_Restore(Appearance *_this){

	switch(Graphics_GetGraphicsApi()){
		default:
			break;
		case GRAPHICS_API_GL:
			Appearance_GL_Restore(_this);
			break;
	}
}

void Appearance_Delete(Appearance * _this){
	if(_this != NULL){
		FREE(_this);
	}
}
