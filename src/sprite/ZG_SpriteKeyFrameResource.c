typedef struct{
	ZG_SpriteKeyFrame 	* 	sprite_keyframes;
	size_t				sprite_keyframes_len;
	ZG_Texture			*	texture;
}ZG_SpriteKeyFrameResource;

ZG_SpriteKeyFrameResource *ZG_SpriteKeyFrameResource_New(size_t _sprite_keyframes_len,ZG_Texture *_texture){
	ZG_SpriteKeyFrameResource *skfp=ZG_NEW(ZG_SpriteKeyFrameResource);
	skfp->texture=_texture;
	skfp->sprite_keyframes=malloc(sizeof(ZG_SpriteKeyFrame)*_sprite_keyframes_len);
	skfp->sprite_keyframes_len=_sprite_keyframes_len;
	return skfp;
}

void ZG_SpriteKeyFrameResource_Delete(ZG_SpriteKeyFrameResource * _this){

	for(unsigned i=0; i < _this->sprite_keyframes_len; i++){
		if(_this->sprite_keyframes[i].frames != NULL){
			free(_this->sprite_keyframes[i].frames);
		}
	}

	free(_this->sprite_keyframes);
	free(_this);
}
