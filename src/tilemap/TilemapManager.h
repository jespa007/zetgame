#ifndef __TILEMAP_MANAGER_H__
#define __TILEMAP_MANAGER_H__

typedef struct TilemapManager TilemapManager;

struct TilemapManager{
	void *data;
};

TilemapManager *TilemapManager_New(void);

bool TilemapManager_LoadFromMemory(
		TilemapManager *_this
		,const char *key_id_prefix
		,uint8_t *buf_texture
		,size_t buf_texture_len
		,uint8_t * buf_json
		,size_t buf_json_len
);

bool TilemapManager_Load(TilemapManager *_this
		,const char *_key_id_prefix
		, const char *_image_filename
		,const char *_json_filename
		, const char *_extra_json_filename
);
Tilemap *GetTilemap(TilemapManager *_this,const char *_skf_id);

void TilemapManager_Delete(TilemapManager *_this);

#endif
