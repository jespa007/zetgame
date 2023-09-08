#ifndef __ZETGAME_H__
#define __ZETGAME_H__

#include "base/@zg_base.h"
#include "mmedia/@zg_mmedia.h"
#include "math/@zg_math.h"
#include "physics/@zg_physics.h"
#include "input/@zg_input.h"
#include "graphics/@zg_graphics.h"
//#include "scene/zg_scene.h"
#include "gui/@zg_gui.h"
#include "sprite/@zg_sprite.h"
#include "tilemap/@zg_tilemap.h"
#include "ecs/@zg_ecs.h"


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
