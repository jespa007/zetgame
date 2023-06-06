#include "zetgame.h"

#define SHAPE_WIDTH			50
#define SHAPER_COLOR		0xff707070
#define SHAPE_BORDER_COLOR	0xffe0e0e0

int main(int argc, char *argv[]){
	UNUSUED_PARAM(argc);
	UNUSUED_PARAM(argv);

	struct{
		uint32_t width, fill_color, border_width,border_color;
	}circle_info[2]={
			{SHAPE_WIDTH>>1,SHAPER_COLOR,0,0}
			,{SHAPE_WIDTH>>1,SHAPER_COLOR,SHAPE_WIDTH*0.1,SHAPE_BORDER_COLOR}
	};

	ZetGame_Init(NULL);

	ZG_List *textures= List_New();

	// circle with without border...
	// foreach image create a texture...
	for(unsigned i=0; i < ARRAY_SIZE(circle_info);i++){
		Texture *texture=NULL;
		SDL_Surface *srf= SDL_NewCircle(
				circle_info[i].width,
				circle_info[i].fill_color,
				circle_info[i].border_width,
				circle_info[i].border_color
		);
		List_Add(textures,texture=Texture_NewFromSurface(srf));

		SDL_FreeSurface(srf);
	}

	Graphics_SetBackgroundColor(Color4f_FromHex(0xFF));

	Geometry *geometry=Geometry_NewRectangleFilled(GEOMETRY_PROPERTY_TEXTURE);
	Appearance *appearance=Appearance_New();

	do{
		Graphics_BeginRender();

		int x=10,y=10;

		//Graphics_SetProjectionMode(PROJECTION_MODE_ORTHO);

		for(unsigned i=0; i < textures->count; i++){

			appearance->texture=textures->items[i];

			// circle
			Graphics_DrawRectangleTextured4i(x,y,SHAPE_WIDTH,SHAPE_WIDTH,COLOR4F_WHITE,textures->items[i],NULL);
			x+=SHAPE_WIDTH+10;

			if((i%2==0) && i > 0){
				y+=SHAPE_WIDTH+10;
				x=10;
			}
		}

		Graphics_EndRender();

		Input_Update();
	}while(!K_ESC);

	// unload all textures...
	for(unsigned i=0; i < textures->count; i++){
		Texture_Delete(textures->items[i]);
	}

	Geometry_Delete(geometry);
	Appearance_Delete(appearance);

	List_Delete(textures);

	ZetGame_DeInit();

	return 0;
}
