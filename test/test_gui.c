#include "zetgame.h"

#define WINDOW_NAME "window"

int main(int argc, char *argv[]){
	ZG_UNUSUED_PARAM(argc);
	ZG_UNUSUED_PARAM(argv);
	ZG_SetupParams params;
	memset(&params,0,sizeof(params));
	params.width=1024;
	params.height=768;

	ZG_Init(&params);
	ZG_SetDebugMode(true);

	ZG_TextureManager * texture_manager= ZG_TextureManager_New();
	ZG_TTFontManager * ttfont_manager= ZG_TTFontManager_New();


	ZG_TTFontManager_SetFontResourcePath(ttfont_manager,"../../../test/data/fonts");
	ZG_TextureManager_SetTextureResourcePath(texture_manager,"../../../test/data/images");



	ZG_GUIWindow * window=NULL;

	if((window=ZG_GUIWindow_NewFromFile(
		"../../../test/data/windows/"WINDOW_NAME".xml"
		,texture_manager
		,ttfont_manager
	))==NULL){
		ZG_LOG_ERROR("Cannot get window '%s'",WINDOW_NAME);
	}

	if(window){

		//ZG_TextBox_SetText(gui_viewer->widget->textbox,"1");
		//ZG_GUIWidget_AttachWidget(window->widget,gui_viewer->widget);
		do{
			// input management

			// toggle fullscreen
			if(ZG_KP_F9){
				ZG_Graphics_ToggleFullscreen();
			}

			// screen shoot
			if(ZG_KP_F12){
				ZG_Graphics_MakeScreenShot();
			}

			ZG_Graphics_BeginRender();

			ZG_GUIWidget_Update(window->widget);

			ZG_Graphics_EndRender();

			ZG_Input_Update();

		}while(!ZG_KP_ESC);
	}

	ZG_TextureManager_Delete(texture_manager);
	ZG_TTFontManager_Delete(ttfont_manager);
	ZG_GUIWindow_Delete(window);

	ZG_DeInit();

	return 0;
}
