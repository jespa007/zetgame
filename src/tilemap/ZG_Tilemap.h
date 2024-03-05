#ifndef __ZG_TILEMAP_H__
#define __ZG_TILEMAP_H__


typedef struct ZG_Tilemap ZG_Tilemap;

struct ZG_Tilemap{
	void *data;
};

typedef struct{
	int			tile_id;
	int 		duration;
}ZG_TileAnimationFrame;

typedef struct{
	ZG_List		*frames;
	int			u1,v1;
	uint32_t	time_change_frame;
	int			current_frame;
}ZG_TileAnimation;

typedef struct{
	ZG_Image *image;
}ZG_TileImage;

typedef struct{
	ZG_TileImage	 **tile_images;
	int			tile_width;
	int			tile_height;
	int			tile_margin;
	int			tile_spacing;
	int			tile_count;
	int			tile_count_x;
	int			tilemap_width;
	int			tilemap_height;

	ZG_List		*animations;
}Tilesets;

ZG_Tilemap *Tilemap_New(short *_tiles
		, size_t _width
		, size_t _height
		, size_t _tile_width
		, size_t _tile_height
		, ZG_Texture *_texture
		, Tilesets *_tilesets
);
void ZG_Tilemap_Update(ZG_Tilemap *_this);
void ZG_Tilemap_Draw(ZG_Tilemap *_this);
void ZG_Tilemap_Delete(ZG_Tilemap *_this);


#endif
