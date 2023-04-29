#ifndef __MATERIAL_H__
#define __MATERIAL_H__


#define MATERIAL_LIGHT	(0x1 << 0)

typedef enum{
	MATERIAL_COMPONENT_COLOR_R		=0,
	MATERIAL_COMPONENT_COLOR_G		,
	MATERIAL_COMPONENT_COLOR_B		,
	MATERIAL_COMPONENT_COLOR_A		,

	MATERIAL_COMPONENT_DIFFUSE_R	,
	MATERIAL_COMPONENT_DIFFUSE_G	,
	MATERIAL_COMPONENT_DIFFUSE_B	,
	MATERIAL_COMPONENT_DIFFUSE_A	,

	MATERIAL_COMPONENT_EMISSIVE_R	,
	MATERIAL_COMPONENT_EMISSIVE_G	,
	MATERIAL_COMPONENT_EMISSIVE_B	,
	MATERIAL_COMPONENT_EMISSIVE_A	,

	MATERIAL_COMPONENT_SPECULAR_R	,
	MATERIAL_COMPONENT_SPECULAR_G	,
	MATERIAL_COMPONENT_SPECULAR_B	,
	MATERIAL_COMPONENT_SPECULAR_A	,

	MATERIAL_COMPONENT_AMBIENT_R	,
	MATERIAL_COMPONENT_AMBIENT_G	,
	MATERIAL_COMPONENT_AMBIENT_B	,
	MATERIAL_COMPONENT_AMBIENT_A	,

	MATERIAL_COMPONENT_SHININESS	,
	MATERIAL_COMPONENT_MAX
}MaterialComponent;


typedef struct Material Material;

struct Material{
	//----------------------------
	// DON'T MOVE! THESE ORDER IS NEEDED FOR PASS ANIMATION CHANNELS
	Color4f color;
	Color4f diffuse;
	Color4f emissive;
	Color4f specular;
	Color4f ambient;
	float   shininess;
	// DON'T MOVE! THESE ORDER IS NEEDED FOR PASS ANIMATION CHANNELS
	//----------------------------


	uint32_t properties;

	void *material_data;
};

Material * Material_New(uint32_t properties);
void Material_Reset(Material *material);
void Material_SetAlpha(Material *material,float alpha);
void Material_SetColor3i(Material *material,uint8_t r,uint8_t g,uint8_t b);
void Material_Apply(Material * material);
void Material_Delete(Material *material);

#endif
