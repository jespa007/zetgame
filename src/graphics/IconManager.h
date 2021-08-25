#ifndef __ICON_MANAGER_H__
#define __ICON_MANAGER_H__


// default icons
typedef enum {
	DEFAULT_ICON_NONE=-1,
	DEFAULT_ICON_ARROW_DOWN_SMALL=16*0+4,
	DEFAULT_ICON_ARROW_UP=16*1+0,
	DEFAULT_ICON_ARROW_RIGHT=16*1+2,
	DEFAULT_ICON_ARROW_DOWN=16*1+4,
	DEFAULT_ICON_ARROW_LEFT=16*1+6,
	DEFAULT_ICON_MOUSE_CLOSED_HAND=16*1+13,
	DEFAULT_ICON_MOUSE_OPENED_HAND,
	DEFAULT_ICON_SIZE_WE=16*3+10,
	DEFAULT_ICON_RELOAD=16*4,
	DEFAULT_ICON_PLAY=160,
	DEFAULT_ICON_PAUSE,
	DEFAULT_ICON_FORWARD,
	DEFAULT_ICON_REWIND,
	DEFAULT_ICON_NEXT_PLAY,
	DEFAULT_ICON_PREVIOUS_PLAY,
	DEFAULT_ICON_STOP,
	DEFAULT_ICON_MOUSE_MOVE=16*5,
	DEFAULT_ICON_MOUSE=16*5+12,
	DEFAULT_ICON_MOUSE_TRANSPARENT,
	DEFAULT_ICON_HAND,
	DEFAULT_ICON_CLOSE_BIG=16*8+6,
	DEFAULT_ICON_CHECK=16*9+4,
	DEFAULT_ICON_FILL_CIRCLE=16*9+5
	//MAX_ICONS
}DefaultIcon;

typedef struct{
	TextureRect   	texture_crop;
	Texture  	*	texture;
}Icon;

typedef struct IconManager IconManager;

struct IconManager{
	uint16_t 		n_icons;
	Texture *	texture;
	uint16_t 		icon_width,
				icon_height,
				icon_offset_x,
				icon_offset_y,
				icon_scanline_width,
				icons_per_row;
};


void 	 		IconManager_Init(void);
void 	 		IconManager_DeInit(void);

IconManager * 	IconManager_GetDefault(void);


IconManager * IconManager_LoadFromMemory(
		const uint8_t *ptr,
		size_t ptr_len,
		uint16_t icon_width,
		uint16_t icon_height,
		uint16_t icon_margin_x,
		uint16_t icon_margin_y,
		uint16_t icons_per_row
);

IconManager * IconManager_LoadFromFile(
		const  char * _filename,
		uint16_t icon_width,
		uint16_t icon_height,
		uint16_t icon_margin_x,
		uint16_t icon_margin_y,
		uint16_t icons_per_row
);


void 	IconManager_DrawIcon(IconManager *_this,uint16_t idx_icon, int x,int y,uint16_t width, uint16_t height);
void 	IconManager_DrawIconDefault(uint16_t idx_icon,int x1,int y1,uint16_t width, uint16_t height);
Icon	IconManager_GetIcon(IconManager *_this, uint16_t idx_icon);
Icon	IconManager_GetIconDefault(uint16_t idx_icon);

void 	IconManager_Delete(IconManager *_this);




#endif
