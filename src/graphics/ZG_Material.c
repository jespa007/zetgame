#include "Material_GL.c"

ZG_Material * ZG_Material_New(uint32_t properties){
	ZG_Material *material=ZG_NEW(ZG_Material);
	material->properties=properties;
	material->material_data=NULL;

	ZG_Material_Reset(material);

	return material;
}

void ZG_Material_Reset(ZG_Material *material){
	material->color=(ZG_Color4f){.r=1.0f,.g=1.0f,.b=1.0f,.a=1.0f};
	material->diffuse=(ZG_Color4f){.r=0.7f,.g=0.7f,.b=0.7f,.a=1.0f};
	material->emissive=(ZG_Color4f){.r=0.0f,.g=0.0f,.b=0.0f,.a=1.0f};
	material->specular=(ZG_Color4f){.r=1.0f,.g=1.0f,.b=1.0f,.a=1.0f};
	material->ambient=(ZG_Color4f){.r=1.0f,.g=0.2f,.b=1.0f,.a=1.0f};
	material->shininess=0;
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

	material->color.r=r;
	material->color.g=g;
	material->color.b=b;
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
