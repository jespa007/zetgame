#include "zetgame.h"

#define TEXT_WIDTH_BB

#define WIDTH_RES	640
#define HEIGHT_RES	480

#ifdef TEXT_WIDTH_BB
#define WIDTH_TEXT	150
#define HEIGHT_TEXT	100
#else
#define WIDTH_TEXT	0
#define HEIGHT_TEXT	0
#endif

typedef struct{
	const char *text;
	int x,y;
	int w,h;
	TextAlign text_align;
	VerticalAlign vertical_align;

}PrintFontInfo;


const PrintFontInfo print_font_info[]={
		 {"hello world<br>top-left"			,0,0,WIDTH_TEXT,HEIGHT_TEXT,TEXT_ALIGN_LEFT,VERTICAL_ALIGN_TOP}
		,{"hello world<br>center-left"		,0,(HEIGHT_RES>>1)-(HEIGHT_TEXT>>1),WIDTH_TEXT,HEIGHT_TEXT,TEXT_ALIGN_LEFT,VERTICAL_ALIGN_CENTER}
		,{"hello world!<br>bottom-left"		,0,HEIGHT_RES-HEIGHT_TEXT,WIDTH_TEXT,HEIGHT_TEXT,TEXT_ALIGN_LEFT,VERTICAL_ALIGN_BOTTOM}
		,{"hello world<br>top-right"		,WIDTH_RES-WIDTH_TEXT,0,WIDTH_TEXT,HEIGHT_TEXT,TEXT_ALIGN_RIGHT,VERTICAL_ALIGN_TOP}
		,{"hello world<br>center-right"		,WIDTH_RES-WIDTH_TEXT,(HEIGHT_RES>>1)-(HEIGHT_TEXT>>1),WIDTH_TEXT,HEIGHT_TEXT,TEXT_ALIGN_RIGHT,VERTICAL_ALIGN_CENTER}
		,{"hello world<br>bottom-right"		,WIDTH_RES-WIDTH_TEXT,HEIGHT_RES-HEIGHT_TEXT,WIDTH_TEXT,HEIGHT_TEXT,TEXT_ALIGN_RIGHT,VERTICAL_ALIGN_BOTTOM}
		,{"hello world<br>top-center"		,(WIDTH_RES>>1)-(WIDTH_TEXT>>1),0,WIDTH_TEXT,HEIGHT_TEXT,TEXT_ALIGN_CENTER,VERTICAL_ALIGN_TOP}
		,{"hello world<br>center-center"	,(WIDTH_RES>>1)-(WIDTH_TEXT>>1),(HEIGHT_RES>>1)-(HEIGHT_TEXT>>1),WIDTH_TEXT,HEIGHT_TEXT,TEXT_ALIGN_CENTER,VERTICAL_ALIGN_CENTER}
		,{"hello world<br>bottom-center"	,(WIDTH_RES>>1)-(WIDTH_TEXT>>1),HEIGHT_RES-HEIGHT_TEXT,WIDTH_TEXT,HEIGHT_TEXT,TEXT_ALIGN_CENTER,VERTICAL_ALIGN_BOTTOM}
		,{NULL}
};

int main(int argc, char * argv[]){
	UNUSUED_PARAM(argc);
	UNUSUED_PARAM(argv);

	ZetGameSetupParams setup_options;
	memset(&setup_options,0,sizeof(ZetGameSetupParams));


	setup_options.width=WIDTH_RES;
	setup_options.height=HEIGHT_RES;

	ZetGame_Init(&setup_options);


	TextBox *textbox=TextBox_New();
	TextBox_SetFontFile(textbox,"../../../test/data/fonts/Trebuchet MS.ttf");
	Transform transform=Transform_New();
	TextBox_SetText(textbox,"H L");
	TextBox_SetBorderThickness(textbox,1);
	TextBox_SetBorderColor4f(textbox,Color4f_New(1,1,1,1));
	do
	{
		Graphics_BeginRender();

		PrintFontInfo *aux_ptr=(PrintFontInfo *)print_font_info;
		while(aux_ptr->text !=NULL){
			//TextBox_SetPosition2i(textbox,aux_ptr->x+(aux_ptr->w>>1),aux_ptr->y+(aux_ptr->h>>1));
			TextBox_SetDimensions(textbox,aux_ptr->w,aux_ptr->h);
			TextBox_SetText(textbox,aux_ptr->text);
			TextBox_SetTextAlign(textbox,aux_ptr->text_align);
			TextBox_SetVerticalAlign(textbox,aux_ptr->vertical_align);

			Transform_SetPosition2i(&transform,aux_ptr->x+(aux_ptr->w>>1),aux_ptr->y+(aux_ptr->h>>1));
			TextBox_Draw(textbox,&transform,NULL);

			aux_ptr++;
		}

		Graphics_EndRender();

		Input_Update();

	}while(!K_ESC);

	//Transform_Delete(transform);

	TextBox_Delete(textbox);


	ZetGame_DeInit();

	return 0;
}
