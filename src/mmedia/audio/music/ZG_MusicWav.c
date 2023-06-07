#include "../_zg_mmedia_audio_.h"

typedef struct{
	uint32_t wav_length;
	uint8_t *wav_buffer;
	bool is_converted;
}ZG_MusicWave;

ZG_BufferWaveMusic * ZG_Music_NewWaveBuffer(void);
void ZG_Music_FreeWaveBuffer(ZG_BufferWaveMusic *wbm);

uint_t ZG_Mixer_TimeToLengthSamples(uint_t time);

bool ZG_MusicWav_LoadFromMemory(ZG_MixerSound *sp_info,uint8_t *ptr, size_t size){
	SDL_AudioSpec wav_spec;
	uint32_t wav_length;
	uint8_t *wav_buffer;

	if(strncmp((char *)ptr+0, "RIFF",4) != 0){
		ZG_Log_ErrorF( "No valid wave format!");
		return false;
	}

	/* Load the WAV */
	if (SDL_LoadWAV_RW(SDL_RWFromMem((void *)ptr,size), 1,&wav_spec, &wav_buffer, &wav_length) == NULL) {
		ZG_Log_Error( "Could not open effect from memory: %s", SDL_GetError());
		return false;
	} else {
		//printf(" loaded wav from memory - %iHz %ich  ",wav_spec.freq, wav_spec.keyframe_tracks);

		int is_converted = ZG_Mixer_ConvertAudio(&wav_spec, &wav_buffer, &wav_length);

		ZG_MusicWave * wave = (ZG_MusicWave *)malloc(sizeof(ZG_MusicWave));
		memset(wave,0,sizeof(ZG_MusicWave));
		wave->wav_buffer=wav_buffer;
		wave->wav_length=wav_length;
		wave->is_converted = is_converted;

		sp_info->wave_buffer = ZG_Music_NewWaveBuffer();

		sp_info->data=wave;
		sp_info->length_bytes = wav_length;
		sp_info->duration = ZG_Mixer_LengthSamplesToTime(wav_length); //((((wav_length)/(BYTES_PER_SAMPLE*SPLAYER_N_CHANNELS)))/ZG_MIXER_FREQUENCY)*1000;

		strcpy(sp_info->file, "from memory");
		sp_info->type=ZG_SOUND_TYPE_WAV;
		sp_info->position_play=0;
		sp_info->volume=1;

		return true;
	}

	return false;
}

bool ZG_MusicWav_Load(ZG_MixerSound *sp_info,const char *file){

	ZG_BufferByte *buffer=ZG_FileSystem_ReadFile(file);

	if(buffer!=NULL){
		bool res=ZG_MusicWav_LoadFromMemory(sp_info, buffer->ptr, buffer->len);

		ZG_BufferByte_Delete(buffer);

		return res;
	}
	return false;
}

void ZG_MusicWav_Update(ZG_MixerSound *sp_info){
	uint32_t len_play;
	ZG_MusicWave *s_wave = (ZG_MusicWave*)sp_info->data;
	ZG_BufferWaveMusic *wave_buffer = sp_info->wave_buffer;
	uint8_t n_current_block = wave_buffer->n_write_block&ZG_MSK_MAX_FFM_BLOCKS;

	if(sp_info->position_play>=s_wave->wav_length){
		sp_info->playing=false;
		return;
	}

	if((int)sp_info->request_seek!=-1){
		sp_info->position_play=ZG_Mixer_TimeToLengthSamples(sp_info->request_seek);

		// avoid position keyframe_tracks not aligned mul of 2...
		sp_info->position_play&=~(1);
		printf( "request_seek %ul - position play %ul",(unsigned int)sp_info->request_seek, (unsigned int)sp_info->position_play);

		sp_info->request_seek=-1;
	}

	len_play = MIN(s_wave->wav_length-sp_info->position_play,g_mixer_vars->frame_size);

	memcpy(wave_buffer->block_ptr[n_current_block],s_wave->wav_buffer+sp_info->position_play, len_play);

	//printf(" Block %2i %3i/%03i bytes %i to fill, %10i bytes original %3i len current block %3i %i ms ", n_current_block,SPLAYER_FRAME_SIZE,sffmpeg->block_len[n_current_block],to_fill,current_bytes_original+=to_fill,sffmpeg->audioFrame->nb_samples*sffmpeg->audioFrame->keyframe_tracks*2,sffmpeg->block_len[n_current_block],(int)(current_bytes_original*ms)/4);
	sp_info->position_play+=g_mixer_vars->frame_size;
	wave_buffer->block_len[n_current_block]=g_mixer_vars->frame_size;
	wave_buffer->n_write_block= (wave_buffer->n_write_block+1)&ZG_MSK_MAX_FFM_BLOCKS;
}

void ZG_MusicWav_Unload(ZG_MixerSound *sp_info){
	ZG_MusicWave *s_wave=(ZG_MusicWave *)sp_info->data;
	printf("unloading wav %s \n", sp_info->file);

	if(s_wave->is_converted){
		free(s_wave->wav_buffer);
	}else{
		SDL_FreeWAV(s_wave->wav_buffer);
	}
	free(s_wave);
}
