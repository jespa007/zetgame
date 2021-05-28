#ifndef __MOVIE_FRAME_H__
#define __MOVIE_FRAME_H__

typedef struct MovieFrame MovieFrame;

struct MovieFrame{
	int 	n_frame; // frame number (required to perform the action)
	uint32_t 	delay; // 0 no delay, > 0
	int	   	goto_frame; // -1 no goto, <>0 else.
	uint32_t 	options;
};

MovieFrame MovieFrame_New();

#endif
