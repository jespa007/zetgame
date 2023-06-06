#include "zetgame.h"

#define WINDOW_NAME "window"

int main(int argc, char *argv[]){
	UNUSUED_PARAM(argc);
	UNUSUED_PARAM(argv);
	ZetGameSetupParams params;
	memset(&params,0,sizeof(params));
	params.width=1024;
	params.height=768;

	ZetGame_Init(&params);
	ZetGame_SetDebugMode(true);

	ZG_TextureManager * texture_manager= ZG_TextureManager_New();
	//ZG_TTFontManager * ttfont_manager= ZG_TTFontManager_New();

	ZG_TTFont_SetFontResourcePath("../../../test/data/fonts");
	ZG_TextureManager_SetTextureResourcePath(texture_manager,"../../../test/data/images");


	GUIWindowManager *window_manager= GUIWindowManager_New(texture_manager);
	GUIWindow * window=NULL;

	if(GUIWindowManager_Load(
			window_manager
			,"../../../test/data/windows/"WINDOW_NAME".xml"
		)){

		if((window=GUIWindowManager_Get(window_manager,WINDOW_NAME))==NULL) {
			ZG_Log_Error("Cannot get window '%s'",WINDOW_NAME);
		}
	}


	//window=GUIWindow_New(10,10,200,100);
	//GUITexture * gui_viewer=GUITexture_New(10,10,42,42);

	if(window){

		//ZG_TextBox_SetText(gui_viewer->widget->textbox,"1");
		//GUIWidget_AttachWidget(window->widget,gui_viewer->widget);
		do{
			Graphics_BeginRender();


			GUIWidget_Update(window->widget);

			Graphics_EndRender();

			Input_Update();

		}while(!K_ESC);
	}

	//GUITexture_Delete(gui_viewer);
	//GUIWindow_Delete(window);
	ZG_TextureManager_Delete(texture_manager);
//	ZG_TTFontManager_Delete(ttfont_manager);
	GUIWindowManager_Delete(window_manager);

	ZetGame_DeInit();

	return 0;
}
