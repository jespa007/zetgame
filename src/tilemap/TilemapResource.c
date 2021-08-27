typedef struct{
	Tilemap 	* 	sprite_keyframes;
	size_t				sprite_keyframes_len;
	Texture			*	texture;
}TilemapResource;

TilemapResource *TilemapResource_New(size_t _sprite_keyframes_len,Texture *_texture){
	TilemapResource *skfp=NEW(TilemapResource);
	skfp->texture=_texture;
	skfp->sprite_keyframes=malloc(sizeof(Tilemap)*_sprite_keyframes_len);
	skfp->sprite_keyframes_len=_sprite_keyframes_len;
	return skfp;
}

void TilemapResource_Delete(TilemapResource * _this){

	for(unsigned i=0; i < _this->sprite_keyframes_len; i++){
		if(_this->sprite_keyframes[i].frames != NULL){
			free(_this->sprite_keyframes[i].frames);
		}
	}

	free(_this->sprite_keyframes);
	free(_this);
}
