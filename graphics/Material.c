#include "Material_GL.c"

Material * Material_New(uint32_t properties){
	Material *material=NEW(Material);
	material->properties=properties;
	material->material_data=NULL;

	Material_Reset(material);

	return material;
}

void Material_Reset(Material *material){
	material->color=(Color4f){.r=1.0f,.g=1.0f,.b=1.0f,.a=1.0f};
	material->diffuse=(Color4f){.r=0.7f,.g=0.7f,.b=0.7f,.a=1.0f};
	material->emissive=(Color4f){.r=0.0f,.g=0.0f,.b=0.0f,.a=1.0f};
	material->specular=(Color4f){.r=1.0f,.g=1.0f,.b=1.0f,.a=1.0f};
	material->ambient=(Color4f){.r=1.0f,.g=0.2f,.b=1.0f,.a=1.0f};
	material->shininess=0;
}

void Material_Apply(Material * material){
	switch(Graphics_GetGraphicsApi()){
		default:
			break;
		case GRAPHICS_API_GL:
			Material_GL_Apply(material);
			break;
	}
}

void Material_SetColor3i(Material *material,uint8_t r,uint8_t g,uint8_t b){
	if(material == NULL) return;

	material->color.r=r;
	material->color.g=g;
	material->color.b=b;


}

void Material_Delete(Material *material){
	if(material != NULL){
		FREE(material);
	}
}
