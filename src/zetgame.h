#ifndef __ZETGAME_H__
#define __ZETGAME_H__

#include "base/_zg_base_.h"
#include "mmedia/_zg_mmedia_.h"
#include "math/_zg_math_.h"
#include "physics/_zg_physics_.h"
#include "input/_zg_input_.h"
#include "graphics/_zg_graphics_.h"
//#include "scene/zg_scene.h"
#include "gui/_zg_gui_.h"
#include "sprite/_zg_sprite_.h"
#include "tilemap/_zg_tilemap_.h"
#include "ecs/_zg_ecs_.h"


#define ZETGAME_VERSION_MAJOR	1
#define ZETGAME_VERSION_MINOR	0
#define ZETGAME_VERSION_PATCH	0

#define ZG_DEFAULT_SCREEN_WIDTH		640
#define ZG_DEFAULT_SCREEN_HEIGHT	480


typedef struct ZG_SetupParams ZG_SetupParams;

struct ZG_SetupParams{

	ZG_GraphicsApi	graphics_api;
	uint16_t width;
	uint16_t height;
	const char * wcaption_title;
	int wposx,wposy;
	uint16_t graphic_properties;

};


void ZG_Init(ZG_SetupParams * info);
void ZG_SetDebugMode(bool _debug_mode);
bool ZG_IsDebugMode(void);


void ZG_DeInit(void);



#endif
