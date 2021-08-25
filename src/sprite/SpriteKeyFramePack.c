typedef struct{
	SpriteKeyFrame 	* 	sprite_keyframes;
	size_t				sprite_keyframes_len;
	Texture			*	texture;
}SpriteKeyFramePack;

SpriteKeyFramePack *SpriteKeyFramePack_New(size_t _sprite_keyframes_len,Texture *_texture){
	SpriteKeyFramePack *skfp=NEW(SpriteKeyFramePack);
	skfp->texture=_texture;
	skfp->sprite_keyframes=malloc(sizeof(SpriteKeyFrame)*_sprite_keyframes_len);
	skfp->sprite_keyframes_len=_sprite_keyframes_len;
	return skfp;
}

void SpriteKeyFramePack_Delete(SpriteKeyFramePack * _this){

	for(unsigned i=0; i < _this->sprite_keyframes_len; i++){
		if(_this->sprite_keyframes[i].frames != NULL){
			free(_this->sprite_keyframes[i].frames);
		}
	}

	free(_this->sprite_keyframes);
	free(_this);
}
