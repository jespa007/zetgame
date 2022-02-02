#include "ZetGame.h"

#define WINDOW_NAME "window"

int main(int argc, char *argv[]){
	UNUSUED_PARAM(argc);
	UNUSUED_PARAM(argv);

	ZetGame_Init(NULL);


	TextureManager * texture_manager= TextureManager_New();
	GUIWindowManager *window_manager=GUIWindowManager_New(texture_manager);
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
	GUIViewer * texture=GUIViewer_New(10,10,42,42);

	Textbox_SetText(texture->widget->textbox,"1");
	GUIWidget_AttachWidget(window->widget,texture->widget);
	do{
		Graphics_BeginRender();


		GUIWidget_Update(window->widget);

		Graphics_EndRender();

		Input_Update();

	}while(!K_ESC);

	GUIViewer_Delete(texture);

	GUIWindow_Delete(window);

	ZetGame_DeInit();

#ifdef __MEMMANAGER__
	MEMMGR_print_status();
#endif

	return 0;
}
