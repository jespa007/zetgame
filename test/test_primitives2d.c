#include "zetgame.h"

#define SHAPE_WIDTH			50
#define SHAPER_COLOR		0xff707070
#define SHAPE_BORDER_COLOR	0xffe0e0e0

int main(int argc, char *argv[]){
	ZG_UNUSUED_PARAM(argc);
	ZG_UNUSUED_PARAM(argv);

	struct{
		uint32_t width, fill_color, border_width,border_color;
	}circle_info[2]={
			{SHAPE_WIDTH>>1,SHAPER_COLOR,0,0}
			,{SHAPE_WIDTH>>1,SHAPER_COLOR,SHAPE_WIDTH*0.1,SHAPE_BORDER_COLOR}
	};

	ZG_Init(NULL);

	ZG_List *textures= ZG_List_New();

	// circle with without border...
	// foreach image create a texture...
	for(unsigned i=0; i < ZG_ARRAY_SIZE(circle_info);i++){
		ZG_Texture *texture=NULL;
		SDL_Surface *srf= SDL_NewCircle(
				circle_info[i].width,
				circle_info[i].fill_color,
				circle_info[i].border_width,
				circle_info[i].border_color
		);
		ZG_List_Add(textures,texture=ZG_Texture_NewFromSurface(srf));

		SDL_FreeSurface(srf);
	}

	ZG_Graphics_SetBackgroundColor(ZG_Color4f_FromHex(0xFF));

	ZG_Geometry *geometry=ZG_Geometry_NewTexturedRectangle2d();
	ZG_Appearance *appearance=ZG_Appearance_New();

	do{
		ZG_Graphics_BeginRender();

		int x=SHAPE_WIDTH,y=SHAPE_WIDTH;

		// paint primitives
		ZG_Graphics_DrawRectangle4i(x,y,SHAPE_WIDTH,SHAPE_WIDTH,ZG_COLOR4F_WHITE,2);
		x+=SHAPE_WIDTH+10;

		ZG_Graphics_DrawFilledRectangle4i(x,y,SHAPE_WIDTH,SHAPE_WIDTH,ZG_COLOR4F_WHITE);
		x+=SHAPE_WIDTH+10;

		ZG_Graphics_DrawCircle3i(x,y,SHAPE_WIDTH>>1,ZG_COLOR4F_WHITE,2);
		x+=SHAPE_WIDTH+10;


		for(unsigned i=0; i < textures->count; i++){

			appearance->texture=textures->items[i];

			// circle
			ZG_Graphics_DrawRectangleTextured4i(x,y,SHAPE_WIDTH,SHAPE_WIDTH,ZG_COLOR4F_WHITE,textures->items[i],NULL);
			x+=SHAPE_WIDTH+10;

			if((i%2==0) && i > 0){
				y+=SHAPE_WIDTH+10;
				x=10;
			}
		}

		ZG_Graphics_EndRender();

		ZG_Input_Update();
	}while(!ZG_KP_ESC);

	// unload all textures...
	for(unsigned i=0; i < textures->count; i++){
		ZG_Texture_Delete(textures->items[i]);
	}

	ZG_Geometry_Delete(geometry);
	ZG_Appearance_Delete(appearance);

	ZG_List_Delete(textures);

	ZG_DeInit();

	return 0;
}
