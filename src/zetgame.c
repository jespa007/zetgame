#include "zetgame.h"

static ZG_SetupParams default_setup={
		.graphics_api=ZG_GRAPHICS_API_GL
		,.width=ZG_DEFAULT_SCREEN_WIDTH
		,.height=ZG_DEFAULT_SCREEN_HEIGHT
		,.wcaption_title="zetgame"
		,.wposx=SDL_WINDOWPOS_CENTERED
		,.wposy=80
		,.graphic_properties=0
};

static bool debug_mode=false;


void ZG_SetupInfo_SetupInfo(ZG_SetupParams *si){
	si->width=ZG_DEFAULT_SCREEN_WIDTH;
	si->height=ZG_DEFAULT_SCREEN_HEIGHT;
}

void ZG_Init(ZG_SetupParams * info){

	if(info == NULL){
		info=&default_setup;
	}

	//setlocale(LC_ALL, ""); // set utf-8 by default

	ZG_Luts_Init();

#ifdef __WITH_FFMPEG__
	av_register_all();
#endif
	ZN_Init();

    if (!ZG_VideoDevice_Init()) {
    	exit(EXIT_FAILURE);
    }

	//ZG_Mixer_Init();

	if(!ZG_Graphics_Init(
			 info->wposx
			,info->wposy
			,info->width
			,info->height
//			,info->physical_width
//			,info->physical_height
			,info->graphics_api
			,info->wcaption_title
			,info->graphic_properties
			)){
		exit(EXIT_FAILURE);
	}

	ZG_Input_Init();
	/*if(!ZG_ECS_Init()){
		exit(EXIT_FAILURE);
	}*/
	//SGRender_Init();
	ZG_GUI_Init();
}

void ZG_SetDebugMode(bool _debug_mode){
	debug_mode=_debug_mode;
}


bool ZG_IsDebugMode(void){
	return debug_mode;
}

void ZG_DeInit(void){

	ZG_GUI_DeInit();
	//SGRender_DeInit();

	//ZG_Mixer_DeInit();
	ZG_Input_DeInit();
	ZG_Graphics_DeInit();
	//ZG_ECS_DeInit();

	ZN_DeInit();
}


