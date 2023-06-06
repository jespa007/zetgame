#ifndef __TILEMAP_H__
#define __TILEMAP_H__


typedef struct Tilemap Tilemap;

struct Tilemap{
	void *data;
};

typedef struct{
	int			tile_id;
	int 		duration;
}TileAnimationFrame;

typedef struct{
	ZG_List		*frames;
	int			u1,v1;
	Uint32		time_change_frame;
	int			current_frame;
}TileAnimation;

typedef struct{
	SDL_Surface *image;
}TileImage;

typedef struct{
	TileImage	 **tile_images;
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

Tilemap *Tilemap_New(short *_tiles
		, size_t _width
		, size_t _height
		, size_t _tile_width
		, size_t _tile_height
		, ZG_Texture *_texture
		, Tilesets *_tilesets
);
void Tilemap_Update(Tilemap *_this);
void Tilemap_Draw(Tilemap *_this);
void Tilemap_Delete(Tilemap *_this);


#endif
