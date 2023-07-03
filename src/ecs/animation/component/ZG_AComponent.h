#ifndef _ZG_ACOMPONENT_H__
#define _ZG_ACOMPONENT_H__

typedef struct ZG_ACTransformAnimation ZG_ACTransformAnimation;
typedef struct ZG_ACMaterialAnimation ZG_ACMaterialAnimation;




typedef struct{
	ZG_Entity		entity; // if NULL -> no entity associated -> inactive
}ZG_AComponentHeader;



/*
int EntityComponent_Register(const char *_name);
int EntityComponent_Max();
int EntityComponent_DeInit();
*/
#endif

