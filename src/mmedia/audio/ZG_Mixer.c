/*
 * Mixer.c
 *
 *  Created on: Sep 22, 2014
 *      Author: jespada
 */
#include "_zg_mmedia_audio_.h"

#define ZG_MIXER_FREQUENCY	44100
#define ZG_MIXER_N_CHANNELS  2
#define ZG_MIXER_SAMPLES		4096

#define ZG_MAX_FFM_BLOCKS 4
#define ZG_MSK_MAX_FFM_BLOCKS (ZG_MAX_FFM_BLOCKS-1)

#define ZG_MAX_SAMPLES 20
#define ZG_MAX_PLAY_SAMPLES 10


#define ZG_MAX_MUSICS 20
#define ZG_MAX_PLAY_MUSICS 8


typedef enum{
	ZG_SOUND_TYPE_NONE=0,
	ZG_SOUND_TYPE_SAMPLE,
	ZG_SOUND_TYPE_WAV,
	ZG_SOUND_TYPE_FFMPEG,
	ZG_SOUND_TYPE_XMP
}ZG_SoundType;

typedef struct {
	//---------------------------
	// buffer to fill audio mix..
	uint8_t *		block_ptr[ZG_MAX_FFM_BLOCKS];//[ZG_MIXER_FRAME_SIZE];
	ZG_SoundType  block_len[ZG_MAX_FFM_BLOCKS];
	uint8_t 		n_read_block;
	uint8_t 		n_write_block;
	//-------------------------
}ZG_BufferWaveMusic;

typedef struct{
	int 	type;
	uint_t	request_seek;
	uint_t	position_play;
	void *	data;
	uint_t	length_bytes;
	uint_t	duration;
	float 	volume;
	bool 	playing;
	bool 	paused;
	bool 	stop_request;
	int 	n_slot_play;
	// only for musics...
	ZG_BufferWaveMusic *wave_buffer;

	char file[512];

}ZG_MixerSound;

typedef struct{
	uint8_t 			n_playing;
	ZG_MixerSound 		loaded[ZG_MAX_MUSICS];
	ZG_MixerSound *	playing[ZG_MAX_PLAY_MUSICS];
}ZG_MixerMusics;

typedef struct{
	ZG_MixerSound 		loaded[ZG_MAX_SAMPLES];
}ZG_MixerSamples;

typedef struct{

	SDL_AudioCVT * 	cvt_16b_to_audio;
	ZG_AudioFormat 	current_format;
	int 			bytes_per_sample;
	uint_t 			frame_size;

	SDL_AudioSpec 	sdl_player_settings;

	ZG_MixerMusics 	musics;
	ZG_MixerSamples 	samples;

}ZG_MixerVars;

ZG_MixerVars *g_mixer_vars=NULL;



bool 	ZG_Mixer_ConvertAudio(SDL_AudioSpec * wav_spec, uint8_t **wav_buffer,	uint32_t *wav_length);
int 	ZG_Mixer_GetSDLFormat(void);
void 	ZG_Mixer_AudioCallback(void *udata, uint8_t *stream, int len);
void 	ZG_Mixer_ConfigureAudioFormat(ZG_AudioFormat format);

//int MIXER_isEffect(int id);
void 	ZG_Sample_AudioCallback(uint8_t *stream, uint_t len);
void 	ZG_Music_AudioCallback(uint8_t *stream, uint_t len);
void 	ZG_Music_Update(void);
void 	ZG_Sample_StopAll(void);
void 	ZG_Music_StopAll(void);
void 	ZG_Sample_UnloadAll(void);
void 	ZG_Music_UnloadAll(void);


uint_t ZG_Mixer_LengthSamplesToTime(uint_t time){
	return (((((time)*1000)/(ZG_MIXER_N_CHANNELS*g_mixer_vars->bytes_per_sample))/ZG_MIXER_FREQUENCY));
}

uint_t ZG_Mixer_TimeToLengthSamples(uint_t time){
	return ((time)*ZG_MIXER_FREQUENCY*ZG_MIXER_N_CHANNELS*g_mixer_vars->bytes_per_sample)/1000;
}


void ZG_Mixer_AudioCallback(void *udata, uint8_t *stream, int len)
{
	ZG_UNUSUED_PARAM(udata);
	// extern const uint8_t *mixData;
	SDL_memset(stream, 0, len);  // make sure this is silence.
	//SDL_memset(stream_aux, 0, ZG_MIXER_FRAME_SIZE);

	ZG_Sample_AudioCallback(stream, len);
	ZG_Music_AudioCallback(stream,len);
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// UTILS
//


bool ZG_Mixer_ConvertAudio(SDL_AudioSpec * wav_spec, uint8_t **wav_buffer,	uint32_t *wav_length) {

	if(g_mixer_vars == NULL){
		ZG_Log_ErrorF("Mixer not init");
		return false;
	}

/* Build AudioCVT */
	if((wav_spec->channels != g_mixer_vars->sdl_player_settings.channels) ||
		(wav_spec->format != g_mixer_vars->sdl_player_settings.format) ||
		(wav_spec->freq != g_mixer_vars->sdl_player_settings.freq)
		//(wav_spec.samples != sdl_player_settings.samples)
		)
	{
		// Change 1024 stereo sample frames at 48000Hz from Float32 to Int16.
		SDL_AudioCVT cvt;
		int ret;
		ret = SDL_BuildAudioCVT(&cvt
				, wav_spec->format
				, wav_spec->channels
				, wav_spec->freq
				, g_mixer_vars->sdl_player_settings.format
				, g_mixer_vars->sdl_player_settings.channels
				, g_mixer_vars->sdl_player_settings.freq);

		if(ret == -1){
			ZG_Log_Error("Error converting sound : %s", SDL_GetError());
			return false;
		}else{

			cvt.buf = (uint8_t *)malloc(*wav_length*cvt.len_mult);
			memcpy(cvt.buf, *wav_buffer, *wav_length);
			cvt.len = *wav_length;
			SDL_ConvertAudio(&cvt);
			SDL_FreeWAV(*wav_buffer);

			/* Put the sound data in the slot (it starts playing immediately) */

			*wav_buffer = cvt.buf;
			*wav_length = cvt.len_cvt;

			return true;
		}
	}
	return false;
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// INIT AUDIO
//

int ZG_Mixer_GetSDLFormat(void){

	switch(g_mixer_vars->current_format){
	case ZG_AUDIO_FORMAT_INT_16:
		return AUDIO_S16;
		break;
	case ZG_AUDIO_FORMAT_FLOAT_32:
		return AUDIO_F32;
		break;
	}

	ZG_Log_Error("Format not supported %i. Return 16 bits (default)",g_mixer_vars->current_format);
	return AUDIO_S16;
}

void ZG_Mixer_ConfigureAudioFormat(ZG_AudioFormat format){
	switch(format){
	case ZG_AUDIO_FORMAT_INT_16:
		g_mixer_vars->bytes_per_sample=2;
		break;
	case ZG_AUDIO_FORMAT_FLOAT_32:
		g_mixer_vars->bytes_per_sample=4;
		break;
	}

	g_mixer_vars->current_format = format;
	g_mixer_vars->frame_size =  (ZG_MIXER_SAMPLES*ZG_MIXER_N_CHANNELS*g_mixer_vars->bytes_per_sample);
}

void ZG_Mixer_PrintListSoundDriver(void){
	for (uint8_t i = 0; i < SDL_GetNumAudioDrivers(); ++i) {
		ZG_Log_Info("Audio driver %d: %s", i, SDL_GetAudioDriver(i));
	}

	ZG_Log_Info("current audio driver is %s",SDL_GetCurrentAudioDriver());

	int nbDevice = SDL_GetNumAudioDevices(0);

	for(int i = 0; i < nbDevice; ++i){
		ZG_Log_Info("device n�%i : %s",SDL_GetAudioDeviceName(i, 0));
	}
}

bool ZG_Mixer_Init(void){

	//ZG_UNUSUED_PARAM(format);

	if(g_mixer_vars != NULL){
		ZG_Log_ErrorF("Mixer already init");
		return false;
	}

	//	SDL_AudioSpec audiospec_sdl;
	if(SDL_WasInit(SDL_INIT_AUDIO) != SDL_INIT_AUDIO){
#ifdef _WIN32
		// I don't know why but wasapi driver is broken. It has to init with "direct sound" or driver > 0
		SDL_setenv("SDL_AUDIODRIVER", "directsound", true);
#endif
		if(SDL_InitSubSystem(SDL_INIT_AUDIO)<0){
			ZG_Log_Error("Failed to init SDL_InitSubSystem Audio: %s", SDL_GetError());
			return false;
		}
	}

	//ZG_Mixer_PrintListSoundDriver();

	g_mixer_vars = (ZG_MixerVars *)malloc(sizeof(ZG_MixerVars));
	memset(g_mixer_vars,0,sizeof(ZG_MixerVars));


	SDL_AudioSpec  have;

	SDL_zero(g_mixer_vars->sdl_player_settings);
	g_mixer_vars->sdl_player_settings.freq = ZG_MIXER_FREQUENCY;
	g_mixer_vars->sdl_player_settings.format = ZG_Mixer_GetSDLFormat();//AUDIO_S16;
	g_mixer_vars->sdl_player_settings.channels = ZG_MIXER_N_CHANNELS;
	g_mixer_vars->sdl_player_settings.samples = ZG_MIXER_SAMPLES;
	g_mixer_vars->sdl_player_settings.callback = ZG_Mixer_AudioCallback;  // you wrote this function elsewhere.

	if (SDL_OpenAudio(&g_mixer_vars->sdl_player_settings, &have) < 0) {
		ZG_Log_Error("Failed to open audio: %s", SDL_GetError());
	} else {
		if (have.format != g_mixer_vars->sdl_player_settings.format){
			ZG_Log_Error( "Cannot init the audio at (%iHZ %iCH ).",ZG_MIXER_FREQUENCY, ZG_MIXER_N_CHANNELS);
		}

		ZG_Mixer_ConfigureAudioFormat(g_mixer_vars->current_format);


		if(have.format != AUDIO_S16){
			g_mixer_vars->cvt_16b_to_audio = (SDL_AudioCVT * )malloc(sizeof(SDL_AudioCVT));
			// prepare configure for conversion from audio16 to another audio  type for run time conversion ...
			SDL_BuildAudioCVT(g_mixer_vars->cvt_16b_to_audio, AUDIO_S16, 2, ZG_MIXER_FREQUENCY,have.format, have.channels, have.freq);

			// set length as constant...
			g_mixer_vars->cvt_16b_to_audio->len = g_mixer_vars->frame_size/g_mixer_vars->cvt_16b_to_audio->len_mult;
			ZG_Log_Info("mul:%i ratio:%f request:%i frame: %i"
					,g_mixer_vars->cvt_16b_to_audio->len_mult
					,g_mixer_vars->cvt_16b_to_audio->len_ratio
					,g_mixer_vars->cvt_16b_to_audio->len
					,g_mixer_vars->cvt_16b_to_audio->len*g_mixer_vars->cvt_16b_to_audio->len_mult);

			SDL_assert(g_mixer_vars->cvt_16b_to_audio->needed); // obviously, this one is always needed.
		}

		ZG_Log_Info("Init %s %i channels at %iHz",CONST_INT_TO_STRING(AUDIO_S16),have.channels,ZG_MIXER_FREQUENCY);

		g_mixer_vars->frame_size=have.size;
		//ZG_Mixer_StartThread();
		SDL_PauseAudio(0);
		return true;
	}
	return false;
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// GENERAL UPDATE
//

void ZG_Mixer_Update(void){
	if(g_mixer_vars == NULL){
		ZG_Log_ErrorF("Mixer not init");
		return;
	}
	ZG_Music_Update();

}



/*void ZG_Mixer_startThread(){
	if(sp_update_thread == NULL){
		SDL_PauseAudio(0);  // start audio playing.

		end_sp_audio_thread=false;
		sp_update_thread = SDL_CreateThread(ZG_Mixer_update, "sp_audio_update", NULL); // start thread ....
	}
}

void ZG_Mixer_StopThread(){
	if(sp_update_thread != NULL){

		SDL_PauseAudio(1);  // stop audio playing.

		SDL_Delay(1);

		int threadReturnValue=0;

		//printf(" wait thread");
		end_sp_audio_thread = true;

		SDL_WaitThread(sp_update_thread, &threadReturnValue);
		fprintf(stdout,"Thread returned value: %d\n", threadReturnValue);

		//printf(" done.");
		sp_update_thread = NULL;
	}
}*/
void ZG_Mixer_StopAll(void){
	if(g_mixer_vars == NULL){
		ZG_Log_ErrorF("Mixer not init");
		return;
	}
	ZG_Sample_StopAll();
	ZG_Music_StopAll();
}

void ZG_Mixer_UnloadAll(void){
	if(g_mixer_vars == NULL){
		ZG_Log_ErrorF("Mixer not init");
		return;
	}
	ZG_Music_UnloadAll();
	ZG_Sample_UnloadAll();

}

void ZG_Mixer_DeInit(void){

	if(g_mixer_vars == NULL){
		ZG_Log_ErrorF("Mixer not init");
		return;
	}

	ZG_Mixer_UnloadAll();
	//ZG_Mixer_stopThread();

	if(g_mixer_vars->cvt_16b_to_audio != NULL){
		free(g_mixer_vars->cvt_16b_to_audio);
	}

	SDL_CloseAudio();

	free(g_mixer_vars);
	g_mixer_vars=NULL;

	ZG_Log_InfoF("MIXER_deinit");
}


#include "ZG_Sample.c"
#include "music/ZG_Music.c"
