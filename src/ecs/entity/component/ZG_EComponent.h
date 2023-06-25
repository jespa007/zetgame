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

#define ZG_EC_INVALID -1

typedef int ZG_EComponent;

typedef struct{
	ZG_ComponentId		id; //
	ZG_EntityManager	*entity_manager;
}ZG_EComponentHeader;


typedef struct{
	const char **	components;
	size_t			n_components;
}ZG_EComponentList;


/*
int EntityComponent_Register(const char *_name);
int EntityComponent_Max();
int EntityComponent_DeInit();
*/
#endif

