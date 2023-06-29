#include "_zg_mmedia_audio_.h"


typedef struct{
	uint32_t position;
	bool loop;
	bool is_playing;
} AudioSamplePlay;

typedef struct{
	AudioSamplePlay current_play[ZG_MAX_PLAY_SAMPLES]; // max playing samples at same time..
	bool is_converted;
	uint32_t wav_length;
	uint8_t *wav_buffer;
}AudioSample;



void ZG_Sample_AudioCallback(uint8_t *stream, uint_t len){

	uint_t i=0,length;
	int j=0;


	for(i=0; i < ZG_MAX_SAMPLES; i++){
		if(g_mixer_vars->samples.loaded[i].type != ZG_SOUND_TYPE_NONE){
			AudioSample *eff = (AudioSample *)g_mixer_vars->samples.loaded[i].data;
			for(j=0; j < ZG_MAX_PLAY_SAMPLES;j++){

				if(eff->current_play[j].is_playing){
					length = MIN(eff->wav_length - eff->current_play[j].position,len);
					SDL_MixAudio(stream, eff->wav_buffer+eff->current_play[j].position, length, SDL_MIX_MAXVOLUME*g_mixer_vars->samples.loaded[i].volume);
					eff->current_play[j].position+=length;

					if(length < len){ // last frame ..
						eff->current_play[j].is_playing=0;
					}

				}
			}
		}
	}
}

bool ZG_Sample_IsValid(ZG_SAMPLE id){
	if((id >= 0) && ((id) < ZG_MAX_SAMPLES)){
		return g_mixer_vars->samples.loaded[id].type == ZG_SOUND_TYPE_SAMPLE;
	}
	return false;
}


int ZG_Sample_GetFreeBlock(void){
	int i = 0;
	for(i=0; i < ZG_MAX_SAMPLES; i++){
		if(g_mixer_vars->samples.loaded[i].type == ZG_SOUND_TYPE_NONE)
			return i;
	}
	return ZG_INVALID_IDX;
}

ZG_SAMPLE ZG_Sample_GetFreeSlotToPlay(AudioSamplePlay * ep){
	int i = 0;
	for(i=0; i < ZG_MAX_PLAY_SAMPLES; i++){
		if(!ep[i].is_playing) return i;
	}
	return ZG_INVALID_IDX;
}


ZG_SAMPLE ZG_Sample_LoadFromMemory(unsigned char *ptr,uint32_t size){
	// to be implemented...
	int n_loaded_sample;

	if(strncmp((char *)ptr+0, "RIFF",4) != 0){
		ZG_LOG_ERRORF( "No valid wave format!");
		return ZG_INVALID_IDX;
	}

	if((n_loaded_sample = ZG_Sample_GetFreeBlock())==ZG_INVALID_IDX){
		ZG_LOG_ERRORF( "Max sounds reached!");
		return ZG_INVALID_IDX;
	}


	if(size > 1024L*512L){
		ZG_LOG_ERRORF( "Max size reached (Max is 512Kb!");
		return ZG_INVALID_IDX;
	}

	SDL_AudioSpec wav_spec;
	uint32_t wav_length;
	uint8_t *wav_buffer;


	/* Load the WAV */
	if (SDL_LoadWAV_RW(SDL_RWFromMem(ptr,size), 1,&wav_spec, &wav_buffer, &wav_length) == NULL) {
		ZG_LOG_ERROR( "Could not open effect from memory: %s", SDL_GetError());
		return ZG_INVALID_IDX;
	} else {
		//printf(" loaded effect from memory - %iHz %ich  ",wav_spec.freq, wav_spec.keyframe_tracks);

		int is_converted = ZG_Mixer_ConvertAudio(&wav_spec, &wav_buffer, &wav_length);

		AudioSample * eff = (AudioSample *)malloc(sizeof(AudioSample));
		memset(eff,0,sizeof(AudioSample));

		eff->is_converted = is_converted;
		eff->wav_buffer=wav_buffer;
		eff->wav_length=wav_length;

		g_mixer_vars->samples.loaded[n_loaded_sample].data=eff;
		g_mixer_vars->samples.loaded[n_loaded_sample].length_bytes = wav_length;
		g_mixer_vars->samples.loaded[n_loaded_sample].duration = ZG_Mixer_LengthSamplesToTime(wav_length);
		ZG_MixerSound *sp = &g_mixer_vars->samples.loaded[n_loaded_sample];
		strcpy(sp->file, "from_mem");
		sp->type=ZG_SOUND_TYPE_SAMPLE;
		sp->position_play=0;
		sp->volume = 1;
	}

	return n_loaded_sample;
}

ZG_SAMPLE ZG_Sample_Load(const char *_filename){
	ZG_SAMPLE idx_sound=ZG_INVALID_IDX;
	ZG_BufferByte *buffer=ZG_FileSystem_ReadFile(_filename);

	if(buffer!=NULL){
		idx_sound=ZG_Sample_LoadFromMemory(buffer->ptr, buffer->len);
		ZG_BufferByte_Delete(buffer);
	}
	return idx_sound;
}

bool ZG_Sample_Play(int id){

	if(!ZG_Sample_IsValid(id)){
		ZG_LOG_ERRORF("ID not valid!");
		return false;
	}

	ZG_MixerSound *sp_info = &g_mixer_vars->samples.loaded[id];
	sp_info->volume = 1;
	AudioSample *eff = (AudioSample *)sp_info->data;
	ZG_SAMPLE idx = ZG_Sample_GetFreeSlotToPlay(eff->current_play);

	if(idx!=ZG_INVALID_IDX){ // start play
		eff->current_play[idx].position=0;
		eff->current_play[idx].is_playing=1;
	}

	return true;
}

bool ZG_Sample_Stop(ZG_SAMPLE id){
	if(!ZG_Sample_IsValid(id)){
		ZG_LOG_ERRORF("ID not valid!");
		return false;
	}

	int i=0;
	ZG_MixerSound *sp_info = &g_mixer_vars->samples.loaded[id];
	AudioSample *eff = (AudioSample *)sp_info->data;
	//int idx = ZG_Mixer_GetFreeSlotEffectToPlay(eff->current_play);

	for(i=0; i < ZG_MAX_PLAY_SAMPLES; i++){
		eff->current_play[i].is_playing=0;
	}

	return true;

}

bool ZG_Sample_SetVolume(ZG_SAMPLE id, float vol){
	if(!ZG_Sample_IsValid(id)){
		ZG_LOG_ERRORF("ID not valid!");
		return false;
	}

	ZG_MixerSound *sp_info = &g_mixer_vars->samples.loaded[id];
	sp_info->volume = MAX(MIN(vol,1),0);

	return true;
}

float ZG_Sample_GetVolume(ZG_SAMPLE id){
	if(!ZG_Sample_IsValid(id)){
		ZG_LOG_ERRORF("ID not valid!");
		return 1;
	}

	ZG_MixerSound *sp_info = &g_mixer_vars->samples.loaded[id];
	return sp_info->volume;
}

uint32_t ZG_Sample_GetDuration(ZG_SAMPLE id){
	if(!ZG_Sample_IsValid(id)){
		ZG_LOG_ERRORF("ID not valid!");
		return 0;
	}

	return g_mixer_vars->samples.loaded[id].duration;
}


void ZG_Sample_Unload(ZG_SAMPLE id){

	if(!ZG_Sample_IsValid(id)){
		ZG_LOG_ERRORF("ID not valid!");
		return;
	}
	//
	ZG_MixerSound *sp_info=&g_mixer_vars->samples.loaded[id];
	ZG_SoundType type =sp_info->type;

	if(type == ZG_SOUND_TYPE_SAMPLE){
		AudioSample *s_effect;
		sp_info->type=0; // make it unusued...
		SDL_Delay(25);

		s_effect = (AudioSample *)sp_info->data;
		if(s_effect->is_converted){ // is allocated by malloc
			free(s_effect->wav_buffer);
		}else{
			SDL_FreeWAV(s_effect->wav_buffer);
		}
		free(s_effect);
	}
	else{
		ZG_LOG_ERRORF("ID not type WAV!");
	}
}

void ZG_Sample_StopAll(void){
	//MIXER_stopThread();
	ZG_SAMPLE s;

	for(s = 0; s < ZG_MAX_SAMPLES; s++){
		if(g_mixer_vars->samples.loaded[s].type != ZG_SOUND_TYPE_NONE){
			ZG_Sample_Stop(s);
		}
	}

}

void ZG_Sample_UnloadAll(void){
	//MIXER_stopThread();
	ZG_SAMPLE s;

	for(s = 0; s < ZG_MAX_SAMPLES; s++){
		if(g_mixer_vars->samples.loaded[s].type != ZG_SOUND_TYPE_NONE){
			ZG_Sample_Unload(s);
		}
	}

}
