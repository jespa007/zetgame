#include "graphics/@zg_graphics.h"
#include "input/@zg_input.h"

#define ZG_PBO_COUNT 2
#define ZG_MASK_PBO_COUNT (ZG_PBO_COUNT-1)
#define ZG_STREAM_WIDTH 480
#define ZG_STREAM_HEIGHT 360
#define ZG_STREAM_CHANNELS 3
#define ZG_STREAM_SIZE (ZG_STREAM_WIDTH*ZG_STREAM_HEIGHT*ZG_STREAM_CHANNELS)
#define ZG_IMAGE_QUALITY 80	// The image quality goes between 1..100 that means low/high quality VS fast/slow processing


#define ZG_RADIUS_CIRCLE_CURSOR 10
#define ZG_COLOR_CIRCLE_CURSOR 0x005FE6

typedef struct{

	int index;
	int nextIndex;
	bool init;
	int n_buffer;

	float scale_x,scale_y;

	GLuint pboIds[ZG_PBO_COUNT];
	ZG_FBOInfo fbo;
	//uint8_t *processed_jpeg[MAX_BUFFERS];
	//unsigned long processed_jpeg_len[MAX_BUFFERS];
	//uint8_t message[MAX_BUFFERS][ZG_STREAM_SIZE+256];
	SDL_Surface *srf_clone;
	SDL_Surface *srf_circle;

	//SDL_RWops *rw;

	ZN_HttpServerMPS * http_server_mps;
}ZG_JpegMPSData;

void ZG_JpegMPS_SaveFrame(ZG_JpegMPSData * data,uint8_t *gpu_data);

#include "ZG_JpegMPS_GL.c"

void fill_circle2(SDL_Surface *surface, int cx, int cy, int radius, uint32_t pixel)
{
	// Note that there is more to altering the bitrate of this
	// method than just changing this value.  See how pixels are
	// altered at the following web page for tips:
	//   http://www.libsdl.org/intro.en/usingvideo.html
	static const int BPP = 3;

	double r = (double)radius;

	for (double dy = 1; dy <= r; dy += 1.0)
	{
		// This loop is unrolled a bit, only iterating through half of the
		// height of the circle.  The result is used to draw a scan line and
		// its mirror image below it.

		// The following formula has been simplified from our original.  We
		// are using half of the width of the circle because we are provided
		// with a center and we need left/right coordinates.

		double dx = floor(sqrt((2.0 * r * dy) - (dy * dy)));
		int x = cx - dx;

		// Grab a pointer to the left-most pixel for each half of the circle
		uint8_t *target_pixel_a = (uint8_t *)surface->pixels + ((int)(cy + r - dy)) * surface->pitch + x * BPP;
		uint8_t *target_pixel_b = (uint8_t *)surface->pixels + ((int)(cy - r + dy)) * surface->pitch + x * BPP;

		for (; x <= cx + dx; x++)
		{
			*(uint32_t *)target_pixel_a = pixel;
			*(uint32_t *)target_pixel_b = pixel;
			target_pixel_a += BPP;
			target_pixel_b += BPP;
		}
	}
}

bool ZG_JpegMPS_GfxSetup(ZG_JpegMPSData *data){
	switch(ZG_Graphics_GetGraphicsApi()){
	case ZG_GRAPHICS_API_GL:
		return ZG_JpegMPS_GL_SetupGfx(data);
		break;
	}

	return false;
}

ZG_JpegMPS * ZG_JpegMPS_New(void){


	ZG_JpegMPS * jpg_mps=ZG_NEW(ZG_JpegMPS);
	ZG_JpegMPSData * data=ZG_NEW(ZG_JpegMPSData);
	data->nextIndex = 0;
	data->index = 0;
	data->init = false;
	//n_write = n_read=0;

	data->fbo.color = GL_INVALID_VALUE;
	data->fbo.id = GL_INVALID_VALUE;
	data->fbo.depth = GL_INVALID_VALUE;
	//memset(aux_buffer,0,sizeof(aux_buffer));
	//rw = SDL_RWFromMem(aux_buffer, ZG_STREAM_SIZE);


	data->srf_clone = SDL_CreateRGBSurface(SDL_SWSURFACE, ZG_STREAM_WIDTH, ZG_STREAM_HEIGHT, 24, 0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);
	data->srf_circle= SDL_CreateRGBSurface(SDL_SWSURFACE, ZG_RADIUS_CIRCLE_CURSOR*2, ZG_RADIUS_CIRCLE_CURSOR*2, 24, 0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);

	fill_circle2( data->srf_circle, ZG_RADIUS_CIRCLE_CURSOR,ZG_RADIUS_CIRCLE_CURSOR,ZG_RADIUS_CIRCLE_CURSOR, ZG_COLOR_CIRCLE_CURSOR);//(0xff<<24)|(r<<16)|(g<<8)|(b<<0) );
	SDL_SetColorKey(data->srf_circle, SDL_TRUE, 0);

	data->scale_x=(float)ZG_STREAM_WIDTH/(float)ZG_Graphics_GetWidth();
	data->scale_y=(float)ZG_STREAM_HEIGHT/(float)ZG_Graphics_GetHeight();

	if(ZG_JpegMPS_GfxSetup(data)){

		data->http_server_mps = ZN_HttpServerMPS_New();
		ZN_HttpServerMPS_SetTimeDelay(data->http_server_mps,16);
		data->init=true;
	}

	return jpg_mps;
}


void ZG_JpegMPS_GfxSetup_Start(ZG_JpegMPS *_this,int port){
	if(_this == NULL) return;
	ZG_JpegMPSData *data= _this->data;

	if(data->init){
		ZN_HttpServerMPS_Start(data->http_server_mps,port);
	}
}

void ZG_JpegMPS_GfxSetup_Stop(ZG_JpegMPS *_this){
	if(_this == NULL) return;
	ZG_JpegMPSData *data= _this->data;
	if(data->init){
		ZN_HttpServerMPS_Stop(data->http_server_mps);
	}
}

#if 0
/*bool CJpegStreamNet::gestMessage(void *_socket, uint8_t *data, unsigned int size){
	return false;
}
*/

void  CJpegStreamNet::gestServer(){
	//char buffer[100];

		if(!gestServerBase())
		{
			return;
		}

		if(n_read != n_write){ // time to send server

			char *buffer=NULL;
			char http_header[128]={0};

			sprintf(http_header,
				"--boundary\r\n"
				"Content-Type: image/jpeg\r\n"
				"Content-Length: %lu\r\n\r\n"
		        , processed_jpeg_len[n_read]);



				int buffer_len = 0;

				char *start_buffer=NULL;

				/*if(!sended_header){
					buffer_len=strlen(HEAD_RESPONSE);
					start_buffer = buffer = (char *)malloc(buffer_len);
					memset(start_buffer,0,buffer_len);
					memcpy(buffer, HEAD_RESPONSE, strlen(HEAD_RESPONSE));
					buffer+=strlen(HEAD_RESPONSE);
					//not_send_multipart = true;
				}
				else{*/
					buffer_len=strlen(http_header)+processed_jpeg_len[n_read]+2;

					start_buffer = buffer = (char *)malloc(buffer_len);
					memset(start_buffer,0,buffer_len);

					memcpy(buffer, http_header, strlen(http_header));
					buffer+=strlen(http_header);
					memcpy(buffer, processed_jpeg[n_read], processed_jpeg_len[n_read]);
					buffer+=processed_jpeg_len[n_read];
					memcpy(buffer, "\r\n", 2);
				//}

		for (int clientNumber = 0; clientNumber < MAX_CLIENTS; clientNumber++)  {
			// If the socket is ready (i.e. it has data we can read)... (SDLNet_SocketReady returns non-zero if there is activity on the socket, and zero if there is no activity)
			if(socket_client[clientNumber].socket != NULL){

				int clientSocketActivity = socketReady(socket_client[clientNumber].socket);

				//cout << "Just checked client number " << clientNumber << " and received activity status is: " << clientSocketActivity << endl;

				// If there is any activity on the client socket...
				if (clientSocketActivity != 0)
				{
						if(!socket_client[clientNumber].header_sent){
							if(!putMsg(socket_client[clientNumber].socket,(uint8_t *)HEAD_RESPONSE, strlen(HEAD_RESPONSE))){
	#if __DEBUG__
								printf("gestMessage:Erasing client %i (Stream)\n",clientNumber);
	#endif
								freeSlot(&socket_client[clientNumber]);

							}

							socket_client[clientNumber].header_sent = true;
						}

						if(!putMsg(socket_client[clientNumber].socket,(uint8_t *)start_buffer, buffer_len)){
	#if __DEBUG__
							printf("gestMessage:Erasing client %i (Stream)\n",clientNumber);
	#endif
							freeSlot(&socket_client[clientNumber]);
						}
					}


				}
			}

		//SDL_Delay(30);

		free(start_buffer);
		free(processed_jpeg[n_read]);
		processed_jpeg[n_read]=NULL;

		n_read = (n_read+1)&0x1; // message readed!
		}// End of server socket check sockets loop


}
#endif
//---------------------------------------------------------------------------------------------------------------

void ZG_JpegMPS_SaveFrame(ZG_JpegMPSData * data,uint8_t *gpu_data){


	// if we can write ...
	if(ZN_HttpServerMPS_CanWrite(data->http_server_mps)){

		ZG_Vector2i mp=ZG_Input_GetMousePosition();

		SDL_Rect rect=(SDL_Rect){
				.x=(int)((mp.x-ZG_RADIUS_CIRCLE_CURSOR)*data->scale_x)
				,.y=(int)((mp.y-ZG_RADIUS_CIRCLE_CURSOR)*data->scale_y)
				//,.w=0
				//,.h=0
		};


		memcpy(data->srf_clone->pixels,gpu_data,ZG_STREAM_SIZE);

		int size_scanline = ZG_STREAM_WIDTH*3;
		char aux_scanline[size_scanline];
		uint8_t *src_up   = (uint8_t *)data->srf_clone->pixels;
		uint8_t *src_down = ((uint8_t *)data->srf_clone->pixels+(ZG_STREAM_HEIGHT-1)*size_scanline);


		for(int y = 0; y < (ZG_STREAM_HEIGHT>>1); y++){

			memcpy(aux_scanline , src_up	  	,size_scanline);
			memcpy(src_up		, src_down		,size_scanline);
			memcpy(src_down		, aux_scanline	,size_scanline);

			src_up	+=size_scanline;
			src_down-=size_scanline;
		}

		SDL_BlitSurface(data->srf_circle,NULL,data->srf_clone,&rect);


		// paint eyetracker cursor...
		long unsigned int jpeg_len;

		uint8_t *jpeg_bin = jpeg_encode_mem((uint8_t *)data->srf_clone->pixels,ZG_STREAM_WIDTH,ZG_STREAM_HEIGHT,ZG_IMAGE_QUALITY , &jpeg_len);

		ZN_HttpServerMPS_Write(data->http_server_mps,jpeg_bin,(size_t)jpeg_len);

		free(jpeg_bin);
	}
}

void ZG_JpegMPS_CaptureFrame(ZG_JpegMPSData *data){
	switch(ZG_Graphics_GetGraphicsApi()){
	case ZG_GRAPHICS_API_GL:
		ZG_JpegMPS_GL_CaptureFrame(data);
		break;
	}


}

void ZG_JpegMPS_FetchFrame(ZG_JpegMPSData *data){
	switch(ZG_Graphics_GetGraphicsApi()){
	case ZG_GRAPHICS_API_GL:
		ZG_JpegMPS_GL_FetchFrame(data);
		break;
	}


}


void ZG_JpegMPS_Update(ZG_JpegMPSData *data){

	if(data->init ) // with PBO
	{
		static unsigned n_frame=0;

		if((n_frame & 0x1) == 0){ // capture fbo..

			ZG_JpegMPS_CaptureFrame(data);

		}
		else{ //write result to framebuffer...

			//if(((n_write+1)&0x1)!= n_read)
			{
				// increment current index first then get the next index
				// "index" is used to read pixels from a framebuffer to a PBO
				// "nextIndex" is used to process pixels in the other PBO
				data->index = (data->index + 1) & ZG_MASK_PBO_COUNT;
				data->nextIndex = (data->index + 1) & ZG_MASK_PBO_COUNT;

				ZG_JpegMPS_FetchFrame(data);

			}
		}

		n_frame=(n_frame+1)&0x1;
	}

    ///////////////////////////////////////////////////
}

void ZG_JpegMPS_Delete(ZG_JpegMPS *_this){

	ZG_JpegMPSData *data=_this->data;

	ZN_HttpServerMPS_Delete(data->http_server_mps);

	//SDL_RWclose(rw);
	switch(ZG_Graphics_GetGraphicsApi()){
	case ZG_GRAPHICS_API_GL:
		ZG_JpegMPS_Delete_GL(data);
		break;
	}


	/*if(web_client != NULL){
		SDLNet_TCP_Close(web_client);
		web_client=NULL;
	}*/
	SDL_FreeSurface(data->srf_circle);
	SDL_FreeSurface(data->srf_clone);

}
