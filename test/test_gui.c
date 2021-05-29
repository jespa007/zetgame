#include "../ZetGame.h"

int main(int argc, char *argv[]){

	ZetGame_Init(NULL);

	GUIWindow * window=GUIWindow_New(10,10,200,100);
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
