#ifndef __ZG_MATERIAL_H__
#define __ZG_MATERIAL_H__


#define ZG_ALPHA_SOLID 			1.0f
#define ZG_ALPHA_TRANSPARENT 	0.0f

#define ZG_MATERIAL_LIGHT	(0x1 << 0)

typedef enum{
	ZG_MATERIAL_COMPONENT_COLOR_R		=0,
	ZG_MATERIAL_COMPONENT_COLOR_G		,
	ZG_MATERIAL_COMPONENT_COLOR_B		,
	ZG_MATERIAL_COMPONENT_COLOR_A		,

	ZG_MATERIAL_COMPONENT_DIFFUSE_R	,
	ZG_MATERIAL_COMPONENT_DIFFUSE_G	,
	ZG_MATERIAL_COMPONENT_DIFFUSE_B	,
	ZG_MATERIAL_COMPONENT_DIFFUSE_A	,

	ZG_MATERIAL_COMPONENT_EMISSIVE_R	,
	ZG_MATERIAL_COMPONENT_EMISSIVE_G	,
	ZG_MATERIAL_COMPONENT_EMISSIVE_B	,
	ZG_MATERIAL_COMPONENT_EMISSIVE_A	,

	ZG_MATERIAL_COMPONENT_SPECULAR_R	,
	ZG_MATERIAL_COMPONENT_SPECULAR_G	,
	ZG_MATERIAL_COMPONENT_SPECULAR_B	,
	ZG_MATERIAL_COMPONENT_SPECULAR_A	,

	ZG_MATERIAL_COMPONENT_AMBIENT_R	,
	ZG_MATERIAL_COMPONENT_AMBIENT_G	,
	ZG_MATERIAL_COMPONENT_AMBIENT_B	,
	ZG_MATERIAL_COMPONENT_AMBIENT_A	,

	ZG_MATERIAL_COMPONENT_SHININESS	,
	ZG_MATERIAL_COMPONENT_MAX
}ZG_MaterialComponent;


typedef struct ZG_Material ZG_Material;

struct ZG_Material{
	//----------------------------
	// DON'T MOVE! THESE ORDER IS NEEDED FOR PASS ANIMATION CHANNELS
	ZG_Color4f color;
	ZG_Color4f diffuse;
	ZG_Color4f emissive;
	ZG_Color4f specular;
	ZG_Color4f ambient;
	float   shininess;
	// DON'T MOVE! THESE ORDER IS NEEDED FOR PASS ANIMATION CHANNELS
	//----------------------------


	uint32_t properties;

	void *material_data;
};

ZG_Material * ZG_Material_New(uint32_t properties);
ZG_Material ZG_Material_DefaultValues(void);
void ZG_Material_SetAlpha(ZG_Material *material,float alpha);
void ZG_Material_SetColor3i(ZG_Material *material,uint8_t r,uint8_t g,uint8_t b);
void ZG_Material_Apply(ZG_Material * material);
void ZG_Material_Delete(ZG_Material *material);

#endif
