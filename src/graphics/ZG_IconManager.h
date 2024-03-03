#ifndef __ZG_ICON_MANAGER_H__
#define __ZG_ICON_MANAGER_H__


// default icons
typedef enum {
	ZG_DEFAULT_ICON_NONE=-1,
	ZG_DEFAULT_ICON_ARROW_DOWN_SMALL=16*0+4,
	ZG_DEFAULT_ICON_ARROW_UP=16*1+0,
	ZG_DEFAULT_ICON_ARROW_RIGHT=16*1+2,
	ZG_DEFAULT_ICON_ARROW_DOWN=16*1+4,
	ZG_DEFAULT_ICON_ARROW_LEFT=16*1+6,
	ZG_DEFAULT_ICON_MOUSE_CLOSED_HAND=16*1+13,
	ZG_DEFAULT_ICON_MOUSE_OPENED_HAND,
	ZG_DEFAULT_ICON_SIZE_WE=16*3+10,
	ZG_DEFAULT_ICON_RELOAD=16*4,
	ZG_DEFAULT_ICON_PLAY=160,
	ZG_DEFAULT_ICON_PAUSE,
	ZG_DEFAULT_ICON_FORWARD,
	ZG_DEFAULT_ICON_REWIND,
	ZG_DEFAULT_ICON_NEXT_PLAY,
	ZG_DEFAULT_ICON_PREVIOUS_PLAY,
	ZG_DEFAULT_ICON_STOP,
	ZG_DEFAULT_ICON_MOUSE_MOVE=16*5,
	ZG_DEFAULT_ICON_MOUSE=16*5+12,
	ZG_DEFAULT_ICON_MOUSE_TRANSPARENT,
	ZG_DEFAULT_ICON_HAND,
	ZG_DEFAULT_ICON_CLOSE_BIG=16*8+6,
	ZG_DEFAULT_ICON_CHECK=16*9+4,
	ZG_DEFAULT_ICON_FILL_CIRCLE=16*9+5
	//MAX_ICONS
}DefaultIcon;

typedef struct{
	ZG_Rectanglef   	texture_crop;
	ZG_Texture  	*	texture;
}Icon;

typedef struct ZG_IconManager ZG_IconManager;

struct ZG_IconManager{
	uint16_t 		n_icons;
	ZG_Texture *	texture;
	uint16_t 		icon_width,
					icon_height,
					icon_offset_x,
					icon_offset_y,
					icon_scanline_width,
					icons_per_row;
};


void 	 		ZG_IconManager_Init(void);
void 	 		ZG_IconManager_DeInit(void);

ZG_IconManager * 	IconManager_GetDefault(void);


ZG_IconManager * IconManager_LoadFromMemory(
		const uint8_t *ptr,
		size_t ptr_len,
		uint16_t icon_width,
		uint16_t icon_height,
		uint16_t icon_margin_x,
		uint16_t icon_margin_y,
		uint16_t icons_per_row
);

ZG_IconManager * IconManager_LoadFromFile(
		const  char * _filename,
		uint16_t icon_width,
		uint16_t icon_height,
		uint16_t icon_margin_x,
		uint16_t icon_margin_y,
		uint16_t icons_per_row
);


void 	ZG_IconManager_DrawIcon(ZG_IconManager *_this,uint16_t idx_icon, int x,int y,uint16_t width, uint16_t height);
void 	ZG_IconManager_DrawIconDefault(uint16_t idx_icon,int x1,int y1,uint16_t width, uint16_t height);
Icon	ZG_IconManager_GetIcon(ZG_IconManager *_this, uint16_t idx_icon);
Icon	ZG_IconManager_GetIconDefault(uint16_t idx_icon);

void 	ZG_IconManager_Delete(ZG_IconManager *_this);




#endif
