#include "../../../mmedia/audio/music/xmp_common.h"
#include "../../../mmedia/audio/music/xmp_s.c"

#define 	AUDIO_MODULE_MAX_SAMPLES	32
#define 	AUDIO_MODULE_MAX_CHANNELS	64
#define 	AUDIO_MODULE_MAX_EVENTS		64


typedef struct{
	uint8_t instrument,channel;
}MusicXmpEventInfo;

typedef struct{
	MusicXmpEventInfo event[AUDIO_MODULE_MAX_EVENTS];
	uint8_t pattern;
	int n_events;
}MusicXmpEvents;

typedef struct{
	xmp_context ctx;
	struct xmp_module_info module_info;
	struct xmp_frame_info mi;
	Sint32 current_playing_row;
	Sint32 last_played_row;
	int event_play_channel_instrument[AUDIO_MODULE_MAX_CHANNELS][AUDIO_MODULE_MAX_SAMPLES];
	int last_played[AUDIO_MODULE_MAX_CHANNELS][AUDIO_MODULE_MAX_SAMPLES];

	//---- callback on event...
	void (* callback_on_event)(void *user_data, MusicXmpEvents *m_event_ch_instrumen);
	void *user_data;

}MusicXmp;


bool MusicXmp_LoadFromMemory(MixerSound *sp_info, const unsigned char *ptr, size_t len){

	xmp_context ctx;
	int error;
	//current_playing_row = last_played_row = -1;

	if ((ctx = xmp_create_context()) == NULL) {
		Log_Error("I can't create context!");
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
		Log_Error("Cannot open module from memory: %s!",error_str);
		return false;
	}

	MusicXmp *sp_xmp = (MusicXmp *)malloc(sizeof(MusicXmp));
	memset(sp_xmp,0,sizeof(MusicXmp));

	if(sp_xmp == 0){
		xmp_free_context(ctx);
		Log_Error("Error allocating %i bytes from memory!", sizeof(MusicXmp));
		return false;
	}

	sp_xmp->ctx = ctx;
	xmp_get_module_info (sp_xmp->ctx, &sp_xmp->module_info);

	sp_info->data=sp_xmp;
	sp_info->wave_buffer = Music_NewWaveBuffer();

	//sp_loaded_music[n_loadedSpMusics].length = wav_length;

	strcpy(sp_info->file, "from memory");
	sp_info->type=SOUND_TYPE_XMP;
	sp_info->duration = sp_xmp->module_info.seq_data->duration;
	sp_info->position_play=0;
	sp_info->playing = false;
	sp_info->paused = false;
	sp_info->volume=1;

	return true;
}

bool MusicXmp_IsFileSupported(const char * file){

	long int str_len=strlen(file);

	if(str_len < 4) return false;

	return
			strncmp("s3m", (file+str_len)-3, 3) == 0
		|| 	strncmp("xm", (file+str_len)-2, 2) == 0
		|| 	strncmp("mod", (file+str_len)-3, 3) == 0;

}

bool MusicXmp_Load(MixerSound *sp_info, const char *file){
	BufferByte *buffer=FileSystem_ReadFile(file);

	if(buffer!=NULL){
		bool res=MusicXmp_LoadFromMemory(sp_info, buffer->ptr, buffer->len);

		BufferByte_Delete(buffer);

		return res;
	}
	return false;
}

void MusicXmp_Seek(MixerSound *sp_info, long t_seek){
	int pos;
	MusicXmp * s_xmp;

	printf(" seek time to %lu...",t_seek);
	//printf(" request time to %i...",t_seek);
	s_xmp=(MusicXmp *)sp_info->data;
#ifndef _WIN32
	if((pos = xmp_seek_time2(s_xmp->ctx,t_seek)) == XMP_ERROR_STATE)
#else
	if((pos = xmp_seek_time(s_xmp->ctx,t_seek)) == XMP_ERROR_STATE)
#endif
	{
		fprintf(stderr," error time to %lu...",t_seek);
	}
}

void MusicXmp_Play(MixerSound *sp_info){

	MusicXmp *s_xmp=(MusicXmp *)sp_info->data;
	if(xmp_start_player(s_xmp->ctx,SPLAYER_FREQUENCY,0) != 0){
		Log_Error("Error starting module");
	}
}

/*
bool MusicXmp_SetCallbackOnEvent(int id, void (* callback_on_event)(void *data, MusicXmpFrameInfo *event_ch_ins), void *user_data){

	MixerSound *info = &sp_loaded_music[id-1];


	if(info->type == MIXER_TYPE_XMP){
		MusicXmp *s_xmp=(MusicXmp *)info->data;
		s_xmp->callback_on_event = callback_on_event;
		s_xmp->user_data = user_data;
	}else
		Mixer_SetError("Id %i not type xmp", id);

	return true;

}*/

bool MusicXmp_UpdateEvents(MusicXmp *s_xmp){
	struct xmp_frame_info frame_info;
	signed ch;
	int event=0;
	MusicXmpEvents module_event;
	memset(&module_event,0,sizeof(module_event));

	xmp_get_frame_info( s_xmp->ctx, &frame_info);

	if(s_xmp->last_played_row == frame_info.row) // update is not needed...
		return false;

	s_xmp->last_played_row = frame_info.row;

	memset(s_xmp->event_play_channel_instrument, 0, sizeof(s_xmp->event_play_channel_instrument));

	// process samples played
	module_event.pattern = frame_info.pattern;

	for( ch=0; ch < AUDIO_MODULE_MAX_CHANNELS; ch++) {
		uint8_t ins = frame_info.channel_info[ch].event.ins;

		if(ins > 0)
			ins-=1;

		//int current_channel = frame_info.xmp_channel_info[ch].
		if(ch < AUDIO_MODULE_MAX_CHANNELS && ins < AUDIO_MODULE_MAX_SAMPLES) {
			if(frame_info.channel_info[ch].volume && !s_xmp->last_played[ch][ins] && module_event.n_events < AUDIO_MODULE_MAX_EVENTS){ // and event ocurrs!
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

void MusicXmp_Update(MixerSound *sp_info){
	BufferWaveMusic *wave_buffer = sp_info->wave_buffer;
	uint8_t n_current_block = wave_buffer->n_write_block&MASK_MAX_FFM_BLOCKS;
	MusicXmp *s_xmp = (MusicXmp *)sp_info->data;
	struct xmp_frame_info frame_info;
	int error=0;
	const char *error_str=NULL;


	MusicXmp_UpdateEvents(s_xmp);

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

		Log_Error("Error playing xmp:%s",error_str);
	}

	wave_buffer->block_len[n_current_block]=g_mixer_vars->frame_size;
	wave_buffer->n_write_block= (wave_buffer->n_write_block+1)&MASK_MAX_FFM_BLOCKS;

	// maybe update also frame ??!?!?!
	// updates play intruments and add to stack...
}

void MusicXmp_Unload(MixerSound *sp_info){
	printf("unloading xmp %s \n", sp_info->file);
	MusicXmp *s_xmp=(MusicXmp *)sp_info->data;
	xmp_release_module(s_xmp->ctx);
	xmp_free_context(s_xmp->ctx);
	free(s_xmp);
}
