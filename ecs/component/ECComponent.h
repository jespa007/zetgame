#ifndef __EC_COMPONENT_H__
#define __EC_COMPONENT_H__

typedef enum uint32_t MskEcTypeChannels;

typedef enum{
	EC_COMPONENT_TRANSFORM=0,		// entity has a position in a world
	EC_COMPONENT_RENDERABLE, 		// entity geometry + texture + appearance
	EC_COMPONENT_CAMERA, 			// entity has camera
	EC_COMPONENT_VIEWER2D, 			// entity has viewer2d (support texture/video)
	EC_COMPONENT_TEXT2D, 			// entity dimension + texture + appearance
	EC_COMPONENT_MAX
}ECComponent;

#define MSK_EC_COMPONENT_TRANSFORM		(0x1<<EC_COMPONENT_TRANSFORM)
#define MSK_EC_COMPONENT_SHAPE			(0x1<<EC_COMPONENT_SHAPE)
#define MSK_EC_COMPONENT_CAMERA			(0x1<<EC_COMPONENT_CAMERA)




#endif

