#include "ZG_Material_GL.c"



ZG_Material * ZG_Material_New(uint32_t properties){
	ZG_Material *material=ZG_NEW(ZG_Material);
	material->properties=properties;
	material->material_data=NULL;

	*material=ZG_Material_DefaultValues();

	return material;
}

ZG_Material ZG_Material_DefaultValues(void){
	ZG_Material material;
	material.color=(ZG_Color4f){.r=1.0f,.g=1.0f,.b=1.0f,.a=ZG_ALPHA_SOLID};
	material.diffuse=(ZG_Color4f){.r=0.7f,.g=0.7f,.b=0.7f,.a=ZG_ALPHA_SOLID};
	material.emissive=(ZG_Color4f){.r=0.0f,.g=0.0f,.b=0.0f,.a=ZG_ALPHA_SOLID};
	material.specular=(ZG_Color4f){.r=1.0f,.g=1.0f,.b=1.0f,.a=ZG_ALPHA_SOLID};
	material.ambient=(ZG_Color4f){.r=1.0f,.g=0.2f,.b=1.0f,.a=ZG_ALPHA_SOLID};
	material.shininess=0;

	material.properties=0;

	return material;
}



void ZG_Material_Apply(ZG_Material * material){
	switch(ZG_Graphics_GetGraphicsApi()){
		default:
			break;
		case ZG_GRAPHICS_API_GL:
			ZG_Material_GL_Apply(material);
			break;
	}
}

void ZG_Material_SetColor3i(ZG_Material *material,uint8_t r,uint8_t g,uint8_t b){
	if(material == NULL) return;

	material->color.r=r*ZG_ONE_OVER_256;
	material->color.g=g*ZG_ONE_OVER_256;
	material->color.b=b*ZG_ONE_OVER_256;
}

void ZG_Material_SetAlpha(ZG_Material *material,float alpha){
	if(material == NULL) return;

	material->color.a=alpha;
}

void ZG_Material_Delete(ZG_Material *material){
	if(material != NULL){
		ZG_FREE(material);
	}
}
