#pragma once

/**
 * https://github.com/leandromoreira/ffmpeg-libav-tutorial/issues/29
 *
 */

#ifdef __WITH_FFMPEG__

namespace zetgine{

	/*
	typedef struct
	{
		SDL_Surface *surf;
		SDL_mutex 	*mutex;
		GLuint 		textureHandle;
	}_tctx;*/

	class CMovieFFMPEG: public CMovie {
		//libvlc_media_player_t *vlc_mp;
		//_tctx ctx;



		//static void *lock(void *data, void **p_pixels);
		//static void unlock(void *data, void *id, void *const *p_pixels);
		//static void display(void *data, void *id);


		int videoStreamIndex;
		int64_t request_seek;
		int streamIndex;
		CMemoryStream * pInStream;


		//---- FFMPEG vars -----
		AVFormatContext *pFormatCtx;
		AVCodecContext *pCodecCtx;
		AVCodec *pCodec;
		AVStream *videoStream;
		AVDictionary *optStream;
		AVFrame *pFrame;
		AVFrame *pFrameConversionRGB;
		uint8_t *bufferOut;
		uint8_t* pBufferIO;
		AVIOContext* pIOCtx;


		AVPacket packet;
		struct SwsContext *img_convert_ctx;
		AVPixelFormat dFormat;
		AVPixelFormat destSDL_Format;
		//SDL_Overlay	*sdl_decoded_frame;
		AVPicture pict;
		//--------------------------

		//float m_infoFPS;
		uint32_t currentPts;
		std::string m_uuid;

		static int readFunc(void* ptr, uint8_t* buf, int buf_size);

		// whence: SEEK_SET, SEEK_CUR, SEEK_END (like fseek) and AVSEEK_SIZE
		static int64_t seekFunc(void* ptr, int64_t pos, int whence);


	protected:

		void goToFrame(int frame);
		bool setup(uint32_t width, uint32_t height, uint32_t bytes_per_pixel);

		int decode_packet(int *got_frame, int cached);


	public:

		enum MOVIE_FILE{
			AVI=0,
			MP4,
			MOV,
			FLV,
			//FFMPEG_MOVIE_GIF_EXT,
			MAX_FFMPEG_MOVIE_EXTENSIONS
		};

		typedef struct{
			MOVIE_FILE id;
			const char *ext;
		}tInfoMovieFile;

		static const tInfoMovieFile ffmpegMovieExtensions[MAX_FFMPEG_MOVIE_EXTENSIONS];



		static bool isFilenameFFMpegExtension(const std::string & filename);

		CMovieFFMPEG();

		bool load( const uint8_t * ptr, unsigned int ptr_len, uint32_t width = DEFAULT_MOVIEWIDTH,uint32_t height = DEFAULT_MOVIEHEIGHT,uint32_t bytes_per_pixel = DEFAULT_MOVIEBPP);
		bool load(const std::string & pathName, uint32_t width = DEFAULT_MOVIEWIDTH,uint32_t height = DEFAULT_MOVIEHEIGHT,uint32_t bytes_per_pixel = DEFAULT_MOVIEBPP);
		static std::string createFilterSupportedFormats();
		virtual void print_video_info();
		virtual void setPosition(uint32_t time_ms);
		virtual uint32_t getPosition();
		virtual void  updateToNextFrame();
		virtual double getFPS();







		void close();

		~CMovieFFMPEG();



	};
}

#endif
