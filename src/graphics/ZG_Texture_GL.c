#include "_zg_graphics_.h"

typedef struct{
	GLuint texture;
	GLuint internal_format;
}ZG_TextureDataGL;

GLuint ZG_Texture_GL_BytesPerPixelToGLInternalFormat(uint8_t bytes_per_pixel);

void ZG_Texture_GL_New(ZG_Texture *text,GLvoid *_pixels, uint16_t _width, uint16_t _height, uint8_t _bytes_per_pixel ){

	ZG_TextureDataGL* texture_data=ZG_NEW(ZG_TextureDataGL);
	texture_data->internal_format=GL_INVALID_VALUE;
	texture_data->texture=GL_INVALID_VALUE;
	text->texture_data=texture_data;
	texture_data->texture = GL_INVALID_VALUE;
	texture_data->internal_format = GL_INVALID_VALUE;


	if(_width == 0 || _height == 0){
		ZG_LOG_ERRORF("height=0 width=0 texture cannot be rebuilt");
		return;
	}


	glPushAttrib( GL_TEXTURE_BIT);

	//glActiveTexture(_GL_TEXTURE0);
	glEnable(GL_TEXTURE_2D);
	// Have OpenGL generate a texture object handle for us
	glGenTextures( 1, &texture_data->texture );


	// Bind the texture object
	glBindTexture( GL_TEXTURE_2D, texture_data->texture );

	// Set the texture's stretching properties
	 glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	 glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

	 texture_data->internal_format = ZG_Texture_GL_BytesPerPixelToGLInternalFormat(_bytes_per_pixel);


	// Edit the texture object's image data using the information SDL_Surface gives us
	glTexImage2D( GL_TEXTURE_2D,
				  0,
				  texture_data->internal_format,
				  _width,
				  _height,
				  0,
				  texture_data->internal_format,
				  GL_UNSIGNED_BYTE,
				  _pixels);


	text->height=_height;
	text->width=_width;
	text->bytes_per_pixel=_bytes_per_pixel;

	glPopAttrib();
}


void ZG_Texture_GL_Bind(ZG_Texture *text){

	if(text ==NULL) return;

	ZG_TextureDataGL * texture_data = text->texture_data;
	if(text->texture_type==ZG_TEXTURE_TYPE_CUBEMAP) {
		glEnable(GL_TEXTURE_CUBE_MAP);
		glBindTexture (GL_TEXTURE_CUBE_MAP, texture_data->texture);
	}
	else {
		// set matrix transform texture if transform3D is set!
		glEnable(GL_TEXTURE_2D);
		glBindTexture( GL_TEXTURE_2D, texture_data->texture);
	}
}

GLuint ZG_Texture_GL_BytesPerPixelToGLInternalFormat(uint8_t bytes_per_pixel){

	GLuint internal_format = GL_INVALID_VALUE;

	switch(bytes_per_pixel)
	{
		default:
			ZG_LOG_ERROR("Unknown bytes per pixel depth format %i",bytes_per_pixel);
			return GL_INVALID_VALUE;
			break;
		case 1:
			internal_format = GL_ALPHA;
			break;
		case 2:
			#if SDL_BYTEORDER == SDL_LIL_ENDIAN
			internal_format = GL_RGB;
			#else
			internal_format = GL_BGR;
			#endif
			break;
		case 3:
			#if SDL_BYTEORDER == SDL_LIL_ENDIAN
			internal_format = GL_RGB;
			#else
			internal_format = GL_BGR;
			#endif
			break;
		case 4:

			#if SDL_BYTEORDER == SDL_LIL_ENDIAN
			internal_format = GL_RGBA;
			#else
				internal_format = GL_BGRA;
			#endif

			break;
	};

	return internal_format;
}

void ZG_Texture_GL_Unload(ZG_Texture * text) {
	if(text==NULL){
		return;
	}
	ZG_TextureDataGL *texture_data=(ZG_TextureDataGL *)text->texture_data;

	if(texture_data->texture != GL_INVALID_VALUE)
		glDeleteTextures(1, &texture_data->texture);

	texture_data->texture = GL_INVALID_VALUE;
}


int	ZG_Texture_GL_GetHandle(ZG_Texture * _this){
	ZG_TextureDataGL *texture_data=(ZG_TextureDataGL *)_this->texture_data;
	return texture_data->texture;
}

void	ZG_Texture_GL_SetRepeatUV(ZG_Texture * _this, bool _repeat_uv){

	ZG_TextureDataGL *texture_data=(ZG_TextureDataGL *)_this->texture_data;
	glBindTexture( GL_TEXTURE_2D,  texture_data->texture);

	GLuint clamp_method=GL_CLAMP_TO_BORDER;
	if(_repeat_uv){
		clamp_method=GL_REPEAT;
	}

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, clamp_method);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, clamp_method);

}

void		ZG_Texture_GL_SetFilter(ZG_Texture *_this, ZG_TextureFilter _filter){
	ZG_TextureDataGL *texture_data=(ZG_TextureDataGL *)_this->texture_data;
	glBindTexture( GL_TEXTURE_2D,  texture_data->texture);


	switch(_filter){
	default:
		break;
	case ZG_TEXTURE_FILTER_LINEAR:
		 glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
		 glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
		break;
	case ZG_TEXTURE_FILTER_NEAREST:
		 glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
		 glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
		break;
	}
}


bool ZG_Texture_GL_Update(ZG_Texture * _this,uint16_t _x, uint16_t _y,uint16_t _width, uint16_t _height, GLvoid *_pixels, uint8_t _bytes_per_pixel){
	if(_this==NULL){
		return false;
	}

	ZG_TextureDataGL *texture_data=(ZG_TextureDataGL *)_this->texture_data;

	// TODO: Create a texture with width/height
	// update

	if((_x+_width)< 0 || ((_y+_height)< 0)) return false;
	if(_x>_this->width || _y > _this->height) return false;
	if(_this->bytes_per_pixel!=_bytes_per_pixel) return false;


	GLuint pack_pixel = GL_UNSIGNED_BYTE;
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	if(_this->bytes_per_pixel == 2)
	{
		glPixelStorei(GL_UNPACK_ALIGNMENT, 2);
		pack_pixel = GL_UNSIGNED_SHORT_5_6_5;
	}

	// Bind the texture object
	glBindTexture( GL_TEXTURE_2D, texture_data->texture );

	glTexSubImage2D( GL_TEXTURE_2D,
				  0,
				  _x,//pos.x, // offsetX
				  _y,//pos.y, // offsetY
				  _width,//dim.x,
				  _height,//dim.y,
				  texture_data->internal_format,
				  pack_pixel,
				  _pixels );

	glBindTexture(GL_TEXTURE_2D, 0);

	return true;

}

void ZG_Texture_GL_Delete(ZG_Texture *texture){
	ZG_TextureDataGL *texture_data=(ZG_TextureDataGL *)texture->texture_data;

	ZG_Texture_GL_Unload(texture);

	ZG_FREE(texture_data);
}



