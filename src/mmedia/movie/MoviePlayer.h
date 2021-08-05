#ifndef __MOVIE_PLAYER_H__
#define __MOVIE_PLAYER_H__

typedef struct MoviePlayer MoviePlayer;

struct MoviePlayer{
	void *data;
};

MoviePlayer * MoviePlayer_New(void);
void MoviePlayer_Delete(MoviePlayer *_this);


#endif
