/*
 * CMovie.cpp
 *
 *  Created on: May 23, 2014
 *      Author: jespada
 */


#include "../../mmedia/movie/zg_movie.h"

#define VISIBLE_FRAME (1-WORK_FRAME)

typedef enum{
	WORK_FRAME_1=0,
	WORK_FRAME_2,
	MAX_WORK_FRAMES,
	MASK_MAX_WORK_FRAMES=(MAX_WORK_FRAMES-1)
}WORK_FRAME;


Movie * Movie_New(Sint16 width, uint16_t height, uint8_t bytes_per_pixel){
	Movie * movie = NEW(Movie);
	memset(movie,0,sizeof(Movie));

	movie->width =  width;
	movie->height = height;
	movie->bytes_per_pixel =  bytes_per_pixel;
	movie->frame_size = width*height*bytes_per_pixel;

	for(int i = 0; i < MAX_WORK_FRAMES;i++){
		movie->work_raw_frame[i] = (uint8_t *) malloc(movie->frame_size);
		memset(movie->work_raw_frame[i],0,movie->frame_size);
	}

	movie->pixels = (uint8_t *) malloc(movie->frame_size);
	memset(movie->pixels,0,movie->frame_size);

	movie->duration_ms = 0;
	movie->n_read=0;
	movie->n_write=0;
	movie->fps = 25;
	movie->fps_ms=(uint32_t)((1000.0f/(double)movie->fps)+0.5f);
	movie->properties = 0;

	movie->video_ready = false;
	movie->current_frame = 0;
	movie->speed=0;

	return movie;
}

void  Movie_UpdateToNextFrame(Movie * _this){

	if(((_this->n_write+1)&MASK_MAX_WORK_FRAMES)!= _this->n_read)
		_this->n_write = (_this->n_write+1)&MASK_MAX_WORK_FRAMES;

	_this->current_frame++;
}


void Movie_SyncFrameBuffer(Movie * _this) {

	if(!_this->video_ready) return;


	if(_this->n_read != _this->n_write){
		memcpy(_this->pixels, _this->work_raw_frame[_this->n_read] ,_this->frame_size);//m_width*m_height*m_bytes_per_pixel);
		_this->n_read = (_this->n_read+1)&MASK_MAX_WORK_FRAMES;
	}
}

void Movie_Delete(Movie *_this){

	if(_this == NULL) return;

	for(int i = 0; i < MAX_WORK_FRAMES; i++){
		free(_this->work_raw_frame[i]);
		_this->work_raw_frame[i]=NULL;
	}

	free(_this->pixels);
}


