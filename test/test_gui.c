#include "zetgame.h"

#define WINDOW_NAME "window"

int main(int argc, char *argv[]){
	UNUSUED_PARAM(argc);
	UNUSUED_PARAM(argv);

	ZetGame_Init(NULL);


	TextureManager * texture_manager= TextureManager_New();
	GUIWindowManager *window_manager= GUIWindowManager_New(texture_manager);
	GUIWindow * window=NULL;

	if(GUIWindowManager_Load(
			window_manager
			,"../../../test/data/windows/"WINDOW_NAME".xml"
		)){

		if((window=GUIWindowManager_Get(window_manager,WINDOW_NAME))==NULL) {
			Log_Error("Cannot get window '%s'",WINDOW_NAME);
		}
	}


	window=GUIWindow_New(10,10,200,100);
	GUIViewer * gui_viewer=GUIViewer_New(10,10,42,42);

	//Textbox_SetText(gui_viewer->widget->textbox,"1");
	//GUIWidget_AttachWidget(window->widget,gui_viewer->widget);
	do{
		Graphics_BeginRender();


		GUIWidget_Update(window->widget);

		Graphics_EndRender();

		Input_Update();

	}while(!K_ESC);

	GUIViewer_Delete(gui_viewer);
	GUIWindow_Delete(window);
	TextureManager_Delete(texture_manager);
	GUIWindowManager_Delete(window_manager);

	ZetGame_DeInit();

	return 0;
}
