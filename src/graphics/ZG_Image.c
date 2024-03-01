#include "@zg_graphics.h"
#include "assets/image/default.png.c"


typedef struct{
	uint8_t *	pixels;
	uint8_t 	bytes_per_pixel;
	uint8_t 	bits_per_pixel;
	uint16_t	width;
	uint16_t	height;
}ZG_ImageData;

/*{
 * signature
 * signature_length
 * load_mem_function (buffer,buffer_len)
 *
 }*/
ZG_Image * ZG_Image_LoadJPGFromMemory(const uint8_t *buffer_src, size_t buffer_src_len);
ZG_Image * ZG_Image_LoadPNGFromMemory(const uint8_t *buffer_src, size_t buffer_src_len);
ZG_Image *ZG_Image_LoadImageFromMemory(const uint8_t * image_src, size_t length, uint32_t properties, uint8_t convert_to_bpp);

typedef struct {
	const char *file_type;
	uint8_t signature[20];
	uint8_t signature_len;
	ZG_Image * (*load_mem)(const uint8_t *ptr, size_t len);
}ImageLoadInfo;

ImageLoadInfo image_load_info[]={
	 {"PNG",{ 0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A,0x1A,0x0A },8,ZG_Image_LoadPNGFromMemory}
	,{"JPG",{ 0xFF, 0xD8, 0xFF, 0xE0 },4,ZG_Image_LoadJPGFromMemory}
};

static ZG_Image * default_image=NULL;

ZG_Image * 	ZG_Image_GetDefaultImage(void){
	if(default_image == NULL){
		default_image=ZG_Image_LoadImageFromMemory(default_png,default_png_len,0,0);
	}
	return default_image;
}

void			ZG_Image_DestroyResources(void){
	if(default_image != NULL){
		ZG_Image_FreeSurface(default_image);
		default_image=NULL;
	}
}


ZG_Image * ZG_Image_LoadJPGFromMemory(const uint8_t *buffer_src, size_t buffer_src_len){
	ZG_Image *srf = NULL;
	unsigned w, h;
	uint8_t *buffer_dst;


	if(jpeg_decode_mem(buffer_src,buffer_src_len, &buffer_dst, &w,&h)){

		// create surface from buffer into 24 bits ...
		srf=ZG_Image_CreateImageFrom(w,h,3,buffer_dst);

		// release memory.
		free(buffer_dst);
	}

	return srf;
}

bool ZG_Image_SaveJPG(ZG_Image * _this, const char * filename){
	uint8_t *out=NULL;
	unsigned long outsize=0;
	bool ok=false;

	if(_this!=NULL){
		ZG_Image *aux=NULL;
		ZG_ImageData *data=_this->data;
		if(data->bits_per_pixel != 24){ // convert in 24 bits auxiliary
			aux=ZG_Image_ConvertSurfaceExt(srf,0,3); // 3 bytes per pixel -> 24bits per pixel
			if(aux==NULL){
				return false;
			}
			srf=aux;
		}

		if((out=jpeg_encode_mem(data->pixels, data->width, data->height, 80, &outsize))!=NULL){
			ZG_BufferByte bb=(ZG_BufferByte){.ptr=out,.len=outsize};
			ok=File_Write(filename,&bb);

			ZG_FREE(bb.ptr);
		}
		else{
			ZG_LOG_ERROR("Error saving %s",filename);
		}


		if(aux != NULL){
			ZG_Image_Delete(aux);
		}

	}

	return ok;
}
//--------------------------------------------------------------------------------------------------------------------------------
ZG_Image * ZG_Image_LoadPNGFromMemory(const uint8_t *buffer_src, size_t buffer_src_len){
	ZG_Image *srf = NULL;
	uint8_t *out;
	unsigned w, h,error;

	//lodepng_decode_memory(unsigned char** out, unsigned* w, unsigned* h, const unsigned char* in,
	//                               size_t insize, LodePNGColorType colortype, unsigned bitdepth);
	error=lodepng_decode_memory(&out,&w,&h,buffer_src,buffer_src_len,LCT_RGBA,8);

	// we can create SDL surface w*h, 32
	if(!error){
		// create surface from buffer (png is always 32 bits in principle) ...
		srf=ZG_Image_CreateImageFrom(w,h,4,out);

		// release memory.
		free(out);
	}

	return srf;
}

bool ZG_Image_SavePNG(ZG_Image * _image, const char * filename){
	ZG_ImageData *data=_image->data;
	unsigned error=0;
	uint8_t *out;
	size_t outsize;
	bool ok=false;

	if(_image!=NULL){
		LodePNGColorType color_type=0;

		switch(data->bits_per_pixel){
		case 24:
			color_type=LCT_RGB;
			break;
		case 32:
			color_type=LCT_RGBA;
			break;
		default:
			ZG_LOG_ERROR("ZG_Image_SavePNG : Error saving %s. Unsupported bit depth",filename);
			return false;

		}

		//unsigned lodepng_encode_memory(unsigned char** out, size_t* outsize, const unsigned char* image,
		//                               unsigned w, unsigned h, LodePNGColorType colortype, unsigned bitdepth)
		error=lodepng_encode_memory(&out,&outsize,data->pixels,data->width,data->height,color_type,8);
		if(!error){
			ZG_BufferByte bb=(ZG_BufferByte){.ptr=out,.len=outsize};
			ok=File_Write(filename,&bb);
			ZG_FREE(bb.ptr);
		}else{
			ZG_LOG_ERROR("ZG_Image_SavePNG : Error saving %s. %s",filename,lodepng_error_text(error));
		}
	}

	return ok;
}
//--------------------------------------------------------------------------------------------------------------------------------
ZG_Image *ZG_Image_LoadImageFromMemory(
		const uint8_t * _image_src
		, size_t _length
		, uint32_t _properties
		, uint8_t _convert_to_bpp
) {
	ZG_Image *new_image_surface = NULL;

	size_t size_image_load_info=ZG_ARRAY_SIZE(image_load_info);

	if(length < 10){
		ZG_LOG_ERRORF("Cannot load from memory. Insuficient buffer");
		return NULL;
	}

	for(unsigned i=0; i < size_image_load_info &&new_image_surface==NULL ; i++){
		ImageLoadInfo *img_info= &image_load_info[i];
		if(strncmp((const char *)img_info->signature,(const char *)_image_src,img_info->signature_len)==0){
			if((new_image_surface=img_info->load_mem(_image_src,length))==NULL){
				return NULL;
			}
		}
	}

	ZG_Image *aux_surf = ZG_Image_ConvertSurfaceExt(new_image_surface, properties, convert_to_bpp);
	ZG_Image_FreeSurface(new_image_surface);

	return aux_surf;
}
//--------------------------------------------------------------------------------------------------------------------------------
ZG_Image	*	ZG_Image_LoadImageFromFile(
	const char * filename
	, uint32_t properties
	, uint8_t convert_to_bpp
) {
	ZG_BufferByte *buf=ZG_FileSystem_ReadFile(filename);
	ZG_Image *new_image_surface = NULL;
	if(!buf){return NULL;}

	new_image_surface = ZG_Image_LoadImageFromMemory(buf->ptr,buf->len,properties,convert_to_bpp);
	ZG_BufferByte_Delete(buf);

	if(new_image_surface!=NULL){
		ZG_Image *aux = ZG_Image_ConvertSurfaceExt(new_image_surface, properties, convert_to_bpp);
		ZG_Image_FreeSurface(new_image_surface);
		return aux;
	}

	return NULL;

}
//--------------------------------------------------------------------------------------------------------------------------------

ZG_Image * ZG_Image_NewImageFromFile(const char *_filename){
	ZG_BufferByte *buffer= NULL;
	ZG_Image *image = NULL;

	if(ZG_FileSystem_FileExists(_filename)){
		buffer=ZG_FileSystem_ReadFile(_filename);

		image=ZG_Image_NewImageFromMemory(buffer->ptr,buffer->len);
		ZG_BufferByte_Delete(buffer);

	}else{
		ZG_LOG_ERROR("File '%s' not exist",_filename);
	}
	return image;
}


ZG_Image *ZG_Image_New(uint16_t _width, uint16_t _height){
	ZG_Image *img=ZG_NEW(ZG_Image);
	ZG_ImageData *data=ZG_NEW(ZG_ImageData);
	uint8_t bytes_per_pixel=ZG_Graphics_GetBytesPerPixel();
	ZG_Image *img = ZG_Image_NewImageBytesPerPixel(_width,_height,bytes_per_pixel);;

	img->data=data;

	return img;
}

uint16_t 		ZG_Image_GetHeight(ZG_Image *_this){
	ZG_ImageData *data=_this->data;

	return data->height;
}

uint16_t 		ZG_Image_GetWidth(ZG_Image *_this){
	ZG_ImageData *data=_this->data;
	return data->width;
}

uint8_t 	*	ZG_Image_GetPixels(ZG_Image *_this){
	ZG_ImageData *data=_this->data;
	return data->pixels;
}


/*
 * This is a 32-bit pixel function created with help from this
 * You will need to make changes if you want it to work with
 * 8-, 16- or 24-bit images.
 */
void ZG_Image_SetPixel(ZG_Image *_this, int _x, int _y, uint32_t _pixel){
	ZG_ImageData *data=_this->data;
	uint8_t bytes_per_pixel=data->bytes_per_pixel;
	uint8_t *target_pixel = (uint8_t *)data->pixels + _y * data->pitch + x * bytes_per_pixel;
	memcpy(target_pixel, &_pixel,bytes_per_pixel);
}


/*
 * This is an implementation of the Midpoint Circle Algorithm
 * found on Wikipedia at the following link:
 *
 *   http://en.wikipedia.org/wiki/Midpoint_circle_algorithm
 *
 * The algorithm elegantly draws a circle quickly, using a
 * set_pixel function for clarity.
 */
void ZG_Image_DrawCircle(ZG_Image *_this, int n_cx, int n_cy, int radius, uint32_t pixel){
	ZG_ImageData *data=_this->data;
	// if the first pixel in the screen is represented by (0,0) (which is in sdl)
	// remember that the beginning of the circle is not in the middle of the pixel
	// but to the left-top from it:

	double error = (double)-radius;
	double x = (double)radius -0.5;
	double y = (double)0.5;
	double cx = n_cx - 0.5;
	double cy = n_cy - 0.5;

	double transX = 0;//cx-surface->w;
	double transY = 0;//cy-surface->h;

	if(transX<0)
		transX=0;

	if(transY<0)
		transY=0;

	while (x >= y)
	{
		ZG_Image_SetPixel(_this, (int)(cx + x+transX), (int)(cy + y+transY), pixel);
		ZG_Image_SetPixel(_this, (int)(cx + y+transX), (int)(cy + x+transY), pixel);

		if (x != 0)
		{
			ZG_Image_SetPixel(_this, (int)(cx - x+transX), (int)(cy + y+transY), pixel);
			ZG_Image_SetPixel(_this, (int)(cx + y+transX), (int)(cy - x+transY), pixel);
		}

		if (y != 0)
		{
			ZG_Image_SetPixel(_this, (int)(cx + x+transX), (int)(cy - y+transY), pixel);
			ZG_Image_SetPixel(_this, (int)(cx - y+transX), (int)(cy + x+transY), pixel);
		}

		if (x != 0 && y != 0)
		{
			ZG_Image_SetPixel(_this, (int)(cx - x+transX), (int)(cy - y+transY), pixel);
			ZG_Image_SetPixel(_this, (int)(cx - y+transX), (int)(cy - x+transY), pixel);
		}

		error += y;
		++y;
		error += y;

		if (error >= 0)
		{
			--x;
			error -= x;
			error -= x;
		}
	}
}


/*
 * ZG_Image 32-bit circle-fill algorithm without using trig
 *
 * While I humbly call this "Celdecea's Method", odds are that the
 * procedure has already been documented somewhere uint32_t ago.  All of
 * the circle-fill examples I came across utilized trig functions or
 * scanning neighbor pixels.  This algorithm identifies the width of
 * a semi-circle at each pixel height and draws a scan-line covering
 * that width.
 *
 * The code is not optimized but very fast, owing to the fact that it
 * alters pixels in the provided surface directly rather than through
 * function calls.
 *
 * WARNING:  This function does not lock surfaces before altering, so
 * use ZG_Image_LockSurface in any release situation.
 */
void ZG_Image_FillCircle(ZG_Image *_this, int cx, int cy, int radius, uint32_t pixel){
	ZG_ImageData *data=_this->data;
	// Note that there is more to altering the bitrate of this
	// method than just changing this value.  See how pixels are
	// altered at the following web page for tips:
	//   http://www.libsdl.org/intro.en/usingvideo.html

	uint8_t bytes_per_pixel = data->bytes_per_pixel;

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
		uint8_t *target_pixel_a = (uint8_t *)data->pixels + ((int)(cy + r - dy)) * data->pitch + x * bytes_per_pixel;
		uint8_t *target_pixel_b = (uint8_t *)data->pixels + ((int)(cy - r + dy)) * data->pitch + x * bytes_per_pixel;

		for (; x <= cx + dx; x++)
		{
			memcpy(target_pixel_a, &pixel,bytes_per_pixel);
			memcpy(target_pixel_b, &pixel,bytes_per_pixel);

			target_pixel_a += bytes_per_pixel;
			target_pixel_b += bytes_per_pixel;
		}
	}
}

ZG_Image * ZG_Image_NewCircle(uint16_t radius, uint32_t fill_color, uint16_t width_border, uint32_t border_color){
	int r1 = radius*2;
	int r2 = radius*2-width_border*2;
	uint8_t bytes_per_pixel=ZG_Graphics_GetBytesPerPixel();

	if((r1 > 0 && r2 > 0)){

		ZG_Image *srf1 = ZG_Image_NewSurface(r1,r1,bytes_per_pixel);
		ZG_Image *srf2 = ZG_Image_NewSurface(r2,r2,bytes_per_pixel);

		ZG_ImageData *srf1_data=srf1->data;
		ZG_ImageData *srf2_data=srf2->data;

		ZG_Image_FillCircle( srf1, radius,radius,radius, border_color);
		ZG_Image_FillCircle( srf2, radius-width_border,radius-width_border,radius-width_border, fill_color);

		ZG_Image_Rect rect={width_border,width_border,srf2_data->width, srf2_data->height};
		ZG_Image_BlitSurface(srf2,NULL, srf1,&rect);
		ZG_Image_FreeSurface(srf2);

		//draw_circle();
		return srf1;
	}
	return NULL;

}

/*
ZG_Image * ZG_Image_NewTriangle(uint16_t dimension, uint32_t fill_color, uint16_t border_width, uint32_t border_color){
	ZG_UNUSUED_PARAM(border_color);
	return NULL;
}
*/

void ZG_Image_BlitImage(
		ZG_Image * _src_image
		,ZG_Rectangle *_src_rect
		,ZG_Image * _dst_image
		,ZG_Rectangle *_dst_rect
){
	// operation step by step
	// set start pixel offset for each src/dst
	// if dimensions src/dst are equal or fill whole image scale=1 otherwise, if calcule dst scale respect from src
	// read 1 src pixel
	// write 1 dst pixel
}

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
ZG_Image *	ZG_Image_Convert(ZG_Image *_src_image, uint32_t _dst_properties_format,	uint8_t _dst_bytes_per_pixel) {

	ZG_Image *new_image=NULL;
	ZG_Image *allocated_image = NULL;
	ZG_ImageData *data=_src_image->data;

	if(_dst_bytes_per_pixel != 0){ // user want some bytes per pixel

		if(data->bytes_per_pixel == 1){ // special copy...
			ZG_LOG_ERROR("Bytes per pixel '%i' conversion not supported ",data->bytes_per_pixel);
			/*new_image = ZG_Image_CreateImageFrom(data->width, data->height, 4,NULL); // 32 bits
			ZG_ImageData *new_data=new_image->data;
			uint8_t *src_p = (uint8_t *)data->pixels;
			uint32_t *dst_p = (uint32_t *)new_image->pixels;
			ZG_Image_Color *p_color=src_surface->format->palette->colors;

			for(int y = 0; y < data->height; y++){
				for(int x = 0; x < data->width; x++){
					uint8_t aux_pixel = *src_p;
					(*dst_p) = (uint32_t)p_color[aux_pixel].r | ((uint32_t)p_color[aux_pixel].g << 8)| ((uint32_t)p_color[aux_pixel].b << 16) | (((uint32_t)p_color[aux_pixel].a) << 24);
					src_p++;
					dst_p++;
				}
			}*/
		}
		else{
			new_image = ZG_Image_CreateImageFrom(data->width, data->height, _dst_bytes_per_pixel,NULL);
			ZG_Image_BlitImage(_src_image,NULL,new_image,NULL);
		}

		_src_image = new_image;
		allocated_image = new_image;
	}

	uint16_t dest_width = data->width;
	uint16_t dest_height = data->height;


	if((_dst_properties_format & ZG_Image_LOAD_IMAGE_POWER_OF_2_ORIGINAL_RESOLUTION) ==ZG_Image_LOAD_IMAGE_POWER_OF_2_ORIGINAL_RESOLUTION ||
			((dest_width == data->width) && (dest_height == data->height))
	){ // simple copy original image...
		new_image = ZG_Image_CreateImageFrom(dest_width, dest_height, data->bytes_per_pixel,NULL);
		// and just blit...
		ZG_Image_BlitSurface(_src_image,NULL,new_image,NULL);

	}else{ //scale to fit image (warning may have a bug with copying rgb values !!!)...
		new_image = ZG_Image_NewImage(dest_width,dest_height,data->bytes_per_pixel);
		if(ZG_Image_BlitScaled(
				src_surface,
				NULL,
				new_image,
				NULL)!=0
		){
			ZG_LOG_ERROR("ZG_Image_BlitScaled:%s",ZG_Image_GetError());
		}

	}


	if (_dst_properties_format & ZG_Image_LOAD_IMAGE_FLIP_Y) {
		ZG_Image_FlipY(new_image);
	}

	if(allocated_image != NULL)
	{
		ZG_Image_Delete(allocated_image);
	}

	return new_image;

}

void ZG_Image_FlipY(ZG_Image *_this){
	ZG_ImageData *data=_this->data;
	int size_scanline = data->width*data->bytes_per_pixel;
	uint8_t *src_up   = (uint8_t *)data->pixels;
	uint8_t *src_down = ((uint8_t *)data->pixels+(data->height-1)*size_scanline);
	uint8_t * aux_scanline = (uint8_t *)malloc(size_scanline);
	for(int y = 0; y < data->height/2; y++){

		memcpy(aux_scanline, src_up	  		,size_scanline);
		memcpy(src_up		, src_down		,size_scanline);
		memcpy(src_down		, aux_scanline	,size_scanline);

		src_up	+=size_scanline;
		src_down-=size_scanline;

	}

	free(aux_scanline);
}

void ZG_Image_SetBytesPerPixel(ZG_Image **_image, uint8_t _new_bytes_per_pixel){

	if(_image != NULL && *_image != NULL){
		ZG_ImageData *data=(*_image)->data;
		if(data->bytes_per_pixel !=_new_bytes_per_pixel){
			ZG_Image *new_image = ZG_Image_Convert(*_image,0,_new_bytes_per_pixel);
			if(new_image){
				ZG_Image_Delete(*_image);
				*_image = new_image;
			}
		}
	}
}

ZG_Image *	ZG_Image_Crop(ZG_Image *_src_surface,ZG_Rectangle _src_rect){
	ZG_ImageData *data=_src_surface->data;
	//ZG_Image_Rect dst_rect={0,0,_src_rect.w,_src_rect.h};
	ZG_Image *dst_surface=ZG_Image_CreateImageFrom(
		data->width
		, data->height
		,data->bytes_per_pixel
		,NULL // we don't want to copy pixels
    );

	ZG_Image_Blit(_src_surface, & _src_rect, dst_surface, NULL);

	return dst_surface;

}


void ZG_Image_Delete(ZG_Image * _this){

	ZG_ImageData *data=_this->data;

	free(data->pixels);

	ZG_FREE(data);
	ZG_FREE(_this);

}
