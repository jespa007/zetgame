#ifndef _ECOMPONENT_H__
#define _ECOMPONENT_H__

// Builtin components
typedef enum{
	ECOMPONENT_INVALID=0,			// reserver for invalid component
	ECOMPONENT_TRANSFORM,			// entity transform
	// to render...
	ECOMPONENT_GEOMETRY,			// entity sprite render
	ECOMPONENT_MATERIAL,			// entity sprite render
	ECOMPONENT_TEXTURE,			// entity sprite render

	ECOMPONENT_SPRITE_RENDERER,	// entity sprite render2D

	// animations...
	ECOMPONENT_TRANSFORM_ANIMATION,
	ECOMPONENT_MATERIAL_ANIMATION,
	ECOMPONENT_TEXTURE_ANIMATION,
	ECOMPONENT_GEOMETRY_ANIMATION,
	/*ENTITY_COMPONENT_CAMERA, 			// entity has camera
	ENTITY_COMPONENT_VIEWER2D, 			// entity has viewer2d (support texture/video)
	ENTITY_COMPONENT_TEXT2D, 			// entity dimension + texture + appearance*/

	ECOMPONENT_MAX
}EComponent;


typedef struct{
	EComponent *	components;
	size_t			n_components;
}EComponentList;

/*
int EntityComponent_Register(const char *_name);
int EntityComponent_Max();
int EntityComponent_DeInit();
*/
#endif

