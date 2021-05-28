#ifndef __ZETGINE_CORE_H__
#define __ZETGINE_CORE_H__

#include "system/zg_system.h"
#include "logger/zg_logger.h"
#include "mmedia/zg_mmedia.h"
#include "math/zg_math.h"
#include "physics/zg_physics.h"
#include "input/zg_input.h"
#include "graphics/zg_graphics.h"
#include "scene/zg_scene.h"
#include "gui/zg_gui.h"
//#include "mmedia/zg_mmedia.h"
/*
#include "scene/zg_scene.h"
#include "ui/zg_ui.h"
#include "stage/zg_stage.h

"*/

#define ZG_CORE_MAJOR_VERSION	1
#define ZG_CORE_MINOR_VERSION	0
#define ZG_CORE_PATCH_VERSION	0

#define ZG_DEFAULT_SCREEN_WIDTH		640
#define ZG_DEFAULT_SCREEN_HEIGHT	480


typedef struct ZetgineCoreSetupParams ZetgineCoreSetupParams;

struct ZetgineCoreSetupParams{

	GraphicsApi	graphics_api;
	uint16_t width;
	uint16_t height;
	const char * wcaption_title;
	int wposx,wposy;
	uint16_t graphic_properties;

};


void ZetgineCore_Init(ZetgineCoreSetupParams * info);
void ZetgineCore_SetDebugMode(bool _debug_mode);
bool ZetgineCore_IsDebugMode(void);


void ZetgineCore_DeInit(void);



#endif
