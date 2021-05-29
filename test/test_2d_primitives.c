#include "../ZetGame.h"

#define SHAPE_WIDTH			50
#define SHAPER_COLOR		0xff707070
#define SHAPE_BORDER_COLOR	0xffe0e0e0



int main(int argc, char *argv[]){

	struct{
		uint32_t width, fill_color, border_width,border_color;
	}circle_info[2]={
			{SHAPE_WIDTH>>1,SHAPER_COLOR,0,0}
			,{SHAPE_WIDTH>>1,SHAPER_COLOR,SHAPE_WIDTH*0.1,SHAPE_BORDER_COLOR}
	};


	ZetgineCore_Init(NULL);



	List *textures= List_New();

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
		List_Add(textures,texture=Texture_New());

		Texture_UpdateFromSurface(texture,srf);



//		List_Add(images,SDL_NewCircle(SHAPE_WIDTH>>1,SHAPER_COLOR,0,0));
//		List_Add(images,SDL_NewCircle(SHAPE_WIDTH>>1,SHAPER_COLOR,SHAPE_WIDTH*0.1,SHAPE_BORDER_COLOR));
	}




	Graphics_SetBackgroundColor(Color4f_FromHex(0x0));

	//img=SDL_NewArrow(10,10);
	//List_Add(l_texture,text=Texture_New());
	//Texture_SetImage(text,img);
	//SDL_FreeSurface(img);
	//SDL_NewSurface(10,10,4);

	do{
		Graphics_BeginRender();

		int x=10,y=10;

		for(unsigned i=0; i < textures->count; i++){

			// circle
			Graphics_DrawRectangleTextured(x,y,SHAPE_WIDTH,SHAPE_WIDTH,COLOR4F_WHITE,textures->items[i],NULL);
			x+=SHAPE_WIDTH+10;

			if((i%2==0) && i > 0){
				y+=SHAPE_WIDTH+10;
				x=10;
			}

		}

		// square

		// cross


		Graphics_EndRender();

		Input_Update();
	}while(!K_ESC);

	// unload all textures...
	for(unsigned i=0; i < textures->count; i++){
		Texture_Delete(textures->items[i]);
	}

	List_Delete(textures);

	ZetgineCore_DeInit();

#ifdef __MEMMANAGER__
	MEMMGR_print_status();
#endif

	return 0;
}
