#include "zg_sprite.h"



/*SpriteKeyFrame *	SpriteKeyFrame_New(Texture *_texture, SpriteKeyFrameInfo *_frames, size_t _frames_len){
	SpriteKeyFrame *skf=ZG_NEW(SpriteKeyFrame);


	skf->frames=_frames;
	skf->frames_len=_frames_len;
	skf->texture=_texture;

	return skf;
}*/


void				SpriteKeyFrame_Render(SpriteKeyFrame *_this, unsigned idx_frame){
	if(idx_frame>= _this->frames_len){
		Log_Error("idx_frame out of bounds");
		return;
	}


}

// TODO: support colliders per frame
// 					SpriteKeyFrame_SetCollider(const char *_key, Collider XXXXX)
/*void 				SpriteKeyFrame_Delete(SpriteKeyFrame *_this){
	FREE(_this->frames);
	FREE(_this);
}*/

