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



	TextureManager * texture_manager= TextureManager_New();
	TTFontManager * ttfont_manager= TTFontManager_New();

	TTFontManager_SetFontResourcePath(ttfont_manager,"../../../test/data/fonts");
	TextureManager_SetTextureResourcePath(texture_manager,"../../../test/data/images");


	GUIWindowManager *window_manager= GUIWindowManager_New(texture_manager,ttfont_manager);
	GUIWindow * window=NULL;

	if(GUIWindowManager_Load(
			window_manager
			,"../../../test/data/windows/"WINDOW_NAME".xml"
		)){

		if((window=GUIWindowManager_Get(window_manager,WINDOW_NAME))==NULL) {
			Log_Error("Cannot get window '%s'",WINDOW_NAME);
		}
	}


	//window=GUIWindow_New(10,10,200,100);
	//GUIViewer * gui_viewer=GUIViewer_New(10,10,42,42);

	if(window){

		//Textbox_SetText(gui_viewer->widget->textbox,"1");
		//GUIWidget_AttachWidget(window->widget,gui_viewer->widget);
		do{
			Graphics_BeginRender();


			GUIWidget_Update(window->widget);

			Graphics_EndRender();

			Input_Update();

		}while(!K_ESC);
	}

	//GUIViewer_Delete(gui_viewer);
	//GUIWindow_Delete(window);
	TextureManager_Delete(texture_manager);
	TTFontManager_Delete(ttfont_manager);
	GUIWindowManager_Delete(window_manager);

	ZetGame_DeInit();

	return 0;
}
