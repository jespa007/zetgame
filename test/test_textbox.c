#include "zetgame.h"

#define TEXT_WIDTH_BB

#define WIDTH_RESOLUTION	640
#define HEIGHT_RESOLUTION	480

#ifdef TEXT_WIDTH_BB
#define TEXTBOX_WIDTH	150
#define TEXTBOX_HEIGHT	100
#else
#define TEXTBOX_WIDTH	0
#define TEXTBOX_HEIGHT	0
#endif

typedef struct{
	const char *text;
	int x,y;
	int w,h;
	HorizontalAlignment horizontal_alignment;
	VerticalAlignment vertical_alignment;

}PrintFontInfo;


const PrintFontInfo print_font_info[]={
		 {"hello world<br>top-left"			,0,0,TEXTBOX_WIDTH,TEXTBOX_HEIGHT,HORIZONTAL_ALIGNMENT_LEFT,VERTICAL_ALIGNMENT_TOP}
		,{"hello world<br>center-left"		,0,(HEIGHT_RESOLUTION>>1)-(TEXTBOX_HEIGHT>>1),TEXTBOX_WIDTH,TEXTBOX_HEIGHT,HORIZONTAL_ALIGNMENT_LEFT,VERTICAL_ALIGNMENT_CENTER}
		,{"hello world!<br>bottom-left"		,0,HEIGHT_RESOLUTION-TEXTBOX_HEIGHT,TEXTBOX_WIDTH,TEXTBOX_HEIGHT,HORIZONTAL_ALIGNMENT_LEFT,VERTICAL_ALIGNMENT_BOTTOM}
		,{"hello world<br>top-right"		,WIDTH_RESOLUTION-TEXTBOX_WIDTH,0,TEXTBOX_WIDTH,TEXTBOX_HEIGHT,HORIZONTAL_ALIGNMENT_RIGHT,VERTICAL_ALIGNMENT_TOP}
		,{"hello world<br>center-right"		,WIDTH_RESOLUTION-TEXTBOX_WIDTH,(HEIGHT_RESOLUTION>>1)-(TEXTBOX_HEIGHT>>1),TEXTBOX_WIDTH,TEXTBOX_HEIGHT,HORIZONTAL_ALIGNMENT_RIGHT,VERTICAL_ALIGNMENT_CENTER}
		,{"hello world<br>bottom-right"		,WIDTH_RESOLUTION-TEXTBOX_WIDTH,HEIGHT_RESOLUTION-TEXTBOX_HEIGHT,TEXTBOX_WIDTH,TEXTBOX_HEIGHT,HORIZONTAL_ALIGNMENT_RIGHT,VERTICAL_ALIGNMENT_BOTTOM}
		,{"hello world<br>top-center"		,(WIDTH_RESOLUTION>>1)-(TEXTBOX_WIDTH>>1),0,TEXTBOX_WIDTH,TEXTBOX_HEIGHT,HORIZONTAL_ALIGNMENT_CENTER,VERTICAL_ALIGNMENT_TOP}
		,{"hello world<br>center-center"	,(WIDTH_RESOLUTION>>1)-(TEXTBOX_WIDTH>>1),(HEIGHT_RESOLUTION>>1)-(TEXTBOX_HEIGHT>>1),TEXTBOX_WIDTH,TEXTBOX_HEIGHT,HORIZONTAL_ALIGNMENT_CENTER,VERTICAL_ALIGNMENT_CENTER}
		,{"hello world<br>bottom-center"	,(WIDTH_RESOLUTION>>1)-(TEXTBOX_WIDTH>>1),HEIGHT_RESOLUTION-TEXTBOX_HEIGHT,TEXTBOX_WIDTH,TEXTBOX_HEIGHT,HORIZONTAL_ALIGNMENT_CENTER,VERTICAL_ALIGNMENT_BOTTOM}
		,{NULL}
};

int main(int argc, char * argv[]){
	UNUSUED_PARAM(argc);
	UNUSUED_PARAM(argv);

	ZetGameSetupParams setup_options;
	memset(&setup_options,0,sizeof(ZetGameSetupParams));

	setup_options.width=WIDTH_RESOLUTION;
	setup_options.height=HEIGHT_RESOLUTION;

	ZetGame_Init(&setup_options);
	ZetGame_SetDebugMode(true);

	TextBox *textbox=TextBox_New();
	TextBox_SetFontFile(textbox,"../../../test/data/fonts/Trebuchet MS.ttf");
	Transform transform=Transform_New();
	TextBox_SetText(textbox,"H L");
	TextBox_SetBorderThickness(textbox,1);
	TextBox_SetBorderColor4f(textbox,Color4f_New3f(1,1,1));
	do{

		Graphics_BeginRender();

		PrintFontInfo *aux_ptr=(PrintFontInfo *)print_font_info;
		while(aux_ptr->text !=NULL){
			//TextBox_SetPosition2i(textbox,aux_ptr->x+(aux_ptr->w>>1),aux_ptr->y+(aux_ptr->h>>1));
			TextBox_SetDimensions(textbox,aux_ptr->w,aux_ptr->h);
			TextBox_SetText(textbox,aux_ptr->text);
			TextBox_SetHorizontalAlignment(textbox,aux_ptr->horizontal_alignment);
			TextBox_SetVerticalAlignment(textbox,aux_ptr->vertical_alignment);

			Transform_SetPosition2i(&transform,aux_ptr->x+(aux_ptr->w>>1),aux_ptr->y+(aux_ptr->h>>1));
			TextBox_Draw(textbox,&transform,NULL);

			aux_ptr++;
		}

		//Draw blue rectangle
		//Graphics_DrawRectangle4i(10,10,20,20,Color4f_New4f(0,1,0,1),2);

		if(K_T){
			Graphics_ToggleFullscreen();
		}

		Graphics_EndRender();

		Input_Update();

	}while(!K_ESC);

	//Transform_Delete(transform);

	TextBox_Delete(textbox);


	ZetGame_DeInit();

	return 0;
}
