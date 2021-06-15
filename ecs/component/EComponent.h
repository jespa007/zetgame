#ifndef _ECOMPONENT_H__
#define _ECOMPONENT_H__

typedef struct Entity Entity;

// Builtin components
typedef enum{
	EC_INVALID=0,			// reserver for invalid component
	EC_TRANSFORM,			// entity transform
	EC_TRANSFORM_ANIMATION,
	// to render...
	EC_GEOMETRY,			// entity sprite render
	EC_MATERIAL,			// entity sprite render
	EC_MATERIAL_ANIMATION,
	EC_TEXTURE,			// entity sprite render
	EC_SPRITE_RENDERER,	// entity sprite render2D

	// later register
	EC_TEXTURE_ANIMATION,
	EC_GEOMETRY_ANIMATION,


	// animations...
	/*ENTITY_COMPONENT_CAMERA, 			// entity has camera
	ENTITY_COMPONENT_VIEWER2D, 			// entity has viewer2d (support texture/video)
	ENTITY_COMPONENT_TEXT2D, 			// entity dimension + texture + appearance*/

	EC_MAX_COMPONENTS
}EComponent;

typedef struct{
	EComponent  id;
	Entity		*entity;
}EComponentHeader;


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

