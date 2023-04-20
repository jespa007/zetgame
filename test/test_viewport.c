#include "zetgame.h"

int main(int argc, char *argv[]){
	UNUSUED_PARAM(argc);
	UNUSUED_PARAM(argv);

	ZetGame_Init(NULL);
	//ViewPort *v1;
	//TextureManager_SetTextureResourcePath("data/images");

	Texture * text_png = Texture_NewFromFile("data/images/test.png");
	TextBox *textbox	=Textbox_New();
	Transform transform=Transform_DefaultValues();

	do{
		Graphics_BeginRender();

		int n_view_port_row=2;
		int w=ZG_DEFAULT_SCREEN_WIDTH/n_view_port_row;
		int h =ZG_DEFAULT_SCREEN_HEIGHT/n_view_port_row;


		for(int i=0; i < n_view_port_row*n_view_port_row; i++){

			int x=(i%n_view_port_row)*w;
			int y=(i/n_view_port_row)*h;

			ViewPort_Push(x,y,w,h,PROJECTION_MODE_ORTHO);

			Transform_SetPosition2i(&transform,10,10);

			Graphics_ClearScreen(Color4f_FromRGB(32+i*10,32+i*10,32+i*10));
			Graphics_DrawRectangleTextured4i(0,0,100,100,Color4f_FromRGB(0,200,0),text_png,NULL);
			Graphics_DrawRectangleFilled4i(0,0,10,10,COLOR4F_WHITE);
			Textbox_Draw(textbox,&transform,NULL);

			ViewPort_Pop();
		}


		Graphics_EndRender();

		Input_Update();
	}while(!K_ESC);

	Texture_Delete(text_png);
	Textbox_Delete(textbox);

	ZetGame_DeInit();

#ifdef __MEMMANAGER__
	MEMMGR_print_status();
#endif

	return 0;
}
