#include "zetgame.h"

int main(int argc, char *argv[]){
	ZG_UNUSUED_PARAM(argc);
	ZG_UNUSUED_PARAM(argv);

	ZG_Init(NULL);
	//ZG_ViewPort *v1;
	//ZG_TextureManager_SetTextureResourcePath("data/images");

	ZG_Texture * text_png = ZG_Texture_NewFromFile("data/images/test.png");
	ZG_TextBox *textbox	=ZG_TextBox_New();
	ZG_Transform transform=ZG_Transform_DefaultValues();

	do{
		ZG_Graphics_BeginRender();

		int n_view_port_row=2;
		int w=ZG_DEFAULT_SCREEN_WIDTH/n_view_port_row;
		int h =ZG_DEFAULT_SCREEN_HEIGHT/n_view_port_row;


		for(int i=0; i < n_view_port_row*n_view_port_row; i++){

			int x=(i%n_view_port_row)*w;
			int y=(i/n_view_port_row)*h;

			ZG_ViewPort_Push(x,y,w,h,ZG_PROJECTION_MODE_ORTHO);

			ZG_Transform_SetPosition2i(&transform,10,10);

			Graphics_ClearScreen(ZG_Color4f_FromRGB(32+i*10,32+i*10,32+i*10));
			ZG_Graphics_DrawRectangleTextured4i(0,0,100,100,ZG_Color4f_FromRGB(0,200,0),text_png,NULL);
			Graphics_DrawRectangleFilled4i(0,0,10,10,ZG_COLOR4F_WHITE);
			ZG_TextBox_Draw(textbox,&transform,NULL);

			ZG_ViewPort_Pop();
		}


		ZG_Graphics_EndRender();

		ZG_Input_Update();
	}while(!K_ESC);

	ZG_Texture_Delete(text_png);
	ZG_TextBox_Delete(textbox);

	ZG_DeInit();

	return 0;
}
