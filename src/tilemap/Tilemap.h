#ifndef __TILEMAP_H__
#define __TILEMAP_H__


typedef struct Tilemap Tilemap;

struct Tilemap{
	void *data;
	List				* 	tile_animations; // array animated_tiles
};

typedef struct{
	SDL_Surface *image;
	int			tile_id;
	int 		duration;
}TileAnimationFrame;

typedef struct{
	List		*frames;
	Uint32		time_change_frame;
	int			current_frame;
}TileAnimation;



Tilemap *Tilemap_New(short *_tiles
		, size_t _width
		, size_t _height
		, size_t _tile_width
		, size_t _tile_height
		, Texture *_texture
		,List *tile_animations
);
void Tilemap_Update(Tilemap *_this);
void Tilemap_Draw(Tilemap *_this);
void Tilemap_Delete(Tilemap *_this);


#endif
