#ifndef __EC_COMPONENT_H__
#define __EC_COMPONENT_H__

typedef enum uint32_t MskEcTypeChannels;

typedef enum{
	EC_TYPE_COMPONENT_TRANSFORM=0,	// entity has a position in a world
	EC_TYPE_COMPONENT_CAMERA, 	// entity has camera
	EC_TYPE_COMPONENT_VIEWER2D, // entity has viewer2d (support texture/video)
	EC_TYPE_COMPONENT_TEXT2D, 	// entity dimension + texture + appearance
	EC_TYPE_COMPONENT_SHAPE3D, 	// entity geometry + texture + appearance
	EC_TYPE_COMPONENT_MAX
}EcTypeComponent;

#define MSK_EC_TYPE_COMPONENT_SGNODE		(0x1<<EC_TYPE_COMPONENT_SGNODE)
#define MSK_EC_TYPE_COMPONENT_SHAPE			(0x1<<EC_TYPE_COMPONENT_SHAPE)
#define MSK_EC_TYPE_COMPONENT_CAMERA		(0x1<<EC_TYPE_COMPONENT_CAMERA)




#endif

