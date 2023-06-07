#include "zetgame.h"


int main(int argc, char *argv[]){

	ZG_UNUSUED_PARAM(argc);
	ZG_UNUSUED_PARAM(argv);

	ZG_Init(NULL);
	//ZG_TTFontManager_SetFontResourcePath("data/fonts");
	//ZG_TextureManager_SetTextureResourcePath("data/images");

	ZG_Texture * text_default = ZG_TextureManager_GetDefaultTexture();
	ZG_Texture * text_png = ZG_Texture_NewFromFile("../../../test/data/images/test.png");
	ZG_Texture * text_jpg = ZG_Texture_NewFromFile("../../../test/data/images/test.jpg");
	ZG_TTFont * font = ZG_TTFont_NewFromFile("../../../test/data/fonts/pf_arma_five.ttf");
	ZG_TTFont * font1 = ZG_TTFont_NewFromFile("../../../test/data/fonts/g5lis1b.ttf");
	ZG_TTFont * font2 = ZG_TTFont_NewFromFile("../../../test/data/fonts/Trebuchet MS.ttf");
	//Shape2d * shape2d = Shape2d_New();
	ZG_Geometry *geometry=ZG_Geometry_NewRectangleFilled(ZG_GEOMETRY_PROPERTY_TEXTURE);
	ZG_Appearance *appearance=ZG_Appearance_New();
	ZG_Transform transform=ZG_Transform_New();

	appearance->texture=text_png;

	Graphics_SetBackgroundColor(ZG_Color4f_FromHex(0xFFFF));

	const char * s="Test font";
	printf("size text %i\n",ZG_TTFont_GetWidth(font2,s));

	do{
		ZG_Graphics_BeginRender();

		Graphics_DrawRectangleFilled4i(10,10,100,100,ZG_Color4f_FromHex(0xFF));
		Graphics_DrawRectangleFilled4i(20,20,100,100,ZG_Color4f_FromHex(0xFFFF));
		ZG_Graphics_DrawRectangleTextured4i(200,200,100,100,ZG_COLOR4F_WHITE,text_png,NULL);
		ZG_Graphics_DrawRectangleTextured4i(300,300,100,100,ZG_COLOR4F_WHITE,text_jpg,NULL);
		ZG_Graphics_DrawRectangleTextured4i(100,200,100,100,ZG_COLOR4F_WHITE,text_default,NULL);


		Graphics_Print(50,50,ZG_COLOR4F_WHITE,"%s %i",s,ZG_TTFont_GetWidth(font2,s));

		//Shape_SetAppearance(v2d->shape2d->shape,appearance); // 10000 sprites at 4FPS
		//ZG_Appearance_Apply(appearance);// 10000 sprites at 20FPS

		// test max draw sprites ...
		for(int i=0; i<10;i++){
			ZG_Transform_SetPosition2i(&transform,rand()%ZG_Graphics_GetWidth(),rand()%ZG_Graphics_GetHeight());
			Graphics_Draw(&transform,geometry,NULL);
		}

		ZG_Graphics_EndRender();

		Input_Update();
	}while(!K_ESC);
	//ZG_Transform_Delete(transform);
//	Shape2d_Delete(shape2d);
	ZG_Geometry_Delete(geometry);

	ZG_Texture_Delete(text_png);
	ZG_Texture_Delete(text_jpg);


	ZG_TTFont_Delete(font);
	ZG_TTFont_Delete(font1);
	ZG_TTFont_Delete(font2);
	//ZG_TTFont_Delete(font);
	ZG_Appearance_Delete(appearance);


	ZetGame_DeInit();

	return 0;
}
