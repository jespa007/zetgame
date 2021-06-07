#ifndef __APPEARANCE_H__
#define __APPEARANCE_H__


#define APPEARANCE_CULL_FACE 		(0x1 << 0)
#define APPEARANCE_DEPTH_TEST		(0x1 << 1)
#define APPEARANCE_LIGHT	  		(0x1 << 2)
#define APPEARANCE_TRANSPARENCY 	(0x1 << 3)
#define APPEARANCE_INTENSITY	  	(0x1 << 4)


typedef struct Appearance Appearance;

struct Appearance{
	Material 	*	material;//, * material_default;
	Texture  	*	texture;
	Transform * 	transform_texture;
	uint32_t 			properties;
};

Appearance * Appearance_New(void);
void Appearance_Apply(Appearance * appearance);
void Appearance_SetColor(Appearance * appearance, Color4f _color);
void Appearance_SetColor3i(Appearance * _this, uint8_t r, uint8_t g, uint8_t b);
void Appearance_Restore(Appearance * appearance);
void Appearance_Delete(Appearance * appearance);
#endif
