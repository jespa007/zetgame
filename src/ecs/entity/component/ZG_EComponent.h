#ifndef _ECOMPONENT_H__
#define _ECOMPONENT_H__


// Builtin components
/*typedef enum{
	EC_INVALID=0,			// reserver for invalid component
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
}EComponent;*/

#define EC_INVALID -1

typedef int EComponent;

typedef struct{
	ComponentId		entity_components_id; //
	EntityManager	*entity_manager;
}EComponentHeader;


typedef struct{
	EComponent *	components;
	size_t			n_components;
}EComponentList;

extern unsigned int __zg_ecs_n_component__;

/*
int EntityComponent_Register(const char *_name);
int EntityComponent_Max();
int EntityComponent_DeInit();
*/
#endif

