#ifndef __ZG_APPEARANCE_H__
#define __ZG_APPEARANCE_H__


#define ZG_APPEARANCE_CULL_FACE 		(0x1 << 0)
#define ZG_APPEARANCE_DEPTH_TEST		(0x1 << 1)
#define ZG_APPEARANCE_LIGHT	  		(0x1 << 2)
#define ZG_APPEARANCE_TRANSPARENCY 	(0x1 << 3)
#define ZG_APPEARANCE_INTENSITY	  	(0x1 << 4)


typedef struct ZG_Appearance ZG_Appearance;

struct ZG_Appearance{
	ZG_Material 	*	material;//, * material_default;
	ZG_Texture  	*	texture;
	ZG_Transform 	* 	transform_texture;
	uint32_t 			properties;
};

ZG_Appearance * ZG_Appearance_New(void);
void ZG_Appearance_Apply(ZG_Appearance * appearance);
void ZG_Appearance_SetColor(ZG_Appearance * appearance, ZG_Color4f _color);
void ZG_Appearance_SetColor3i(ZG_Appearance * _this, uint8_t r, uint8_t g, uint8_t b);
void ZG_Appearance_Restore(ZG_Appearance * appearance);
void ZG_Appearance_Delete(ZG_Appearance * appearance);
#endif
