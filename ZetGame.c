#include "ZetGame.h"

static ZetgineCoreSetupParams default_setup={
		.graphics_api=GRAPHICS_API_GL
		,.width=ZG_DEFAULT_SCREEN_WIDTH
		,.height=ZG_DEFAULT_SCREEN_HEIGHT
		,.wcaption_title="Zetgine"
		,.wposx=SDL_WINDOWPOS_CENTERED
		,.wposy=80
		,.graphic_properties=0
};

static bool debug_mode=false;


void SetupInfo_SetupInfo(ZetgineCoreSetupParams *si){
	si->width=ZG_DEFAULT_SCREEN_WIDTH;
	si->height=ZG_DEFAULT_SCREEN_HEIGHT;
}

void ZetgineCore_Init(ZetgineCoreSetupParams * info){

	if(info == NULL){
		info=&default_setup;
	}

	//setlocale(LC_ALL, ""); // set utf-8 by default

	Luts_Init();

#ifdef __WITH_FFMPEG__
	av_register_all();
#endif
	ZetNet_Init();

	Mixer_Init(AUDIO_FORMAT_INT_16);

	if(!Graphics_Init(
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

	Input_Init();
	//SGRender_Init();
	GUI_Init();
}

void ZetgineCore_SetDebugMode(bool _debug_mode){
	debug_mode=_debug_mode;
}


bool ZetgineCore_IsDebugMode(void){
	return debug_mode;
}

void ZetgineCore_DeInit(void){

	GUI_DeInit();
	//SGRender_DeInit();

	Mixer_DeInit();
	Input_DeInit();
	Graphics_DeInit();

	SDL_DestroyResources();

	ZetNet_DeInit();
}


