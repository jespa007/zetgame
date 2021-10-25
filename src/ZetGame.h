#ifndef __ZETGINE_CORE_H__
#define __ZETGINE_CORE_H__

#include "base/zg_base.h"
#include "mmedia/zg_mmedia.h"
#include "math/zg_math.h"
#include "physics/zg_physics.h"
#include "input/zg_input.h"
#include "graphics/zg_graphics.h"
#include "scene/zg_scene.h"
#include "gui/zg_gui.h"
#include "sprite/zg_sprite.h"
#include "tilemap/zg_tilemap.h"


#define ZETGAME_MAJOR_VERSION	1
#define ZETGAME_MINOR_VERSION	0
#define ZETGAME_PATCH_VERSION	0

#define ZG_DEFAULT_SCREEN_WIDTH		640
#define ZG_DEFAULT_SCREEN_HEIGHT	480


typedef struct ZetGameSetupParams ZetGameSetupParams;

struct ZetGameSetupParams{

	GraphicsApi	graphics_api;
	uint16_t width;
	uint16_t height;
	const char * wcaption_title;
	int wposx,wposy;
	uint16_t graphic_properties;

};


void ZetGame_Init(ZetGameSetupParams * info);
void ZetGame_SetDebugMode(bool _debug_mode);
bool ZetGame_IsDebugMode(void);


void ZetGame_DeInit(void);



#endif
