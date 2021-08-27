#ifndef __TILEMAP_H__
#define __TILEMAP_H__


typedef struct Tilemap Tilemap;

struct Tilemap{
	void *data;
};


Tilemap *Tilemap_New(short *_tiles, size_t _width, size_t _height, size_t _tile_width, size_t _tile_height, Texture *_texture);
void Tilemap_Delete(Tilemap *_this);


#endif
