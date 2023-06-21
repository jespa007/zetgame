#ifndef __ZG_SPRITE_KEYFRAME_MANAGER_H__
#define __ZG_SPRITE_KEYFRAME_MANAGER_H__

typedef struct ZG_SpriteKeyFrameManager ZG_SpriteKeyFrameManager;

struct ZG_SpriteKeyFrameManager{
	void *data;
};

ZG_SpriteKeyFrameManager *ZG_SpriteKeyFrameManager_New(ZG_TextureManager	* _texture_manager);

bool ZG_SpriteKeyFrameManager_LoadFromMemory(
		ZG_SpriteKeyFrameManager *_this
		,const char *_path
		,const char *_key_id_prefix
		,uint8_t * _buf_ase_json
		,size_t _buf_ase_json_len
);

bool ZG_SpriteKeyFrameManager_Load(ZG_SpriteKeyFrameManager *_this
		,const char *_key_id_prefix
		,const char *_ase_json_filename
);
ZG_SpriteKeyFrame *ZG_SpriteKeyFrameManager_Get(ZG_SpriteKeyFrameManager *_this,const char *_skf_id);

void ZG_ZG_SpriteKeyFrameManager_Delete(ZG_SpriteKeyFrameManager *_this);


#endif
