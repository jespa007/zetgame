#include "mmedia/_zg_mmedia_.h"

MoviePlayer * MoviePlayer_New(void){
	MoviePlayer *movie_player=ZG_NEW(MoviePlayer);
	return movie_player;
}

void MoviePlayer_Delete(MoviePlayer *_this){
	ZG_FREE(_this);
}
