#ifdef __WITH_FFMPEG__
#define FFMPEG_INBUF_SIZE 4096


typedef struct{
	AVFormatContext *pFormatCtx;
	AVCodecContext  *aCodecCtx;
	AVCodec         *aCodec;
	AVDictionary        *audioOptionsDict;
	AVPacket readingPacket;
	AVFrame* audioFrame;// = avcodec_alloc_frame();
	AVStream* audioStream;// = formatContext->streams[streamIndex];
	int audioStreamIndex;
	struct SwrContext *swr_ctx;
	//--------------
	void *audio_buf1,*audio_buf;
	int reformat_ctx;
	int audio_src_fmt;
	uint32_t currentPts;
	//--------------
	// Load from memory
	CMemoryStream * pInStream;
	uint8_t *pBufferIO;
	AVIOContext* pIOCtx;

}MusicFFmpeg;



static AVSampleFormat GetFFMPEGFormat();

/**
 * Load FFMPEG
 */



AVSampleFormat MusicFFmpeg_GetFormat(){

	switch(current_format){
	case AudioFormat::FORMAT_INT16:
		return AV_SAMPLE_FMT_S16;
		break;
	case AudioFormat::FORMAT_FLOAT32:
		return AV_SAMPLE_FMT_FLT;
		break;
	}

	fprintf(stderr,"AV FFMPEG Format not supported %i. Return 16 bits (default)",current_format);
	return AV_SAMPLE_FMT_S16;
}

bool MusicFFmpeg_IsFileSupported(){
	long int str_len=strlen(file);

	if(str_len < 4) return false;

	return
			strncmp("ogg", (file+str_len)-3, 3) == 0;

}


int MusicFFmpeg_Load(MixerSound *sp_info,const char *file){

	unsigned i;
	AVFormatContext *pFormatCtx = avformat_alloc_context();
	AVStream *audioStream;
	int audioStreamIndex=0;

	// Open video file
	if(avformat_open_input(&pFormatCtx, file, NULL,NULL)!=0){
		Log_Error( "Cannot open file",file);
		return false; // Couldn't open file
	}

	// Retrieve stream information
	if(avformat_find_stream_info(pFormatCtx, NULL)<0){
		Log_Error( "Cannot finst stream information",file);
		return false; // Couldn't find stream information
	}

	// Dump information about file onto standard error
	av_dump_format(pFormatCtx, 0, file, 0);

	// Find the first video stream
	audioStream = NULL;
	for(i=0; i<pFormatCtx->nb_streams; i++) {
		if(pFormatCtx->streams[i]->codec->codec_type==AVMEDIA_TYPE_AUDIO) {
			audioStreamIndex=i;
			audioStream =pFormatCtx->streams[i];
			break;
		}
	}

	if(audioStream != NULL){
		MusicFFmpeg *sffmpeg= (MusicFFmpeg*)malloc(sizeof(MusicFFmpeg));

		memset(sffmpeg,0,sizeof(MusicFFmpeg));

		sffmpeg->pFormatCtx = pFormatCtx;
		sffmpeg->audioFrame = av_frame_alloc();
		sffmpeg->audioStream=audioStream;
		sffmpeg->audioStreamIndex=audioStreamIndex;
		sffmpeg->aCodecCtx=sffmpeg->audioStream->codec;

		sffmpeg->aCodec = avcodec_find_decoder(sffmpeg->aCodecCtx->codec_id);
		if(!sffmpeg->aCodec) {
			Log_Error( "Unsupported codec!");
			return false;
		}

		if(avcodec_open2(sffmpeg->aCodecCtx, sffmpeg->aCodec, &sffmpeg->audioOptionsDict)<0){
			Log_Error( "can open codec!");
			return false;

		}

		/* create resampler context */
		sffmpeg->swr_ctx = swr_alloc();
		if (!sffmpeg->swr_ctx) {
			Log_Error("Could not allocate resampler context");
			//return -1;
		}
		//av_samples_alloc(&output, avFrame->linesize, AV_CH_LAYOUT_STEREO, 32768, AV_SAMPLE_FMT_S16, 0);
		/* set options */
		av_opt_set_int(sffmpeg->swr_ctx, "in_channel_layout",    sffmpeg->aCodecCtx->channel_layout, 0);
		av_opt_set_int(sffmpeg->swr_ctx, "in_sample_rate",       sffmpeg->aCodecCtx->sample_rate, 0);
		av_opt_set_sample_fmt(sffmpeg->swr_ctx, "in_sample_fmt", sffmpeg->aCodecCtx->sample_fmt, 0);
		av_opt_set_int(sffmpeg->swr_ctx, "out_channel_layout",    AV_CH_LAYOUT_STEREO, 0);
		av_opt_set_int(sffmpeg->swr_ctx, "out_sample_rate",       SPLAYER_FREQUENCY, 0);
		av_opt_set_sample_fmt(sffmpeg->swr_ctx, "out_sample_fmt", GetFFMPEGFormat(), 0);

		if(swr_init(sffmpeg->swr_ctx) < 0){
			Log_Error("can't init swr");
			//return 0;
		}

		av_new_packet(&sffmpeg->readingPacket,   FFMPEG_INBUF_SIZE + FF_INPUT_BUFFER_PADDING_SIZE);

		sp_info->data=sffmpeg;
		sp_info->wave_buffer = newWaveBufferMusic();

		strcpy(sp_info->file, file);
		sp_info->position_play=0;
		sp_info->playing=false;
		sp_info->paused = false;
		sp_info->type=MIXER_TYPE_FFMPEG;
		sp_info->duration = (pFormatCtx->duration/1000);
		sp_info->length_bytes = (sp->duration * SPLAYER_FREQUENCY * SPLAYER_N_CHANNELS * BYTES_PER_SAMPLE)/1000;
		sp_info->volume = 1;


		return true;//(int)sp;
	}

	return false;
}



bool MusicFFmpeg_LoadFromMemory(MixerSound *sp_info,const unsigned char *ptr, size_t len){

	unsigned i;
	// Allocate the AVFormatContext:
	AVFormatContext *pFormatCtx = avformat_alloc_context();
	AVStream *audioStream;
	int audioStreamIndex=0;

	CMemoryStream *pInStream = new  CMemoryStream((char *)ptr, len );



	// Create internal Buffer for FFmpeg:
	const int iBufSize = 32 * 1024;
	unsigned char *pBufferIO = (unsigned char *)av_malloc(iBufSize + FF_INPUT_BUFFER_PADDING_SIZE);//new uint8_t[iBufSize];

	// Allocate the AVIOContext:
	// The fourth parameter (pStream) is a user parameter which will be passed to our callback functions
	AVIOContext* pIOCtx = avio_alloc_context(pBufferIO, iBufSize,  // internal Buffer and its size
											 0,                  // bWriteable (1=true,0=false)
											 pInStream,          // user data ; will be passed to our callback functions
											 FFmpeg_ReadFunc,
											 0,                  // Write callback function (not used in this example)
											 FFmpeg_SeekFunc);

	// Allocate the AVFormatContext:


	// Set the IOContext:
	pFormatCtx->pb = pIOCtx;

	// Determining the input format:
	pFormatCtx->flags = AVFMT_FLAG_CUSTOM_IO;


	//av_dict_set(&optStream, "b", "2.5M", 0);

	// Open video file
	if(avformat_open_input(&pFormatCtx, "dummyFilename", NULL, NULL)<0) {
		delete pInStream;
		avformat_free_context(pFormatCtx);
		av_free(pBufferIO);

		Log_Error("Couldn't get file info");
		return false;
	 }

	// Retrieve stream information
	if(avformat_find_stream_info(pFormatCtx, NULL)<0){
		Log_Error( "Cannot finst stream information");
		delete pInStream;
		avformat_free_context(pFormatCtx);
		av_free(pBufferIO);

		return false; // Couldn't find stream information
	}

	// Dump information about file onto standard error
	av_dump_format(pFormatCtx, 0, "", 0);

	// Find the first video stream
	audioStream = NULL;
	for(i=0; i<pFormatCtx->nb_streams; i++) {
		if(pFormatCtx->streams[i]->codec->codec_type==AVMEDIA_TYPE_AUDIO) {
			audioStreamIndex=i;
			audioStream =pFormatCtx->streams[i];
			break;
		}
	}

	if(audioStream != NULL){
		MusicFFmpeg *sffmpeg= (MusicFFmpeg*)malloc(sizeof(MusicFFmpeg));

		memset(sffmpeg,0,sizeof(MusicFFmpeg));

		sffmpeg->pInStream = pInStream;
		sffmpeg->pBufferIO = pBufferIO;
		sffmpeg->pIOCtx = pIOCtx;
		sffmpeg->pFormatCtx = pFormatCtx;
		sffmpeg->audioFrame = av_frame_alloc();
		sffmpeg->audioStream=audioStream;
		sffmpeg->audioStreamIndex=audioStreamIndex;
		sffmpeg->aCodecCtx=sffmpeg->audioStream->codec;

		sffmpeg->aCodec = avcodec_find_decoder(sffmpeg->aCodecCtx->codec_id);
		if(!sffmpeg->aCodec) {
			Log_Error( "Unsupported codec!");
			return false;
		}

		if(avcodec_open2(sffmpeg->aCodecCtx, sffmpeg->aCodec, &sffmpeg->audioOptionsDict)<0){
			Log_Error( "can open codec!");
			return false;

		}

		/* create resampler context */
		sffmpeg->swr_ctx = swr_alloc();
		if (!sffmpeg->swr_ctx) {
			Log_Error("Could not allocate resampler context");
			//return -1;
		}
		//av_samples_alloc(&output, avFrame->linesize, AV_CH_LAYOUT_STEREO, 32768, AV_SAMPLE_FMT_S16, 0);
		/* set options */
		av_opt_set_int(sffmpeg->swr_ctx, "in_channel_layout",    sffmpeg->aCodecCtx->channel_layout, 0);
		av_opt_set_int(sffmpeg->swr_ctx, "in_sample_rate",       sffmpeg->aCodecCtx->sample_rate, 0);
		av_opt_set_sample_fmt(sffmpeg->swr_ctx, "in_sample_fmt", sffmpeg->aCodecCtx->sample_fmt, 0);
		av_opt_set_int(sffmpeg->swr_ctx, "out_channel_layout",    AV_CH_LAYOUT_STEREO, 0);
		av_opt_set_int(sffmpeg->swr_ctx, "out_sample_rate",       SPLAYER_FREQUENCY, 0);
		av_opt_set_sample_fmt(sffmpeg->swr_ctx, "out_sample_fmt", GetFFMPEGFormat(), 0);

		if(swr_init(sffmpeg->swr_ctx) < 0){
			Log_Error("can't init swr");
			//return 0;
		}

		//av_new_packet(&sffmpeg->readingPacket,   FFMPEG_INBUF_SIZE + FF_INPUT_BUFFER_PADDING_SIZE);
		av_init_packet(&sffmpeg->readingPacket);

		sp_info->data=sffmpeg;
		sp_info->wave_buffer = newWaveBufferMusic();

		//sp_loaded_music[n_loadedSpMusics].length = wav_length;

		strcpy(sp_info->file, "from memory");
		sp_info->position_play=0;
		sp_info->playing=false;
		sp_info->paused = false;
		sp_info->type=MIXER_TYPE_FFMPEG;
		sp_info->duration = (pFormatCtx->duration/1000);
		sp_info->length_bytes = (sp->duration * SPLAYER_FREQUENCY * SPLAYER_N_CHANNELS * BYTES_PER_SAMPLE)/1000;
		sp_info->volume=1;


		return true;//(int)sp;
	}

	return false;
}


void MusicFFmpeg_Seek(MusicFFmpeg *sffmpeg){

	sp_info->request_seek = t_seek;

	if(sp_info->type ==SOUND_TYPE_FFMPEG  && !sp_info->playing){

		uint64_t request_seek = 0;
		request_seek = av_rescale(request_seek, sffmpeg->audioStream->time_base.den, sffmpeg->audioStream->time_base.num);
		request_seek /= 1000;

		//SEEK
		if (avformat_seek_file(sffmpeg->pFormatCtx, sffmpeg->audioStreamIndex, INT64_MIN, request_seek, INT64_MAX, 0) < 0) {
			av_log(NULL, AV_LOG_ERROR, "ERROR av_seek_frame: %lu", (long unsigned int)request_seek);
		} else {
			avcodec_flush_buffers(sffmpeg->aCodecCtx);
		}
	}
}

void MusicFFmpeg_Update(MixerSound *sp_info){
	MusicFFmpeg = *sffmpeg= (MusicFFmpeg*)sp_info->data;
	BufferWaveMusic *wave_buffer = sp_info->wave_buffer;
	uint8_t n_current_block = wave_buffer->n_write_block&MASK_MAX_FFM_BLOCKS;


	if(sp_info->request_seek!=-1){
		 //Convert ts to frame
		uint64_t request_seek = sp_info->request_seek;
		request_seek = av_rescale(request_seek, sffmpeg->audioStream->time_base.den, sffmpeg->audioStream->time_base.num);
		request_seek /= 1000;

		//SEEK
		if (avformat_seek_file(sffmpeg->pFormatCtx, sffmpeg->audioStreamIndex, INT64_MIN, request_seek, INT64_MAX, 0) < 0) {
			av_log(NULL, AV_LOG_ERROR, "ERROR av_seek_frame: %lu", (long unsigned int)request_seek);
		} else {
			//Log_Info("a");
			//av_log(NULL, AV_LOG_VERBOSE, "SUCCEEDED av_seek_frame: %lu newPos:%d", (uint32_t unsigned int)request_seek, pFormatCtx->pb->pos);
			avcodec_flush_buffers(sffmpeg->aCodecCtx);
		}

		sp_info->request_seek=-1;
	}

	av_init_packet(&sffmpeg->readingPacket);

	if(av_read_frame(sffmpeg->pFormatCtx, &sffmpeg->readingPacket) == 0) {

		if (sffmpeg->readingPacket.stream_index == sffmpeg->audioStream->index)
		{
			AVPacket decodingPacket = sffmpeg->readingPacket;

			// Audio packets can have multiple audio frames in a single packet
			while (decodingPacket.size > 0)
			{
				//printf("decodingPacket.size %i",decodingPacket.size);
				// Try to decode the packet into a frame
				int frameFinished = 0;



				if((result = avcodec_decode_audio4(sffmpeg->aCodecCtx, sffmpeg->audioFrame, &frameFinished, &decodingPacket))<0){
					av_log(NULL, AV_LOG_ERROR, "error decoding ...");
					//sp_info->playing=false;
					return 0;
				}

				// Some frames rely on multiple packets, so we have to make sure the frame is finished before
				// we can use it
				if (frameFinished && result >= 0)
				{

					double d_pts;
					if(decodingPacket.dts != AV_NOPTS_VALUE) {
						d_pts = av_frame_get_best_effort_timestamp(sffmpeg->audioFrame);
					} else {
						d_pts = 0;
					}
					d_pts *= av_q2d( sffmpeg->audioStream->time_base);

					sffmpeg->currentPts = d_pts*1000;

					decodingPacket.size -= result;
					decodingPacket.data += result;

					if( av_samples_alloc(	&swr_out_data,
							sffmpeg->audioFrame->linesize,
							2, // is converted by 2 keyframe_tracks always!
							sffmpeg->audioFrame->nb_samples,
							GetFFMPEGFormat(),
							0
					) < 0){
						av_log(NULL, AV_LOG_ERROR, "Cannot alloc");
					}
					else{

						if(swr_convert(sffmpeg->swr_ctx, &swr_out_data, sffmpeg->audioFrame->nb_samples, (uint8_t const **)sffmpeg->audioFrame->extended_data, sffmpeg->audioFrame->nb_samples)<0){
							av_log(NULL, AV_LOG_ERROR, "Cannot convert");

						}
						else{


							to_fill=sffmpeg->audioFrame->nb_samples*SPLAYER_N_CHANNELS*BYTES_PER_SAMPLE;// 2ch < to_fill)

							//printf("\n%i %i %i < %i ?",sffmpeg->audioFrame->nb_samples,av_frame_get_channels(sffmpeg->audioFrame),to_fill,SPLAYER_FRAME_SIZE);

							if(wave_buffer->block_len[n_current_block]+to_fill >= SPLAYER_FRAME_SIZE){
								to_fill = SPLAYER_FRAME_SIZE-wave_buffer->block_len[n_current_block];
							}

							memcpy(&wave_buffer->block_ptr[n_current_block][wave_buffer->block_len[n_current_block]],&swr_out_data[0], to_fill);

							//printf(" Block %2i %3i/%03i bytes %i to fill, %10i bytes original %3i len current block %3i %i ms ", n_current_block,SPLAYER_FRAME_SIZE,sffmpeg->block_len[n_current_block],to_fill,current_bytes_original+=to_fill,sffmpeg->audioFrame->nb_samples*sffmpeg->audioFrame->keyframe_tracks*2,sffmpeg->block_len[n_current_block],(int)(current_bytes_original*ms)/4);
							sp_info->position_play+=to_fill;
							wave_buffer->block_len[n_current_block]+=to_fill;

							if(wave_buffer->block_len[n_current_block]==SPLAYER_FRAME_SIZE){ // next block

								int rest_fill = sffmpeg->audioFrame->nb_samples*SPLAYER_N_CHANNELS*BYTES_PER_SAMPLE - to_fill;
								wave_buffer->n_write_block= (wave_buffer->n_write_block+1)&MASK_MAX_FFM_BLOCKS;
								n_current_block = wave_buffer->n_write_block;

								wave_buffer->block_len[n_current_block]=0;
								if(rest_fill>0){
									wave_buffer->block_len[n_current_block]=rest_fill;
									//printf("*write block %2i %3i/%03i bytes %i", n_current_block,SPLAYER_FRAME_SIZE,sffmpeg->block_len[n_current_block],rest_fill);
									memcpy(wave_buffer->block_ptr[n_current_block],&swr_out_data[to_fill], rest_fill);
									sp_info->position_play+=rest_fill;
								}
								//printf("*Block %2i %3i/%03i %10i bytes %i ms.", n_current_block,SPLAYER_FRAME_SIZE,sffmpeg->block_len[n_current_block],current_bytes_original+=rest_fill,(int)(current_bytes_original*ms)/4);
							}
						}
						av_freep(&swr_out_data);
					}
				}
				else{
					av_log(NULL, AV_LOG_ERROR, " not finished!");
				}
			}
		}

		// You *must* call av_free_packet() after each call to av_read_frame() or else you'll leak memory
		av_free_packet(&sffmpeg->readingPacket);
	}
}

void MusicFFmpeg_Unload(MixerSound *sp_info){
	printf("unloading ffmpeg %s \n", sp_info->file);

	MusicFFmpeg *sffmpeg=(MusicFFmpeg *)sp_info->data;

	// Some codecs will cause frames to be buffered up in the decoding process. If the CODEC_CAP_DELAY flag
	// is set, there can be buffered up frames that need to be flushed, so we'll do that
	if (sffmpeg->aCodecCtx->codec->capabilities & CODEC_CAP_DELAY)
	{
		int frameFinished = 0;
		av_init_packet(&sffmpeg->readingPacket);
		// Decode all the remaining frames in the buffer, until the end is reached

		while (avcodec_decode_audio4(sffmpeg->aCodecCtx, sffmpeg->audioFrame, &frameFinished, &sffmpeg->readingPacket) >= 0 && frameFinished)
		{
		}
	}
	// Close the audio codec
	avcodec_close(sffmpeg->aCodecCtx);

	// Close the audio file
	avformat_close_input(&sffmpeg->pFormatCtx);

	delete sffmpeg->pInStream;

	//av_freep(&swr_out_data[0]);
	swr_free(&sffmpeg->swr_ctx);

	// deallocate sffmpeg struct...
	free(sffmpeg);
}

#endif
