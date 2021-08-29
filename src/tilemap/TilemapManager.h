#ifndef __TILEMAP_MANAGER_H__
#define __TILEMAP_MANAGER_H__

typedef struct TilemapManager TilemapManager;

struct TilemapManager{
	void *data;
};

TilemapManager *TilemapManager_New(TextureManager	* _texture_manager);
bool 			TilemapManager_Load(TilemapManager *_this,const char *_tmx_filename);
Tilemap 	  *	TilemapManager_GetTilemap(TilemapManager *_this,const char *_tm_id);

void TilemapManager_Delete(TilemapManager *_this);

#endif
