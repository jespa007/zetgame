/*
 * CMovieGif.h
 *
 *  Created on: May 24, 2014
 *      Author: jespada
 */

#pragma once



namespace zetgine{

	class CMovieGif: public CMovie{

		enum{
			ANIFRAME_UNSPECIFIED=       0,      /* No disposal specified. */
			ANIFRAME_LEAVE_IN_PLACE=    1,      /* Leave image in place */
			ANIFRAME_SET_BACKGROUND=    2,      /* Set area too background color */
			ANIFRAME_RESTORE_PREVIOUS=  3       /* Restore to previous content */
		};

		/*typedef struct{
			CImageSDL *img;
			uint32_t m_delay; // custom  delay (if any).
			uint32_t m_renderAttribs; //custom render (if any)
			uint32_t m_backColor; // for gif..
		} tAnimationFrame;
	*/
		GifFileType *ft;

		void updateToNextFrameGif();

		SDL_Surface	*gif_frame;

		void (CMovieGif::*updateNextFrameInternal)();
		static int customReadGifFromMem(GifFileType *gif, GifByteType *byteType, int leng);
		static bool chkFileSequence(const std::string & m_filename, std::string & detected_base, int & detected_digits, std::string & extension);
	public:

		static bool isFilenameMovieGif(const std::string & m_filename);


		//static bool loadGif(	GifFileType *gifFile,vector<tAnimationFrame> & sdl_surface_collection,uint32_t bytes_per_pixel= CImage::RGBA32);
		bool loadGif(const std::string & m_filename);


		CMovieGif();


		//bool load( const std::string & filename, int properties=ImageObject::POWER_OF_2, int bpp=0);
		bool load(const std::string & pathName, uint32_t width = DEFAULT_MOVIEWIDTH,uint32_t height = DEFAULT_MOVIEHEIGHT,uint32_t bytes_per_pixel = DEFAULT_MOVIEBPP, SDL_Rect *emmb=NULL);

		virtual void setPosition(uint32_t time_ms);
		virtual uint32_t getPosition();

		virtual void updateToNextFrame();

		virtual void unload();

		virtual ~CMovieGif();
	};


}
