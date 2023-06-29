#include "xmp_common.h"
#include "xmp_s.c"

#define 	ZG_AUDIO_MODULE_MAX_SAMPLES	32
#define 	ZG_AUDIO_MODULE_MAX_CHANNELS	64
#define 	ZG_AUDIO_MODULE_MAX_EVENTS		64


typedef struct{
	uint8_t instrument,channel;
}ZG_MusicXmpEventInfo;

typedef struct{
	ZG_MusicXmpEventInfo event[ZG_AUDIO_MODULE_MAX_EVENTS];
	uint8_t pattern;
	int n_events;
}ZG_MusicXmpEvents;

typedef struct{
	xmp_context ctx;
	struct xmp_module_info module_info;
	struct xmp_frame_info mi;
	Sint32 current_playing_row;
	Sint32 last_played_row;
	int event_play_channel_instrument[ZG_AUDIO_MODULE_MAX_CHANNELS][ZG_AUDIO_MODULE_MAX_SAMPLES];
	int last_played[ZG_AUDIO_MODULE_MAX_CHANNELS][ZG_AUDIO_MODULE_MAX_SAMPLES];

	//---- callback on event...
	void (* callback_on_event)(void *user_data, ZG_MusicXmpEvents *m_event_ch_instrumen);
	void *user_data;

}ZG_MusicXmp;


bool ZG_MusicXmp_LoadFromMemory(ZG_MixerSound *sp_info, const unsigned char *ptr, size_t len){

	xmp_context ctx;
	int error;
	//current_playing_row = last_played_row = -1;

	if ((ctx = xmp_create_context()) == NULL) {
		ZG_LOG_ERRORF("I can't create context!");
		return false;
	}

	if((error=xmp_load_module_from_memory (ctx,(void *)ptr, len)) != 0) {
		const char *error_str="unknown error";
		switch(error){
		case -XMP_ERROR_FORMAT:
			error_str="unrecognized file format";
			break;
		case -XMP_ERROR_LOAD:
			error_str="the file format was recognized but the file loading failed";
			break;
		case -XMP_ERROR_SYSTEM:
			error_str=" system error";
			break;
		}

		xmp_free_context(ctx);
		ZG_LOG_ERROR("Cannot open module from memory: %s!",error_str);
		return false;
	}

	ZG_MusicXmp *sp_xmp = (ZG_MusicXmp *)malloc(sizeof(ZG_MusicXmp));
	memset(sp_xmp,0,sizeof(ZG_MusicXmp));

	if(sp_xmp == 0){
		xmp_free_context(ctx);
		ZG_LOG_ERROR("Error allocating %i bytes from memory!", sizeof(ZG_MusicXmp));
		return false;
	}

	sp_xmp->ctx = ctx;
	xmp_get_module_info (sp_xmp->ctx, &sp_xmp->module_info);

	sp_info->data=sp_xmp;
	sp_info->wave_buffer = ZG_Music_NewWaveBuffer();

	//sp_loaded_music[n_loadedSpMusics].length = wav_length;

	strcpy(sp_info->file, "from memory");
	sp_info->type=ZG_SOUND_TYPE_XMP;
	sp_info->duration = sp_xmp->module_info.seq_data->duration;
	sp_info->position_play=0;
	sp_info->playing = false;
	sp_info->paused = false;
	sp_info->volume=1;

	return true;
}

bool ZG_MusicXmp_IsFileSupported(const char * file){

	long int str_len=strlen(file);

	if(str_len < 4) return false;

	return
			strncmp("s3m", (file+str_len)-3, 3) == 0
		|| 	strncmp("xm", (file+str_len)-2, 2) == 0
		|| 	strncmp("mod", (file+str_len)-3, 3) == 0;

}

bool ZG_MusicXmp_Load(ZG_MixerSound *sp_info, const char *file){
	ZG_BufferByte *buffer=ZG_FileSystem_ReadFile(file);

	if(buffer!=NULL){
		bool res=ZG_MusicXmp_LoadFromMemory(sp_info, buffer->ptr, buffer->len);

		ZG_BufferByte_Delete(buffer);

		return res;
	}
	return false;
}

void ZG_MusicXmp_Seek(ZG_MixerSound *sp_info, long t_seek){
	int pos;
	ZG_MusicXmp * s_xmp;

	printf(" seek time to %lu...",t_seek);
	//printf(" request time to %i...",t_seek);
	s_xmp=(ZG_MusicXmp *)sp_info->data;
#ifndef _WIN32
	if((pos = xmp_seek_time2(s_xmp->ctx,t_seek)) == XMP_ERROR_STATE)
#else
	if((pos = xmp_seek_time(s_xmp->ctx,t_seek)) == XMP_ERROR_STATE)
#endif
	{
		fprintf(stderr," error time to %lu...",t_seek);
	}
}

void ZG_MusicXmp_Play(ZG_MixerSound *sp_info){

	ZG_MusicXmp *s_xmp=(ZG_MusicXmp *)sp_info->data;
	if(xmp_start_player(s_xmp->ctx,ZG_MIXER_FREQUENCY,0) != 0){
		ZG_LOG_ERRORF("Error starting module");
	}
}

/*
bool ZG_MusicXmp_SetCallbackOnEvent(int id, void (* callback_on_event)(void *data, MusicXmpFrameInfo *event_ch_ins), void *user_data){

	ZG_MixerSound *info = &sp_loaded_music[id-1];


	if(info->type == MIXER_TYPE_XMP){
		ZG_MusicXmp *s_xmp=(ZG_MusicXmp *)info->data;
		s_xmp->callback_on_event = callback_on_event;
		s_xmp->user_data = user_data;
	}else
		ZG_Mixer_SetError("Id %i not type xmp", id);

	return true;

}*/

bool ZG_MusicXmp_UpdateEvents(ZG_MusicXmp *s_xmp){
	struct xmp_frame_info frame_info;
	signed ch;
	int event=0;
	ZG_MusicXmpEvents module_event;
	memset(&module_event,0,sizeof(module_event));

	xmp_get_frame_info( s_xmp->ctx, &frame_info);

	if(s_xmp->last_played_row == frame_info.row) // update is not needed...
		return false;

	s_xmp->last_played_row = frame_info.row;

	memset(s_xmp->event_play_channel_instrument, 0, sizeof(s_xmp->event_play_channel_instrument));

	// process samples played
	module_event.pattern = frame_info.pattern;

	for( ch=0; ch < ZG_AUDIO_MODULE_MAX_CHANNELS; ch++) {
		uint8_t ins = frame_info.channel_info[ch].event.ins;

		if(ins > 0)
			ins-=1;

		//int current_channel = frame_info.xmp_channel_info[ch].
		if(ch < ZG_AUDIO_MODULE_MAX_CHANNELS && ins < ZG_AUDIO_MODULE_MAX_SAMPLES) {
			if(frame_info.channel_info[ch].volume && !s_xmp->last_played[ch][ins] && module_event.n_events < ZG_AUDIO_MODULE_MAX_EVENTS){ // and event ocurrs!
				// maark as event ...
				s_xmp->event_play_channel_instrument[ch][ins] = 1;
				module_event.event[module_event.n_events].instrument = ins;
				module_event.event[module_event.n_events].channel = ch;

				module_event.n_events++;
				event = 1;
				//printf("\n%i %i",ch,ins);
				//printf("event %i", ins);
			}
		}
	}

	if(s_xmp->callback_on_event && event)
		s_xmp->callback_on_event(s_xmp->user_data, &module_event);

	memcpy(s_xmp->last_played, s_xmp->event_play_channel_instrument, sizeof(s_xmp->event_play_channel_instrument));

	return true;
}

void ZG_MusicXmp_Update(ZG_MixerSound *sp_info){
	ZG_BufferWaveMusic *wave_buffer = sp_info->wave_buffer;
	uint8_t n_current_block = wave_buffer->n_write_block&ZG_MSK_MAX_FFM_BLOCKS;
	ZG_MusicXmp *s_xmp = (ZG_MusicXmp *)sp_info->data;
	struct xmp_frame_info frame_info;
	int error=0;
	const char *error_str=NULL;


	ZG_MusicXmp_UpdateEvents(s_xmp);

	xmp_get_frame_info(s_xmp->ctx, &frame_info);
	if(g_mixer_vars->cvt_16b_to_audio != NULL){ // need reconvert audio ...
		error=xmp_play_buffer(
				(char *)s_xmp->ctx
				, (void *)wave_buffer->block_ptr[n_current_block]
				, g_mixer_vars->cvt_16b_to_audio->len, 0);
		//1024 * 2 * 4;  // 1024 stereo float32 sample frames.
		g_mixer_vars->cvt_16b_to_audio->buf = wave_buffer->block_ptr[n_current_block];
		SDL_ConvertAudio(g_mixer_vars->cvt_16b_to_audio);

	}
	else{
		error=xmp_play_buffer((char *)s_xmp->ctx
				, (void *)wave_buffer->block_ptr[n_current_block]
				, g_mixer_vars->frame_size, 0);
	}

	if(error!=0){
		error_str="unknown error";
		switch(error){
		case -XMP_END:
			error_str="module was stopped or the loop counter was reached";
			break;
		case -XMP_ERROR_STATE:
			error_str="the player is not in playing state";
			break;
		}

		ZG_LOG_ERROR("Error playing xmp:%s",error_str);
	}

	wave_buffer->block_len[n_current_block]=g_mixer_vars->frame_size;
	wave_buffer->n_write_block= (wave_buffer->n_write_block+1)&ZG_MSK_MAX_FFM_BLOCKS;

	// maybe update also frame ??!?!?!
	// updates play intruments and add to stack...
}

void ZG_MusicXmp_Unload(ZG_MixerSound *sp_info){
	ZG_LOG_INFO("unloading xmp %s \n", sp_info->file);
	ZG_MusicXmp *s_xmp=(ZG_MusicXmp *)sp_info->data;
	xmp_release_module(s_xmp->ctx);
	xmp_free_context(s_xmp->ctx);
	free(s_xmp);
}
