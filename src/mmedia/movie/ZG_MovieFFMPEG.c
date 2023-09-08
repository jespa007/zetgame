#include "mmedia/@zg_mmedia.h"

#ifdef __WITH_FFMPEG__

namespace zetgine{

	const CMovieFFMPEG::tInfoMovieFile CMovieFFMPEG::ffmpegMovieExtensions[MAX_FFMPEG_MOVIE_EXTENSIONS]={
			{AVI,".avi"}
			//".gif", --> since a gif can be a variable framerate, is not recomended to use as movie.
			,{MP4,".mp4"}
			,{MOV,".mov"}
			,{FLV,".flv"}
	};

	std::string CMovieFFMPEG::createFilterSupportedFormats(){
		std::string filter="";

		for(int i = 0; i < MAX_FFMPEG_MOVIE_EXTENSIONS;i++){
			if(i>0){
				filter+="|";
			}

			filter+="*";
			filter+=ffmpegMovieExtensions[i].ext;

		}

		return filter;
	}

	bool CMovieFFMPEG::isFilenameFFMpegExtension(const std::string & m_filename) {

		for(int i = 0; i < MAX_FFMPEG_MOVIE_EXTENSIONS;i++){
			if(string::EndsWith(m_filename, ffmpegMovieExtensions[i].ext)){
				return true;
			}
		}

		return false;
	}


	//
	// FACTORY MANAGEMENT ...
	//
	//----------------------------------------------------------------------------------------------------------------------------------------v

	int readFunc(void* ptr, uint8_t* buf, int buf_size)
	{
		auto& me = *reinterpret_cast<CMemoryStream*>(ptr);
		size_t len = me.read(reinterpret_cast<char*>(buf), buf_size);
		if(len == 0){
			return AVERROR_EOF;
		}

		return (int)len;
	}

	// whence: SEEK_SET, SEEK_CUR, SEEK_END (like fseek) and AVSEEK_SIZE
	int64_t seekFunc(void* ptr, int64_t pos, int whence)
	{
		// Quelle Abfragen:
		auto& me = *reinterpret_cast<CMemoryStream*>(ptr);

		if (whence == AVSEEK_SIZE) {
			// return the file size if you wish to
		}

		int rs = me.seek((uint32_t)pos, whence);
		if (rs != 0) {
			return -1;
		}
		uint32_t fpos = me.tell(); // int64_t is usually uint32_t uint32_t
		return (int64_t)fpos;

	}


	CMovieFFMPEG::CMovieFFMPEG(){
		img_convert_ctx = NULL;
		bufferOut = NULL;
		pFrameConversionRGB = NULL;
		pFrame = NULL;
		pCodecCtx = NULL;
		pFormatCtx = NULL;
		request_seek=-1;
		currentPts=0;
		pBufferIO = NULL;
		pIOCtx = NULL;
		pInStream = NULL;
		videoStreamIndex = -1;
		videoStream = NULL;
		pCodec = NULL;
		streamIndex = -1;
		optStream = NULL;
		destSDL_Format = PIX_FMT_NONE;
		dFormat = PIX_FMT_NONE;
		m_uuid = CUUID::newUUID();
	}


	bool CMovieFFMPEG::setup(uint32_t width, uint32_t height, uint32_t bytes_per_pixel) {
		if(pFormatCtx != NULL && !videoReady){

			switch(bytes_per_pixel) {
			default:
				ZG_LOG_ERROR("Bytes per pixel %i unsupportted", bytes_per_pixel);
				return false;
				break;
			case CImage::RGB16:

				// dest format ...
	#if SDL_BYTEORDER == SDL_LIL_ENDIAN
				dFormat = PIX_FMT_RGB565LE;
	#else
				dFormat = PIX_FMT_RGB565BE;
	#endif
				break;
			case CImage::RGB24:
	#if SDL_BYTEORDER == SDL_LIL_ENDIAN
				dFormat = PIX_FMT_RGB24;
	#else
				dFormat = PIX_FMT_BGR24;
	#endif
				break;
			case CImage::RGBA32:
	#if SDL_BYTEORDER == SDL_LIL_ENDIAN
				dFormat = PIX_FMT_RGBA;
	#else
				dFormat = PIX_FMT_BGRA;
	#endif
				break;
			}

			setupWorkFrames(width,height,bytes_per_pixel);

			// Retrieve stream information
			if(avformat_find_stream_info(pFormatCtx, &optStream)<0) {
				ZG_LOG_ERROR("Couldn't find stream information");
				return false;
			}

			// provisional ...
			//findMovieStream(AvFormatContext *ctx)
			videoStreamIndex=-1;
			for(unsigned int i=0; i<pFormatCtx->nb_streams; i++)
				if(pFormatCtx->streams[i]->codec->codec_type==AVMEDIA_TYPE_VIDEO) {
					videoStreamIndex=i;
					break;
				}
			if(videoStreamIndex==-1)
			{
				ZG_LOG_ERROR("Didn't find a video stream");
				return false; //
			}


			videoStream = pFormatCtx->streams[videoStreamIndex];

			// Get a pointer to the codec context for the video stream
			pCodecCtx=pFormatCtx->streams[videoStreamIndex]->codec;

			//m_infoFPS = (float)videoStream->r_frame_rate.den / videoStream->r_frame_rate.num;

			// Find the decoder for the video stream
			pCodec=avcodec_find_decoder(pCodecCtx->codec_id);
			if(pCodec==NULL) {
				fprintf(stderr, "Unsupported codec!\n");
				return false; // Codec not found
			}
			// Open codec


			if (avcodec_open2(pCodecCtx, pCodec, &optStream) < 0) {
				ZG_LOG_ERROR("Could not open codec");
				return false;
			}

			// alloc AVFrame to read packets from stream ...
			pFrame=av_frame_alloc();

			img_convert_ctx =
					sws_getContext(
							pCodecCtx->width,
							pCodecCtx->height,
							pCodecCtx->pix_fmt,
							m_width,
							m_height,
							dFormat,
							SWS_X,
							NULL, NULL, NULL
					);

			//-------
			// FRAME 2
			// Allocate another AVFrame for our conversions to RGB SDL Surface
			pFrameConversionRGB=av_frame_alloc();
			if(pFrameConversionRGB==NULL)
			{
				ZG_LOG_ERROR("Cannot allocate RGB frame");
				return false;
			}
			// Determine required buffer size and allocate dest buffer
			m_totalBytesOutFrame=avpicture_get_size(
					dFormat,
					m_width,
					m_height);
			bufferOut=(uint8_t *)av_malloc(m_totalBytesOutFrame*sizeof(uint8_t));

			avpicture_fill((AVPicture *)pFrameConversionRGB, bufferOut,
					dFormat,
					m_width,
					m_height
			);

			//avcodec_flush_buffers(pFormatCtx->streams[videoStreamIndex]->codec);

			//setPosition(0);

			// create sdl stuff...

			//print_video_info("");
			//m_fps = 30;//getFPS();
			//m_fps_ms=(uint32_t)((1000.0/(double)m_fps)+0.5);


			return true;
		}

		return false;

	}


	double CMovieFFMPEG::getFPS()
	{
		double fps = 0;

		if(videoStream != NULL && pCodecCtx != NULL){

			if((videoStream->r_frame_rate.den == 0) && (pCodecCtx->time_base.num || pCodecCtx->ticks_per_frame)){
				ZG_LOG_ERROR("Cannot get current FPS (gif or raw video load ?)");
				return 0;
			}

			if(videoStream->r_frame_rate.den==0)
				fps = pCodecCtx->time_base.den /
				pCodecCtx->time_base.num /
				pCodecCtx->ticks_per_frame;
			else
				fps = videoStream->r_frame_rate.num / videoStream->r_frame_rate.den;
		}
		return fps;

	}

	void CMovieFFMPEG::print_video_info(){

		if(pFormatCtx!= NULL){
			av_dump_format(pFormatCtx, 0, "", 0);
		}
	}

	bool CMovieFFMPEG::load( const uint8_t * ptr, unsigned int ptr_len, uint32_t width,uint32_t height,uint32_t bytes_per_pixel){

		if(!videoReady){
			// IStream-Interface that was already created elsewhere:


			pInStream = new  CMemoryStream((char *)ptr, ptr_len );



			// Create internal Buffer for FFmpeg:
			const int iBufSize = 32 * 1024;
			pBufferIO = (unsigned char *)malloc(iBufSize + FF_INPUT_BUFFER_PADDING_SIZE);//new uint8_t[iBufSize];

			// Allocate the AVIOContext:
			// The fourth parameter (pStream) is a user parameter which will be passed to our callback functions
			pIOCtx = avio_alloc_context(pBufferIO, iBufSize,  // internal Buffer and its size
					0,                  // bWriteable (1=true,0=false)
					pInStream,          // user data ; will be passed to our callback functions
					FFmpeg_ReadFunc,
					0,                  // Write callback function (not used in this example)
					FFmpeg_SeekFunc);

			// Allocate the AVFormatContext:
			pFormatCtx= avformat_alloc_context();

			// Set the IOContext:
			pFormatCtx->pb = pIOCtx;

			pFormatCtx->flags = AVFMT_FLAG_CUSTOM_IO;

			// Determining the input format:
			/*ULONG ulReadBytes = 0;

			if(pInStream->Read(pBufferIO, iBufSize, &ulReadBytes)){

			}
				// Error Handling...

			// Don't forget to reset the data pointer back to the beginning!
			if(pInStream->Seek(0, SEEK_SET)){

			}*/
			// Error Handling...

			// Now we set the ProbeData-structure for av_probe_input_format:
			AVProbeData probeData;
			probeData.buf = (unsigned char *)ptr;
			probeData.buf_size = 4096;
			probeData.filename = "";

			// Determine the input-format:
			pFormatCtx->iformat = av_probe_input_format(&probeData, 1);


	/*

			if(av_dict_set(&optStream, "b", "2.5M", 0) <0){
				ZG_LOG_ERROR("Couldn't set dict set ");
				return false;
			}*/

			// Open video file
			if(avformat_open_input(&pFormatCtx, "dummyFilename", NULL, NULL/*&optStream*/)<0) {
				ZG_LOG_ERROR("Couldn't get file info");
				return false;
			}





			if(!setup(width, height, bytes_per_pixel))
				return false;

			videoReady = true;
		}

		return videoReady==true;

	}


	bool CMovieFFMPEG::load(const std::string & _filename, uint32_t width, uint32_t height, uint32_t bytes_per_pixel) {

		// load from disk...

		if(!videoReady){

			// Open video file
			if(avformat_open_input(&pFormatCtx, _filename.c_str(), NULL, NULL)<0) {
				ZG_LOG_ERROR("Couldn't open file %s", _filename.c_str());
				return false;
			}
			setup(width, height, bytes_per_pixel);
			videoReady = true;
		}
		return videoReady==true;
	}

	void CMovieFFMPEG::setPosition(uint32_t time_ms){
		// internal implementation set position ...
		if(!videoReady)
			return ;

		request_seek = time_ms;
	}

	uint32_t CMovieFFMPEG::getPosition(){
		// internal implementation set position ...
		if(!videoReady)
			return 0;
		return currentPts;
	}


	void CMovieFFMPEG::updateToNextFrame() {

		if(!videoReady)
			return ;

		int frameFinished;

		av_init_packet(&packet);

		if(request_seek != -1){

			//Convert ts to frame
			request_seek = av_rescale(request_seek, videoStream->time_base.den, videoStream->time_base.num);
			request_seek /= 1000;

			//SEEK
			if (avformat_seek_file(pFormatCtx, videoStreamIndex, INT64_MIN, request_seek, INT64_MAX, 0) < 0) {
				av_log(NULL, AV_LOG_ERROR, "ERROR av_seek_frame: %lu\n", (long unsigned int)request_seek);
			} else {
				avcodec_flush_buffers(pFormatCtx->streams[videoStreamIndex]->codec);
			}
			request_seek = -1;
		}
		int posf = av_read_frame(pFormatCtx, &packet);

		// without this delay after av_read_frame it desn't work
		SDL_Delay(1);


		if(m_options & LOOP){

			if(posf < 0){ // loop
				if(avformat_seek_file(pFormatCtx, videoStreamIndex, INT64_MIN, 0, INT64_MAX, 0) < 0){
					printf("\n cannot seek 1");
				}else{
					avcodec_flush_buffers(pFormatCtx->streams[videoStreamIndex]->codec);
					posf = av_read_frame(pFormatCtx, &packet);

					// without this delay after av_read_frame it desn't work
					SDL_Delay(1);
				}

				if(posf < 0){
					printf("\n cannot seek 2");
					return;
				}
			}
		}

		if(posf >= 0 ) {//av_read_frame(pFormatCtx, &packet)>=0) {

			while(packet.stream_index!=videoStreamIndex){
				av_read_frame(pFormatCtx, &packet);
			}
			// Is this a packet from the video stream?
			if(packet.stream_index==videoStreamIndex) {

				// Decode video frame
				if(avcodec_decode_video2(pCodecCtx, pFrame, &frameFinished,
						&packet) < 0){
					av_log(NULL, AV_LOG_ERROR, "Error decoding frame ");
					return;

				}
				else{

					// Did we get a video frame?
					if(frameFinished)
					{
						// Convert the image from its native format to RGB
						if(img_convert_ctx) {


							// Convert the image into YUV format ...
							sws_scale(		img_convert_ctx,
									pFrame->data,
									pFrame->linesize,
									0,
									pCodecCtx->height,
									pFrameConversionRGB->data,
									pFrameConversionRGB->linesize);


							memcpy(work_raw_frame[n_write], bufferOut ,m_totalBytesOutFrame);//m_width*m_height*m_bytesPerPixel);
							double d_pts;
							if(packet.dts != AV_NOPTS_VALUE) {
								d_pts = av_frame_get_best_effort_timestamp(pFrame);
							} else {
								d_pts = 0;
							}
							d_pts *= av_q2d(pFormatCtx->streams[videoStreamIndex]->time_base);

							currentPts = d_pts*1000;
							CMovie::updateToNextFrame();
						}

						av_free_packet(&packet);
					}
				}

			}
		}
	}

	void CMovieFFMPEG::close() {

		if(optStream)
			av_dict_free(&optStream);

		if(img_convert_ctx)
			sws_freeContext(img_convert_ctx);

		if(bufferOut)
			av_free(bufferOut);

		if(pFrameConversionRGB)
			av_free(pFrameConversionRGB);

		// Free the YUV frame
		if(pFrame)
			av_free(pFrame);

		// Close the codec
		if(pCodecCtx)
			avcodec_close(pCodecCtx);

		// Close the video file
		if(pFormatCtx)
			avformat_close_input(&pFormatCtx);

		if(pIOCtx != NULL)
			av_free(pIOCtx);

		if(pBufferIO!=NULL)
			free(pBufferIO);


		delete pInStream;
	}

	CMovieFFMPEG::~CMovieFFMPEG() {


		close();
	}
}


#endif
