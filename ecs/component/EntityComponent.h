#ifndef __ENTITY_COMPONENT_H__
#define __ENTITY_COMPONENT_H__

typedef enum uint32_t MskEcTypeChannels;

typedef enum{
	ENTITY_COMPONENT_TRANSFORM=0,		// entity transform
	ENTITY_COMPONENT_RENDERABLE, 		// entity geometry + appearance
	ENTITY_COMPONENT_CAMERA, 			// entity has camera
	ENTITY_COMPONENT_VIEWER2D, 			// entity has viewer2d (support texture/video)
	ENTITY_COMPONENT_TEXT2D, 			// entity dimension + texture + appearance
	ENTITY_COMPONENT_MAX
}EntityComponent;

#define MSK_ENTITY_COMPONENT_TRANSFORM		(0x1<<ENTITY_COMPONENT_TRANSFORM)
#define MSK_ENTITY_COMPONENT_SHAPE			(0x1<<ENTITY_COMPONENT_SHAPE)
#define MSK_ENTITY_COMPONENT_CAMERA			(0x1<<ENTITY_COMPONENT_CAMERA)




#endif

