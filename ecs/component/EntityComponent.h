#ifndef _ENTITY_COMPONENT_H__
#define _ENTITY_COMPONENT_H__

// Builtin components
typedef enum{
	ENTITY_COMPONENT_INVALID=0,			// reserver for invalid component
	ENTITY_COMPONENT_TRANSFORM,			// entity transform
	// to render...
	ENTITY_COMPONENT_GEOMETRY,			// entity sprite render
	ENTITY_COMPONENT_MATERIAL,			// entity sprite render
	ENTITY_COMPONENT_TEXTURE,			// entity sprite render

	ENTITY_COMPONENT_SPRITE_RENDERER,	// entity sprite render2D

	// animations...
	ENTITY_COMPONENT_TRANSFORM_ANIMATION,
	ENTITY_COMPONENT_MATERIAL_ANIMATION,
	ENTITY_COMPONENT_TEXTURE_ANIMATION,
	ENTITY_COMPONENT_GEOMETRY_ANIMATION,
	/*ENTITY_COMPONENT_CAMERA, 			// entity has camera
	ENTITY_COMPONENT_VIEWER2D, 			// entity has viewer2d (support texture/video)
	ENTITY_COMPONENT_TEXT2D, 			// entity dimension + texture + appearance*/

	ENTITY_COMPONENT_MAX
}EntityComponent;

typedef struct{
	unsigned *	components;
	size_t		n_components;
}ComponentList;

/*
int EntityComponent_Register(const char *_name);
int EntityComponent_Max();
int EntityComponent_DeInit();
*/
#endif

