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
	List		*frames;
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
	int			tile_count;
	int			tilemap_width;
	int			tilemap_height;

	List		*animations;
}Tilesets;

Tilemap *Tilemap_New(short *_tiles
		, size_t _width
		, size_t _height
		, size_t _tile_width
		, size_t _tile_height
		, Texture *_texture
		,Tilesets *_tilesets
);
void Tilemap_Update(Tilemap *_this);
void Tilemap_Draw(Tilemap *_this);
void Tilemap_Delete(Tilemap *_this);


#endif
