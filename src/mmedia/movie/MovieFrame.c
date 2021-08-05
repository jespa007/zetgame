#include "../../mmedia/movie/zg_movie.h"

MovieFrame MovieFrame_New(){
	MovieFrame movie_frame;

	movie_frame.n_frame = -1;
	movie_frame.delay=0;
	movie_frame.goto_frame=-1;
	movie_frame.options=0;

	return MovieFrame;
}
