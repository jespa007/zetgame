#ifndef __ZG_TILEMAP_MANAGER_H__
#define __ZG_TILEMAP_MANAGER_H__

typedef struct ZG_TilemapManager ZG_TilemapManager;

struct ZG_TilemapManager{
	void *data;
};

ZG_TilemapManager *ZG_TilemapManager_New(ZG_TextureManager	* _texture_manager);
bool 			ZG_TilemapManager_Load(ZG_TilemapManager *_this,const char *_tmx_filename);
ZG_Tilemap 	  *	ZG_TilemapManager_Get(ZG_TilemapManager *_this,const char *_tm_id);

void ZG_TilemapManager_Delete(ZG_TilemapManager *_this);

#endif
