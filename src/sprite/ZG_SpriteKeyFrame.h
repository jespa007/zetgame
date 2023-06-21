#ifndef __ZG_SPRITE_KEYFRAME_H__
#define __ZG_SPRITE_KEYFRAME_H__

#define MAX_SPRITE_KEYFRAME_NAME	25

typedef struct ZG_SpriteKeyFrame ZG_SpriteKeyFrame;

typedef struct {
	ZG_TextureRect frame;
	uint32_t	duration;
}ZG_SpriteKeyFrameInfo;

struct ZG_SpriteKeyFrame{
	char				name[MAX_SPRITE_KEYFRAME_NAME];
	ZG_SpriteKeyFrameInfo 	*frames;
	size_t				frames_len;
	ZG_Texture				*texture;
};


//ZG_SpriteKeyFrame *	ZG_SpriteKeyFrame_New(ZG_Texture *texture, ZG_SpriteKeyFrameInfo *frames, size_t length);
void				ZG_SpriteKeyFrame_Render(ZG_SpriteKeyFrame *_this, unsigned idx_frame);
// TODO: support colliders per frame
// 					ZG_SpriteKeyFrame_SetCollider(const char *_key, Collider XXXXX)
//void 				ZG_SpriteKeyFrame_Delete(ZG_SpriteKeyFrame *_this);


#endif
