#include "../ZetGame.h"


int main(int argc, char *argv[]){

	ZetgineCore_Init(NULL);
	TTFont_SetFontResourcePath("data/fonts");
	Texture_SetTextureResourcePath("data/images");

	Texture * text_default = Texture_GetDefault();
	Texture * text_png = Texture_LoadFromFile("test.png");
	Texture * text_jpg = Texture_LoadFromFile("test.jpg");
	TTFont * font = TTFont_GetFontFromName("pf_arma_five.ttf",16);
	TTFont * font1 = TTFont_GetFontFromName("g5lis1b.ttf",36);
	TTFont * font2 = TTFont_GetFontFromName("Trebuchet MS.ttf",36);
	//Shape2d * shape2d = Shape2d_New();
	Geometry *geometry=Geometry_NewQuad(GEOMETRY_TEXTURE);
	Appearance *appearance=Appearance_New();
	Transform *transform=Transform_New();

	appearance->texture=text_png;

	Graphics_SetBackgroundColor(Color4f_FromHex(0xFFFF));

	const char * s="Test font";
	printf("size text %i\n",TTFont_GetWidth(font2,s));

	do{
		Graphics_BeginRender();

		Graphics_DrawRectangleFilled(10,10,100,100,Color4f_FromHex(0xFF));
		Graphics_DrawRectangleFilled(20,20,100,100,Color4f_FromHex(0xFFFF));
		Graphics_DrawRectangleTextured(200,200,100,100,COLOR4F_WHITE,text_png,NULL);
		Graphics_DrawRectangleTextured(300,300,100,100,COLOR4F_WHITE,text_jpg,NULL);
		Graphics_DrawRectangleTextured(100,200,100,100,COLOR4F_WHITE,text_default,NULL);


		Graphics_Print(50,50,COLOR4F_WHITE,"%s %i",s,TTFont_GetWidth(font2,s));

		//Shape_SetAppearance(v2d->shape2d->shape,appearance); // 10000 sprites at 4FPS
		//Appearance_Apply(appearance);// 10000 sprites at 20FPS

		// test max draw sprites ...
		for(int i=0; i<10;i++){
			Transform_SetPosition2i(transform,rand()%Graphics_GetWidth(),rand()%Graphics_GetHeight());
			Transform_Apply(transform);
			Graphics_Draw(transform,geometry,NULL);
			//Shape_DrawRectangle(0.1f,0.1f,false);
			Transform_Restore(transform);
		}

		//Appearance_Restore(appearance);

		Graphics_EndRender();

		Input_Update();
	}while(!K_ESC);
	Transform_Delete(transform);
//	Shape2d_Delete(shape2d);
	Geometry_Delete(geometry);

	Texture_Delete(text_png);
	Texture_Delete(text_jpg);
	//TTFont_Delete(font);
	Appearance_Delete(appearance);


	ZetgineCore_DeInit();

#ifdef __MEMMANAGER__
	MEMMGR_print_status();
#endif

	return 0;
}
