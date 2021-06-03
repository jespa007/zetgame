#ifndef _ENTITY_COMPONENT_H__
#define _ENTITY_COMPONENT_H__

typedef enum{
	ENTITY_COMPONENT_TRANSFORM=0,		// entity transform
	ENTITY_COMPONENT_SPRITE_RENDERER, 	// entity sprite render
	ENTITY_COMPONENT_CAMERA, 			// entity has camera
	ENTITY_COMPONENT_VIEWER2D, 			// entity has viewer2d (support texture/video)
	ENTITY_COMPONENT_TEXT2D, 			// entity dimension + texture + appearance
	ENTITY_COMPONENT_MAX
}EntityComponent;



#endif

