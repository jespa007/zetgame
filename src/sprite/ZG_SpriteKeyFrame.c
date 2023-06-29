#include "_zg_sprite_.h"



/*ZG_SpriteKeyFrame *	ZG_SpriteKeyFrame_New(ZG_Texture *_texture, ZG_SpriteKeyFrameInfo *_frames, size_t _frames_len){
	ZG_SpriteKeyFrame *skf=ZG_NEW(ZG_SpriteKeyFrame);


	skf->frames=_frames;
	skf->frames_len=_frames_len;
	skf->texture=_texture;

	return skf;
}*/


void				ZG_SpriteKeyFrame_Render(ZG_SpriteKeyFrame *_this, unsigned idx_frame){
	if(idx_frame>= _this->frames_len){
		ZG_LOG_ERRORF("idx_frame out of bounds");
		return;
	}


}

// TODO: support colliders per frame
// 					ZG_SpriteKeyFrame_SetCollider(const char *_key, Collider XXXXX)
/*void 				ZG_SpriteKeyFrame_Delete(ZG_SpriteKeyFrame *_this){
	ZG_FREE(_this->frames);
	ZG_FREE(_this);
}*/

