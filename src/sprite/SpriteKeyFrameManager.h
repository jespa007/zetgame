#ifndef __SPRITE_KEYFRAME_MANAGER_H__
#define __SPRITE_KEYFRAME_MANAGER_H__

typedef struct SpriteKeyFrameManager SpriteKeyFrameManager;

struct SpriteKeyFrameManager{
	void *data;
};

SpriteKeyFrameManager *SpriteKeyFrameManager_New(TextureManager	* _texture_manager);

bool SpriteKeyFrameManager_LoadFromMemory(
		SpriteKeyFrameManager *_this
		,const char *_path
		,const char *_key_id_prefix
		,uint8_t * _buf_ase_json
		,size_t _buf_ase_json_len
);

bool SpriteKeyFrameManager_Load(SpriteKeyFrameManager *_this
		,const char *_key_id_prefix
		,const char *_ase_json_filename
);
SpriteKeyFrame *SpriteKeyFrameManager_GetSpriteKeyFrame(SpriteKeyFrameManager *_this,const char *_skf_id);

void SpriteKeyFrameManager_Delete(SpriteKeyFrameManager *_this);


#endif
