#ifndef __SPRITE_KEYFRAME_MANAGER_H__
#define __SPRITE_KEYFRAME_MANAGER_H__

typedef struct SpriteKeyFrameManager SpriteKeyFrameManager;

struct SpriteKeyFrameManager{
	void *data;
};

SpriteKeyFrameManager *SpriteKeyFrameManager_New(void);

bool SpriteKeyFrameManager_LoadFromMemory(
		SpriteKeyFrameManager *_this
		,const char *key_id_prefix
		,uint8_t *buf_texture
		,size_t buf_texture_len
		,uint8_t * buf_json
		,size_t buf_json_len
);

bool SpriteKeyFrameManager_Load(SpriteKeyFrameManager *_this
		,const char *_key_id_prefix
		, const char *_texture_filename
		,const char *_json_filename
		, const char *_extra_json_filename
);
SpriteKeyFrame *GetSpriteKeyFrame(SpriteKeyFrameManager *_this,const char *_skf_id);

void SpriteKeyFrameManager_Delete(SpriteKeyFrameManager *_this);


#endif
