#ifndef _ACOMPONENT_H__
#define _ACOMPONENT_H__

typedef struct ACTransformAnimation ACTransformAnimation;
typedef struct ACMaterialAnimation ACMaterialAnimation;

// Builtin components
typedef enum{
	AC_INVALID=0,			// reserver for invalid component
	AC_TRANSFORM_ANIMATION,
	AC_MATERIAL_ANIMATION,
	// AC_TEXTURE_ANIMATION,
	// AC_GEOMETRY_ANIMATION,

	AC_MAX_COMPONENTS
}AComponent;

typedef struct{
	ComponentId	id;
	Entity		*entity; // if NULL -> no entity associated -> inactive
}AComponentHeader;




/*
int EntityComponent_Register(const char *_name);
int EntityComponent_Max();
int EntityComponent_DeInit();
*/
#endif

