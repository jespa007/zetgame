/*
 * CMovieGif.cpp
 *
 *  Created on: May 24, 2014
 *      Author: jespada
 */

#include "mmedia/zg_mmedia.h"

namespace zetgine{

	#define MAX_DIGITS_SEQUENCE 6


	 //code adapted by David Olsen from Lanczos filtering article on wikipedia.org




	//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	int CMovieGif::customReadGifFromMem(GifFileType *gif, GifByteType *byteType, int leng) {
		ZG_BufferByte * userData = (ZG_BufferByte *)gif->UserData;

		memcpy(byteType, (char *)userData->data_buffer, leng);

		userData->data_buffer = userData->data_buffer+leng;
		return leng;
	}

	//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	bool CMovieGif::isFilenameMovieGif(const std::string & m_filename){
		std::string db, ge;


		return (string::EndsWith(m_filename, ".gif"));



	}
	/*
	bool CMovieGif::loadGif(	GifFileType *ft){




		if (ft == NULL) {
			  ZG_LOG_ERROR("DGifOpen failed");
			  return false;
		}

		if( DGifSlurp( ft ) != GIF_OK ) {
				return false;
		}

		if( ft->ImageCount <= 0 ) {
				return false;
		}

		int sWidth = ft->SWidth;
		int sHeight = ft->SHeight;


		GifColorType *ptr_pal = ft->SColorMap->Colors;

		for(int fr = 0; fr < ft->ImageCount; fr++) {

			ZG_LOG_INFO("loading (%i/%i)",fr+1, ft->ImageCount);

			int start_x = ft->SavedImages[fr].ImageDesc.Left;
			int start_y = ft->SavedImages[fr].ImageDesc.Top;
			int w = ft->SavedImages[fr].ImageDesc.Width;
			int h = ft->SavedImages[fr].ImageDesc.Height;
			//------------------------------------------------
			// create a 32b sdl surface

			SDL_Surface * dst_surface =
				SDL_CreateRGBSurface (SDL_SWSURFACE, sWidth, sHeight,
						32, //new_image_surface->format->BitsPerPixel,
						RMASK32, GMASK32, BMASK32, AMASK32);

			if(dst_surface== NULL) {
				ZG_LOG_ERROR("Cannot create SDL Surface %s", SDL_GetError());
				return false;
			}

			GraphicsControlBlock GCB;
			DGifSavedExtensionToGCB(ft,
							fr,
							&GCB);


			uint8_t *ptr_src = ft->SavedImages[fr].RasterBits;
			uint32_t *ptr_dst = (uint32_t *)dst_surface->pixels;



			// offset to center frame regarding size gif.
			ptr_dst+=(start_x);
			ptr_dst+=(start_y)*sWidth;

			// read image/s ...
			for( int  ht = 0 ; ht < h; ht++){
				for(int wt =0; wt < w; wt++) {


					(*ptr_dst) = ptr_pal[*ptr_src].Red+
								(ptr_pal[*ptr_src].Green<<8)+
								(ptr_pal[*ptr_src].Blue<<16); //(Put 0xff000000 for alpa if you want to see solid!)

					if(GCB.TransparentColor != -1)
					if(*ptr_src != GCB.TransparentColor){ // if this is not transparency color then, make it solid.
						(*ptr_dst) += (0xff<<24);
					}

					ptr_dst++;
					ptr_src++;
				}
				// offset dest because usign a diferent scanline width from other frames...ptr_src++
				ptr_dst+=(sWidth-w);
			}

			// when is loaded, convert into POWER2...
			//dst_surface = CImage::convertSurface(dst_surface,0);//CImage::POWER_OF_2);
			tAnimationFrame ani={0,0,0};


			ani.img = new CImageSDL(dst_surface);
			ani.m_delay = GCB.DelayTime*10;
			ani.m_backColor = 0;
			if(ft->SBackGroundColor!=255){
			ani.m_backColor =
								ptr_pal[ft->SBackGroundColor].Red+
								(ptr_pal[ft->SBackGroundColor].Green<<8)+
								(ptr_pal[ft->SBackGroundColor].Blue<<16)+
								(0xff<<24);
			}


			switch(GCB.DisposalMode){
			default:
			case DISPOSAL_UNSPECIFIED:
				ani.m_renderAttribs =CMovieGif::ANIFRAME_UNSPECIFIED;
				break;
			case DISPOSE_DO_NOT:
				ani.m_renderAttribs = CMovieGif::ANIFRAME_LEAVE_IN_PLACE;
				break;
			case DISPOSE_BACKGROUND:
				ani.m_renderAttribs = CMovieGif::ANIFRAME_SET_BACKGROUND;
				break;
			case DISPOSE_PREVIOUS:
				ani.m_renderAttribs = CMovieGif::ANIFRAME_RESTORE_PREVIOUS;
				break;
			}
			//ZG_LOG_INFO("dispose mode: %i ",GCB.DisposalMode);
			image_collection.push_back(ani);
		}

		return true;
	}
	*/


	//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	/*bool CMovieGif::loadGif(const uint8_t * userPtr, int userPtrLen) {//,vector<tAnimationFrame> & image_collection,uint32_t bytes_per_pixel) {



		GifFileType *ft;
		int error_code;
		tRawVector userData = {userPtr,userPtrLen};

		ft = DGifOpen(&userData, customReadGifFromMem, &error_code);
		if (ft == NULL) {
			  ZG_LOG_ERROR("DGifOpen failed");
			  return false;
		}

		if( DGifSlurp( ft ) != GIF_OK ) {
				return false;
		}

		if( ft->ImageCount <= 0 ) {
				return false;
		}

		//return loadGif(ft);//,image_collection,bytes_per_pixel);

		DGifCloseFile(ft, &error_code);

		return true;
	}*/
	//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	bool CMovieGif::loadGif(const std::string & m_filename ) {//,vector<tAnimationFrame> & image_collection,uint32_t bytes_per_pixel) {

		int error_code;

		if(ft != NULL){
			unload();
		}

		ft=NULL;

		/*if(PFSG_isInitialized()) {// load from memory ..
			PackedFile fp;

			if(PFSG_open(m_filename.c_str(),&fp)){
				//res = loadGif((uint8_t *)fp.ini_ptr,fp.size);//,image_collection,bytes_per_pixel);
				tRawVector userData = {(uint8_t *)fp.ini_ptr,fp.size};
				ft = DGifOpen(&userData, customReadGifFromMem, &error_code);
			}


		}else{*/
			ft = DGifOpenFileName((const char *)m_filename.c_str(), &error_code);
		//}




		if (ft == NULL) {
			  ZG_LOG_ERROR("DGifOpen failed");
			  return false;
		}

		if( DGifSlurp( ft ) != GIF_OK ) {
			ZG_LOG_ERROR(" NOT GIF ");
			return false;
		}

		if( ft->ImageCount <= 0 ) {
			ZG_LOG_ERROR(" NOT IMAGES ");
				return false;
		}

		//loadGif(ft,image_collection,bytes_per_pixel);

		//DGifCloseFile( ft , &error_code);

		return true;
	}

	/*
	bool CMovieGif::load( const std::string & filename, int properties, int bpp) {
		int 	n_digits;
		std::string 	m_string_base;
		std::string 	m_extension;
		bool ok=false;

		if(chkFileSequence(filename,m_string_base, n_digits, m_extension)){
			m_isSequence = true;
			ok = true;
		}
		else if(StrUtils::EndsWith(filename, ".gif")) {// try to load gif with libGif (Frame support)
			ok= loadGif(filename,m_frame);

		}else ZG_LOG_ERROR("\"%s\": invalid extension .gif?",filename.c_str());

		videoReady = ok;

		return ok;
	}*/

	CMovieGif::CMovieGif(){
		updateNextFrameInternal=NULL;
		ft=NULL;
		gif_frame = NULL;
	}


	bool CMovieGif::load(const std::string & filename, uint32_t width,uint32_t height,uint32_t bytes_per_pixel, SDL_Rect *emmb){

		if(!videoReady){
			/*m_width =  width;
			m_height = height;
			m_bytes_per_pixel =  bytes_per_pixel;*/

			// 24 bit default.
			/*uint32_t rmask=RMASK32,
				   gmask=GMASK32,
				   bmask=BMASK32,
				   amask=0;*/

			switch(bytes_per_pixel) {
			default:
				ZG_LOG_ERROR("Bytes per pixel %i unsupportted", bytes_per_pixel);
				return false;
				break;
			case CImage::RGB16:
				/* rmask=RMASK16;
				  gmask=GMASK16;
				  bmask=BMASK16;
				  amask=AMASK16;*/

				break;
			case CImage::RGB24:
			case CImage::RGBA32:
				//  amask=AMASK32;
				break;
			}


			/*srf_info = SDL_CreateRGBSurface(SDL_SWSURFACE,
					m_width,
					m_height,
					bytes_per_pixel*8,
					rmask,
					gmask,
					bmask,
					amask);
	*/



			/*SDL_Rect dim={0,0,aux->w,aux->h};
			ZG_Image_Blit(aux,&dim,m_transformed_surface,NULL);
	*/
			// provisional ...
			/*m_totalBytesOutFrame = m_width*m_height*m_bytes_per_pixel;
			this->m_pixels = (uint8_t *) malloc(m_width*m_height*m_bytes_per_pixel);//(uint8_t *)srf_info->pixels;
			this->work_raw_frame = (uint8_t *) malloc(m_width*m_height*m_bytes_per_pixel);
			//this->work_raw_frame = NULL;
	*/
			setupWorkFrames(width,height,bytes_per_pixel);

			updateNextFrameInternal = NULL;


			if(loadGif(filename)){
				updateNextFrameInternal=&CMovieGif::updateToNextFrameGif;
				videoReady = true;
				return true;
			}


		}

		return false;
	}



	void CMovieGif::setPosition(uint32_t time_ms){

	}


	uint32_t CMovieGif::getPosition(){
		return 0;
	}



	void CMovieGif::updateToNextFrameGif(){



		if(!videoReady) return;
		// a fest module op (0...n-1)

		//int index = m_currentFrame%m_cachedFrame.size();

		// rotate scale ...
		//tAnimationFrame ani = m_cachedFrame[index];
		//ani.img->setDimension(m_width,m_height);
		//ZG_Image_Blit(srf_info , NULL,m_cachedFrame[index]->getSDLSurface(), NULL);
		//uint32_t * dst_pixels = (uint32_t *)work_raw_frame;

		int sWidth = ft->SWidth;
		int sHeight = ft->SHeight;

		if(gif_frame == NULL){ // reserves for it ..
			//gif_frame = (uint32_t *)malloc(sWidth*sHeight*sizeof(uint32_t));
			if((gif_frame =newSurface(sWidth,sHeight,m_bytes_per_pixel))==NULL)
				return;
	/*					SDL_CreateRGBSurface (SDL_SWSURFACE, sWidth, sHeight,
								15, //new_image_surface->format->BitsPerPixel,
								RMASK16, GMASK16, BMASK16, AMASK16);

			if(gif_frame== NULL) {
				ZG_LOG_ERROR("Cannot create SDL Surface %s", SDL_GetError());
				return;
			}*/
		}

		uint32_t m_delay = 0;


		GifColorType *ptr_pal = ft->SColorMap->Colors;

		//for(int fr = 0; fr < ft->ImageCount; fr++)
		int fr = this->m_currentFrame%ft->ImageCount;

		{

			//ZG_LOG_INFO("loading (%i/%i)",fr+1, ft->ImageCount);

			int start_x = ft->SavedImages[fr].ImageDesc.Left;
			int start_y = ft->SavedImages[fr].ImageDesc.Top;
			int w = ft->SavedImages[fr].ImageDesc.Width;
			int h = ft->SavedImages[fr].ImageDesc.Height;
			//------------------------------------------------
			// create a 32b sdl surface

			/*SDL_Surface * dst_surface =
				SDL_CreateRGBSurface (SDL_SWSURFACE, sWidth, sHeight,
						32, //new_image_surface->format->BitsPerPixel,
						RMASK32, GMASK32, BMASK32, AMASK32);

			if(dst_surface== NULL) {
				ZG_LOG_ERROR("Cannot create SDL Surface %s", SDL_GetError());
				return;
			}*/

			GraphicsControlBlock GCB;
			DGifSavedExtensionToGCB(ft,
							fr,
							&GCB);


			uint8_t *ptr_src = ft->SavedImages[fr].RasterBits;
			uint32_t *ptr_dst = (uint32_t *)gif_frame->pixels;



			// offset to center frame regarding size gif.
			ptr_dst+=(start_x);
			ptr_dst+=(start_y)*sWidth;

			m_delay = GCB.DelayTime*10;
			uint32_t m_backColor = 0;
			if(ft->SBackGroundColor!=255){
			m_backColor =
								ptr_pal[ft->SBackGroundColor].Red+
								(ptr_pal[ft->SBackGroundColor].Green<<8)+
								(ptr_pal[ft->SBackGroundColor].Blue<<16)+
								(0xff<<24);
			}

			// read image/s ...
			for( int  ht = 0 ; ht < h; ht++){
				for(int wt =0; wt < w; wt++) {



					uint32_t dst_pixel = ptr_pal[*ptr_src].Red+
								(ptr_pal[*ptr_src].Green<<8)+
								(ptr_pal[*ptr_src].Blue<<16); //(Put 0xff000000 for alpa if you want to see solid!)

					if(GCB.TransparentColor != -1)
					if(*ptr_src != GCB.TransparentColor){ // if this is not transparency color then, make it solid.
						dst_pixel += (0xff<<24);
					}

					switch(GCB.DisposalMode){
					default:
					case DISPOSAL_UNSPECIFIED:
						//ani.m_renderAttribs =CMovieGif::ANIFRAME_UNSPECIFIED;
						*ptr_dst=m_backColor;
						if((dst_pixel & 0xFF000000)  != 0x00000000) // can combine
							*ptr_dst=dst_pixel;//src_pixels[i];
						break;
					case DISPOSE_DO_NOT:
						//ani.m_renderAttribs = CMovieGif::ANIFRAME_LEAVE_IN_PLACE;
						if((dst_pixel & 0xFF000000)  != 0x00000000) // can combine
							*ptr_dst=dst_pixel;
						break;
					case DISPOSE_BACKGROUND:
						//ani.m_renderAttribs = CMovieGif::ANIFRAME_SET_BACKGROUND;
						*ptr_dst=dst_pixel;
						break;
					//case DISPOSE_PREVIOUS:
						//ani.m_renderAttribs = CMovieGif::ANIFRAME_RESTORE_PREVIOUS;
						break;
					}

					ptr_dst++;
					ptr_src++;
				}
				// offset dest because usign a diferent scanline width from other frames...ptr_src++
				ptr_dst+=(sWidth-w);
			}


			// when is loaded, convert into POWER2...
			//dst_surface = CImage::convertSurface(dst_surface,0);//CImage::POWER_OF_2);
			//tAnimationFrame ani={0,0,0};


			//CImageSDL *img = new CImageSDL(dst_surface);
			//img->setDimension(m_width)
			//SDL_Surface *dst_surface = CImageSDL::createSurfaceFrom(sWidth, sHeight, 4 ,(uint8_t *)gif_frame);

			//SDL_Surface *aux2 = rotozoomSurfaceXY(gif_frame,0, (float)m_width/(float)gif_frame->w, (float)m_height/(float)gif_frame->h, 0);
			SDL_Surface *aux2 = newSurface(gif_frame->w,gif_frame->h,gif_frame->format->BytesPerPixel);
			if(SDL_BlitScaled(gif_frame,NULL,aux2,NULL) != 0){

			}


			//SDL_FreeSurface(dst_surface);
			//SDL_BlitSurface(aux2, NULL,srf_info , NULL);

			//SDL_LockMutex(mutex);
			//m_imageFrame.setImagePropertiesFrom(m_cachedFrame[index]);
			memcpy(work_raw_frame[n_write],(uint8_t *)aux2->pixels, m_totalBytesOutFrame);//m_cachedFrame[index]->getPixels();

			//SDL_UnlockMutex(mutex);


			SDL_FreeSurface(aux2);
		}

			/*switch(GCB.DisposalMode){
			default:
			case DISPOSAL_UNSPECIFIED:
				//ani.m_renderAttribs =CMovieGif::ANIFRAME_UNSPECIFIED;
				dst_pixels[i]=ani.m_backColor;
				if((src_pixels[i] & 0xFF000000)  != 0x00000000) // can combine
					dst_pixels[i]=src_pixels[i];
				break;
			case DISPOSE_DO_NOT:
				//ani.m_renderAttribs = CMovieGif::ANIFRAME_LEAVE_IN_PLACE;
				if((src_pixels[i] & 0xFF000000)  != 0x00000000) // can combine
					dst_pixels[i]=src_pixels[i];
				break;
			case DISPOSE_BACKGROUND:
				//ani.m_renderAttribs = CMovieGif::ANIFRAME_SET_BACKGROUND;
				dst_pixels[i]=src_pixels[i];
				break;
			//case DISPOSE_PREVIOUS:
				//ani.m_renderAttribs = CMovieGif::ANIFRAME_RESTORE_PREVIOUS;
				break;
			}*/
			//ZG_LOG_INFO("dispose mode: %i ",GCB.DisposalMode);

		//}

		//uint32_t * src_pixels = (uint32_t *)m_cachedFrame[index].img->getSDLSurface()->pixels;


		/*{

			for(int i=0; i< m_width*m_height; i++){

				switch(ani.m_renderAttribs){
				default:
				case ANIFRAME_UNSPECIFIED: // non transparent frame

					dst_pixels[i]=ani.m_backColor;
					if((src_pixels[i] & 0xFF000000)  != 0x00000000) // can combine
						dst_pixels[i]=src_pixels[i];
					break;

				case ANIFRAME_LEAVE_IN_PLACE:

					if((src_pixels[i] & 0xFF000000)  != 0x00000000) // can combine
						dst_pixels[i]=src_pixels[i];
					break;

				case ANIFRAME_SET_BACKGROUND: // transparent frame...
					dst_pixels[i]=src_pixels[i];
					break;


				}
			}
		}*/
		//memcpy(work_raw_frame,m_cachedFrame[index].img->getSDLSurface()->pixels, m_totalBytesOutFrame);//m_cachedFrame[index]->getPixels();





		//--- delay to next frame...
		SDL_Delay(m_delay*m_speed);


	}

	void CMovieGif::unload(){

		int error_code;

		//CMovie::unload();
		SDL_FreeSurface(gif_frame);

		DGifCloseFile(ft, &error_code);
		//for(unsigned i = 0; i < m_cachedFrame.size(); i++)
		//	delete m_cachedFrame[i].img;

		//m_cachedFrame.clear();
	}


	void CMovieGif::updateToNextFrame(){


		//printf("\nup");
		if(updateNextFrameInternal)
			((*this).*updateNextFrameInternal)();
		CMovie::updateToNextFrame();

	}

	CMovieGif::~CMovieGif(){

		unload();

	}
}
