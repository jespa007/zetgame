#include "ZetGame.h"


int main(int argc, char *argv[]){

	UNUSUED_PARAM(argc);
	UNUSUED_PARAM(argv);

	ZetGame_Init(NULL);
	//TTFontManager_SetFontResourcePath("data/fonts");
	//TextureManager_SetTextureResourcePath("data/images");

	Texture * text_default = TextureManager_GetDefaultTexture();
	Texture * text_png = Texture_NewFromFile("data/imagestest.png");
	Texture * text_jpg = Texture_NewFromFile("data/imagestest.jpg");
	TTFont * font = TTFont_NewFromFile("data/fonts/pf_arma_five.ttf",16);
	TTFont * font1 = TTFont_NewFromFile("data/fonts/g5lis1b.ttf",36);
	TTFont * font2 = TTFont_NewFromFile("data/fonts/Trebuchet MS.ttf",36);
	//Shape2d * shape2d = Shape2d_New();
	Geometry *geometry=Geometry_NewRectangleFilled(GEOMETRY_PROPERTY_TEXTURE);
	Appearance *appearance=Appearance_New();
	Transform transform=Transform_New();

	appearance->texture=text_png;

	Graphics_SetBackgroundColor(Color4f_FromHex(0xFFFF));

	const char * s="Test font";
	printf("size text %i\n",TTFont_GetWidth(font2,s));

	do{
		Graphics_BeginRender();

		Graphics_DrawRectangleFilledTranslate2i(10,10,100,100,Color4f_FromHex(0xFF));
		Graphics_DrawRectangleFilledTranslate2i(20,20,100,100,Color4f_FromHex(0xFFFF));
		Graphics_DrawRectangleTexturedTranslate2i(200,200,100,100,COLOR4F_WHITE,text_png,NULL);
		Graphics_DrawRectangleTexturedTranslate2i(300,300,100,100,COLOR4F_WHITE,text_jpg,NULL);
		Graphics_DrawRectangleTexturedTranslate2i(100,200,100,100,COLOR4F_WHITE,text_default,NULL);


		Graphics_Print(50,50,COLOR4F_WHITE,"%s %i",s,TTFont_GetWidth(font2,s));

		//Shape_SetAppearance(v2d->shape2d->shape,appearance); // 10000 sprites at 4FPS
		//Appearance_Apply(appearance);// 10000 sprites at 20FPS

		// test max draw sprites ...
		for(int i=0; i<10;i++){
			Transform_SetPosition2i(&transform,rand()%Graphics_GetWidth(),rand()%Graphics_GetHeight());
			Transform_Apply(&transform);
			Graphics_Draw(&transform,geometry,NULL);
			Transform_Restore(&transform);
		}

		Graphics_EndRender();

		Input_Update();
	}while(!K_ESC);
	//Transform_Delete(transform);
//	Shape2d_Delete(shape2d);
	Geometry_Delete(geometry);

	Texture_Delete(text_png);
	Texture_Delete(text_jpg);


	TTFont_Delete(font);
	TTFont_Delete(font1);
	TTFont_Delete(font2);
	//TTFont_Delete(font);
	Appearance_Delete(appearance);


	ZetGame_DeInit();

#ifdef __MEMMANAGER__
	MEMMGR_print_status();
#endif

	return 0;
}
