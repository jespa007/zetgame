#ifndef _ZG_ECOMPONENT_H__
#define _ZG_ECOMPONENT_H__


// Builtin components
/*typedef enum{
	ZG_EC_INVALID=0,			// reserver for invalid component
	EC_TRANSFORM,			// entity transform

	// to render...
	EC_GEOMETRY,			// entity sprite render
	EC_MATERIAL,			// entity sprite render
	EC_TEXTURE,			// entity sprite render
	EC_SPRITE_RENDERER,	// entity sprite render2D
	// later register
	EC_TEXTBOX_RENDERER,	// entity sprite render2D


	// animations...
	ENTITY_COMPONENT_CAMERA, 			// entity has camera
	ENTITY_COMPONENT_VIEWER2D, 			// entity has viewer2d (support texture/video)
	ENTITY_COMPONENT_TEXT2D, 			// entity dimension + texture + appearance

	EC_MAX_COMPONENTS
}ZG_EComponent;*/

#define ZG_IDX_EC_INVALID -1

extern int ZG_IDX_EC_TRANSFORM;
extern int ZG_IDX_EC_GEOMETRY;
extern int ZG_IDX_EC_MATERIAL;
extern int ZG_IDX_EC_TEXTURE;
extern int ZG_IDX_EC_SPRITE_RENDERER;
extern int ZG_IDX_EC_TEXTBOX_RENDERER;
extern int ZG_IDX_EC_TRANSFORM_ANIMATION;
extern int ZG_IDX_EC_MATERIAL_ANIMATION;
extern int ZG_IDX_EC_COLLIDER_RECTANGLE2D;
extern int ZG_IDX_EC_COLLIDER_CIRCLE2D;



typedef struct{
	const char **	components;
	size_t			n_components;
}ZG_ComponentList;


/*
int EntityComponent_Register(const char *_name);
int EntityComponent_Max();
int EntityComponent_DeInit();
*/
#endif

