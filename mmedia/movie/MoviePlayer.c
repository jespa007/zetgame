#include "mmedia/zg_mmedia.h"

MoviePlayer * MoviePlayer_New(void){
	MoviePlayer *movie_player=NEW(MoviePlayer);
	return movie_player;
}

void MoviePlayer_Delete(MoviePlayer *_this){
	FREE(_this);
}
