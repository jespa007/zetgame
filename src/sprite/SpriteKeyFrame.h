#ifndef __SPRITE_KEYFRAME_H__
#define __SPRITE_KEYFRAME_H__

typedef struct SpriteKeyFrame SpriteKeyFrame;

typedef struct {
	TextureRect frame;
	uint32_t	time;
}SpriteKeyFrameInfo;

struct SpriteKeyFrame{
	SpriteKeyFrameInfo *frames;
	size_t				frames_len;
	Texture				*texture;
};


//SpriteKeyFrame *	SpriteKeyFrame_New(Texture *texture, SpriteKeyFrameInfo *frames, size_t length);
void				SpriteKeyFrame_Render(SpriteKeyFrame *_this, unsigned idx_frame);
// TODO: support colliders per frame
// 					SpriteKeyFrame_SetCollider(const char *_key, Collider XXXXX)
//void 				SpriteKeyFrame_Delete(SpriteKeyFrame *_this);


#endif
