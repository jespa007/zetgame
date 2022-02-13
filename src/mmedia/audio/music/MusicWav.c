#include "../zg_audio.h"

typedef struct{
	uint32_t wav_length;
	uint8_t *wav_buffer;
	bool is_converted;
}MusicWave;

BufferWaveMusic * Music_NewWaveBuffer(void);
void Music_FreeWaveBuffer(BufferWaveMusic *wbm);

uint_t Mixer_TimeToLengthSamples(uint_t time);

bool MusicWav_LoadFromMemory(MixerSound *sp_info,uint8_t *ptr, size_t size){
	SDL_AudioSpec wav_spec;
	uint32_t wav_length;
	uint8_t *wav_buffer;

	if(strncmp((char *)ptr+0, "RIFF",4) != 0){
		Log_ErrorF( "No valid wave format!");
		return false;
	}

	/* Load the WAV */
	if (SDL_LoadWAV_RW(SDL_RWFromMem((void *)ptr,size), 1,&wav_spec, &wav_buffer, &wav_length) == NULL) {
		Log_Error( "Could not open effect from memory: %s", SDL_GetError());
		return false;
	} else {
		//printf(" loaded wav from memory - %iHz %ich  ",wav_spec.freq, wav_spec.keyframe_tracks);

		int is_converted = Mixer_ConvertAudio(&wav_spec, &wav_buffer, &wav_length);

		MusicWave * wave = (MusicWave *)malloc(sizeof(MusicWave));
		memset(wave,0,sizeof(MusicWave));
		wave->wav_buffer=wav_buffer;
		wave->wav_length=wav_length;
		wave->is_converted = is_converted;

		sp_info->wave_buffer = Music_NewWaveBuffer();

		sp_info->data=wave;
		sp_info->length_bytes = wav_length;
		sp_info->duration = Mixer_LengthSamplesToTime(wav_length); //((((wav_length)/(BYTES_PER_SAMPLE*SPLAYER_N_CHANNELS)))/SPLAYER_FREQUENCY)*1000;

		strcpy(sp_info->file, "from memory");
		sp_info->type=SOUND_TYPE_WAV;
		sp_info->position_play=0;
		sp_info->volume=1;

		return true;
	}

	return false;
}

bool MusicWav_Load(MixerSound *sp_info,const char *file){

	BufferByte *buffer=FileSystem_ReadFile(file);

	if(buffer!=NULL){
		bool res=MusicWav_LoadFromMemory(sp_info, buffer->ptr, buffer->len);

		BufferByte_Delete(buffer);

		return res;
	}
	return false;
}

void MusicWav_Update(MixerSound *sp_info){
	uint32_t len_play;
	MusicWave *s_wave = (MusicWave*)sp_info->data;
	BufferWaveMusic *wave_buffer = sp_info->wave_buffer;
	uint8_t n_current_block = wave_buffer->n_write_block&MASK_MAX_FFM_BLOCKS;

	if(sp_info->position_play>=s_wave->wav_length){
		sp_info->playing=false;
		return;
	}

	if((int)sp_info->request_seek!=-1){
		sp_info->position_play=Mixer_TimeToLengthSamples(sp_info->request_seek);

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
	wave_buffer->n_write_block= (wave_buffer->n_write_block+1)&MASK_MAX_FFM_BLOCKS;
}

void MusicWav_Unload(MixerSound *sp_info){
	MusicWave *s_wave=(MusicWave *)sp_info->data;
	printf("unloading wav %s \n", sp_info->file);

	if(s_wave->is_converted){
		free(s_wave->wav_buffer);
	}else{
		SDL_FreeWAV(s_wave->wav_buffer);
	}
	free(s_wave);
}
