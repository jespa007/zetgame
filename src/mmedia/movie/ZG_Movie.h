/*
 * CMovie.h
 *
 *  Created on: May 23, 2014
 *      Author: jespada
 */

#ifndef __MOVIE_H__
#define __MOVIE_H__



typedef enum{
	MOVIE_PROPERTY_LOOP = 0x1 << 0
}MOVIE_PROPERTY;



typedef struct Movie Movie;


struct Movie{

	uint8_t *	work_raw_frame[MAX_WORK_FRAMES],*pixels;
	int 	n_read,n_write,current_frame;
	float 	speed, fps;
	uint32_t 	duration_ms,movie_property,fps_ms,frame_size;
	uint16_t 	width, height;
	bool 	video_ready;
	uint8_t 	bytes_per_pixel;

};


Movie * Movie_New(Sint16 width, uint16_t height, uint8_t bytes_per_pixel);
void  	Movie_UpdateToNextFrame(Movie * _this);
void 	Movie_SyncFrameBuffer(Movie * _this);
void 	Movie_Delete(Movie *_this);

#endif
