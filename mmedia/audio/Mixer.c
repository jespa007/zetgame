/*
 * Mixer.c
 *
 *  Created on: Sep 22, 2014
 *      Author: jespada
 */
#include "../../mmedia/audio/zg_audio.h"

#define SPLAYER_FREQUENCY	44100
#define SPLAYER_N_CHANNELS  2
#define SPLAYER_SAMPLES		4096

#define MAX_FFM_BLOCKS 4
#define MASK_MAX_FFM_BLOCKS (MAX_FFM_BLOCKS-1)

#define MAX_SAMPLES 20
#define MAX_PLAY_SAMPLES 10


#define MAX_MUSICS 20
#define MAX_PLAY_MUSICS 8


typedef enum{
	SOUND_TYPE_NONE=0,
	SOUND_TYPE_SAMPLE,
	SOUND_TYPE_WAV,
	SOUND_TYPE_FFMPEG,
	SOUND_TYPE_XMP
}SOUND_TYPE;

typedef struct {
	//---------------------------
	// buffer to fill audio mix..
	uint8_t *		block_ptr[MAX_FFM_BLOCKS];//[SPLAYER_FRAME_SIZE];
	SOUND_TYPE  block_len[MAX_FFM_BLOCKS];
	uint8_t 		n_read_block;
	uint8_t 		n_write_block;
	//-------------------------
}BufferWaveMusic;

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
	BufferWaveMusic *wave_buffer;

	char file[512];

}MixerSound;

typedef struct{
	uint8_t 			n_playing;
	MixerSound 		loaded[MAX_MUSICS];
	MixerSound *	playing[MAX_PLAY_MUSICS];
}MixerMusics;

typedef struct{
	MixerSound 		loaded[MAX_SAMPLES];
}MixerSamples;

typedef struct{

	SDL_AudioCVT * 	cvt_16b_to_audio;
	AudioFormat 	current_format;
	int 			bytes_per_sample;
	uint_t 			frame_size;

	SDL_AudioSpec 	sdl_player_settings;

	MixerMusics 	musics;
	MixerSamples 	samples;

}MixerVars;

MixerVars *g_mixer_vars=NULL;



bool Mixer_ConvertAudio(SDL_AudioSpec * wav_spec, uint8_t **wav_buffer,	uint32_t *wav_length);
int Mixer_GetSDLFormat(void);
void Mixer_AudioCallback(void *udata, uint8_t *stream, int len);
void Mixer_ConfigureAudioFormat(AudioFormat format);

//int MIXER_isEffect(int id);
void Sample_AudioCallback(uint8_t *stream, uint_t len);
void Music_AudioCallback(uint8_t *stream, uint_t len);
void Music_Update(void);
void Sample_StopAll(void);
void Music_StopAll(void);
void Sample_UnloadAll(void);
void Music_UnloadAll(void);


uint_t Mixer_LengthSamplesToTime(uint_t time){
	return (((((time)*1000)/(SPLAYER_N_CHANNELS*g_mixer_vars->bytes_per_sample))/SPLAYER_FREQUENCY));
}

uint_t Mixer_TimeToLengthSamples(uint_t time){
	return ((time)*SPLAYER_FREQUENCY*SPLAYER_N_CHANNELS*g_mixer_vars->bytes_per_sample)/1000;
}


void Mixer_AudioCallback(void *udata, uint8_t *stream, int len)
{

	// extern const uint8_t *mixData;
	SDL_memset(stream, 0, len);  // make sure this is silence.
	//SDL_memset(stream_aux, 0, SPLAYER_FRAME_SIZE);

	Sample_AudioCallback(stream, len);
	Music_AudioCallback(stream,len);
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// UTILS
//


bool Mixer_ConvertAudio(SDL_AudioSpec * wav_spec, uint8_t **wav_buffer,	uint32_t *wav_length) {

	if(g_mixer_vars == NULL){
		Log_Error("Mixer not init");
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
			Log_Error("Error converting sound : %s", SDL_GetError());
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

int Mixer_GetSDLFormat(void){

	switch(g_mixer_vars->current_format){
	case AUDIO_FORMAT_INT_16:
		return AUDIO_S16;
		break;
	case AUDIO_FORMAT_FLOAT_32:
		return AUDIO_F32;
		break;
	}

	Log_Error("Format not supported %i. Return 16 bits (default)",g_mixer_vars->current_format);
	return AUDIO_S16;
}

void Mixer_ConfigureAudioFormat(AudioFormat format){
	switch(format){
	case AUDIO_FORMAT_INT_16:
		g_mixer_vars->bytes_per_sample=2;
		break;
	case AUDIO_FORMAT_FLOAT_32:
		g_mixer_vars->bytes_per_sample=4;
		break;
	}

	g_mixer_vars->current_format = format;
	g_mixer_vars->frame_size =  (SPLAYER_SAMPLES*SPLAYER_N_CHANNELS*g_mixer_vars->bytes_per_sample);
}

void Mixer_SoundDriverList(void){
	for (uint8_t i = 0; i < SDL_GetNumAudioDrivers(); ++i) {
		Log_Info("Audio driver %d: %s", i, SDL_GetAudioDriver(i));
	}

	Log_Info("current audio driver is %s",SDL_GetCurrentAudioDriver());

	int nbDevice = SDL_GetNumAudioDevices(0);

	for(int i = 0; i < nbDevice; ++i){
		Log_Info("device n°%i : %s",SDL_GetAudioDeviceName(i, 0));
	}
}

bool Mixer_Init(AudioFormat format){

	if(g_mixer_vars != NULL){
		Log_Error("Mixer already init");
		return false;
	}

	//	SDL_AudioSpec audiospec_sdl;
	if(SDL_WasInit(SDL_INIT_AUDIO) != SDL_INIT_AUDIO){
#ifdef _WIN32
		// I don't know why but wasapi driver is broken. It has to init with "direct sound" or driver > 0
		SDL_setenv("SDL_AUDIODRIVER", "directsound", true);
#endif
		if(SDL_InitSubSystem(SDL_INIT_AUDIO)<0){
			Log_Error("Failed to init SDL_InitSubSystem Audio: %s", SDL_GetError());
			return false;
		}
	}

	Mixer_SoundDriverList();

	g_mixer_vars = (MixerVars *)malloc(sizeof(MixerVars));
	memset(g_mixer_vars,0,sizeof(MixerVars));


	SDL_AudioSpec  have;

	SDL_zero(g_mixer_vars->sdl_player_settings);
	g_mixer_vars->sdl_player_settings.freq = SPLAYER_FREQUENCY;
	g_mixer_vars->sdl_player_settings.format = Mixer_GetSDLFormat();//AUDIO_S16;
	g_mixer_vars->sdl_player_settings.channels = SPLAYER_N_CHANNELS;
	g_mixer_vars->sdl_player_settings.samples = SPLAYER_SAMPLES;
	g_mixer_vars->sdl_player_settings.callback = Mixer_AudioCallback;  // you wrote this function elsewhere.

	if (SDL_OpenAudio(&g_mixer_vars->sdl_player_settings, &have) < 0) {
		Log_Error("Failed to open audio: %s", SDL_GetError());
	} else {
		if (have.format != g_mixer_vars->sdl_player_settings.format){
			Log_Error( "Cannot init the audio at (%iHZ %iCH ).",SPLAYER_FREQUENCY, SPLAYER_N_CHANNELS);
		}

		Mixer_ConfigureAudioFormat(g_mixer_vars->current_format);


		if(have.format != AUDIO_S16){
			g_mixer_vars->cvt_16b_to_audio = (SDL_AudioCVT * )malloc(sizeof(SDL_AudioCVT));
			// prepare configure for conversion from audio16 to another audio  type for run time conversion ...
			SDL_BuildAudioCVT(g_mixer_vars->cvt_16b_to_audio, AUDIO_S16, 2, SPLAYER_FREQUENCY,have.format, have.channels, have.freq);

			// set length as constant...
			g_mixer_vars->cvt_16b_to_audio->len = g_mixer_vars->frame_size/g_mixer_vars->cvt_16b_to_audio->len_mult;
			Log_Info("mul:%i ratio:%f request:%i frame: %i"
					,g_mixer_vars->cvt_16b_to_audio->len_mult
					,g_mixer_vars->cvt_16b_to_audio->len_ratio
					,g_mixer_vars->cvt_16b_to_audio->len
					,g_mixer_vars->cvt_16b_to_audio->len*g_mixer_vars->cvt_16b_to_audio->len_mult);

			SDL_assert(g_mixer_vars->cvt_16b_to_audio->needed); // obviously, this one is always needed.
		}

		Log_Info("Init %s %i channels at %iHz",CONST_INT_TO_STRING(AUDIO_S16),have.channels,SPLAYER_FREQUENCY);

		g_mixer_vars->frame_size=have.size;
		//Mixer_StartThread();
		SDL_PauseAudio(0);
		return true;
	}
	return false;
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// GENERAL UPDATE
//

void Mixer_Update(void){
	if(g_mixer_vars == NULL){
		Log_Error("Mixer not init");
		return;
	}
	Music_Update();

}



/*void Mixer_startThread(){
	if(sp_update_thread == NULL){
		SDL_PauseAudio(0);  // start audio playing.

		end_sp_audio_thread=false;
		sp_update_thread = SDL_CreateThread(Mixer_update, "sp_audio_update", NULL); // start thread ....
	}
}

void Mixer_StopThread(){
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
void Mixer_StopAll(void){
	if(g_mixer_vars == NULL){
		Log_Error("Mixer not init");
		return;
	}
	Sample_StopAll();
	Music_StopAll();
}

void Mixer_UnloadAll(void){
	if(g_mixer_vars == NULL){
		Log_Error("Mixer not init");
		return;
	}
	Music_UnloadAll();
	Sample_UnloadAll();

}

void Mixer_DeInit(void){

	if(g_mixer_vars == NULL){
		Log_Error("Mixer not init");
		return;
	}

	Mixer_UnloadAll();
	//Mixer_stopThread();

	if(g_mixer_vars->cvt_16b_to_audio != NULL){
		free(g_mixer_vars->cvt_16b_to_audio);
	}

	SDL_CloseAudio();

	free(g_mixer_vars);
	g_mixer_vars=NULL;

	Log_Info("MIXER_deinit");
}


#include "../../mmedia/audio/Sample.c"
#include "../../mmedia/audio/music/Music.c"
