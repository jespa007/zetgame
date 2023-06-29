#include "mmedia/audio/_zg_mmedia_audio_.h"

#include "ZG_MusicFFmpeg.c"
#include "ZG_MusicWav.c"
#include "ZG_MusicXmp.c"



void ZG_Music_AudioCallback(uint8_t *stream, uint_t len){

	int i=0;
	ZG_MixerMusics * musics=&g_mixer_vars->musics;

	for(i=0; i < ZG_MAX_PLAY_MUSICS; i++){

		ZG_MixerSound *sp_info=musics->playing[i];
		if(sp_info!=NULL){

			if(sp_info->playing) {
				if(!sp_info->paused){ // if not is paused...
					ZG_BufferWaveMusic *wave_buffer = sp_info->wave_buffer;

					if((((wave_buffer->n_read_block+1)&ZG_MSK_MAX_FFM_BLOCKS )!= wave_buffer->n_write_block) && (wave_buffer->block_len[wave_buffer->n_read_block]==len)){
						SDL_MixAudio(stream
								, wave_buffer->block_ptr[wave_buffer->n_read_block]
								, len
								, SDL_MIX_MAXVOLUME*sp_info->volume);

						wave_buffer->block_len[wave_buffer->n_read_block]=0;
						wave_buffer->n_read_block = (wave_buffer->n_read_block+1)&ZG_MSK_MAX_FFM_BLOCKS;
					}
				}
			}else{
				musics->playing[i]=NULL;
			}
		}
	}

}

bool ZG_Music_IsValid(ZG_MUSIC id){
	if((id >= 0) && ((id) < ZG_MAX_MUSICS)){
		ZG_SoundType type= g_mixer_vars->musics.loaded[id].type;

		return 		type == ZG_SOUND_TYPE_XMP
				|| 	type == ZG_SOUND_TYPE_FFMPEG
				|| 	type == ZG_SOUND_TYPE_WAV;

	}
	return false;
}

ZG_MUSIC ZG_Music_GetFreeSlotToPlay(void){
	ZG_MUSIC music = 0;
	for(music=0; music < ZG_MAX_PLAY_MUSICS; music++){
		if(g_mixer_vars->musics.playing[music]==NULL) {
			return music;
		}
	}
	return ZG_INVALID_IDX;
}

ZG_MUSIC ZG_Music_GetFreeBlock(void){
	ZG_MUSIC music = 0;
	for(music=0; music < ZG_MAX_MUSICS; music++){
		if(g_mixer_vars->musics.loaded[music].type == ZG_SOUND_TYPE_NONE){
			return music;
		}
	}
	return ZG_INVALID_IDX;
}

ZG_BufferWaveMusic * ZG_Music_NewWaveBuffer(void){
	ZG_BufferWaveMusic *wbm = (ZG_BufferWaveMusic *) malloc(sizeof(ZG_BufferWaveMusic));


	wbm->n_read_block=0;
	wbm->n_write_block=0;

	for(unsigned i = 0; i < ZG_MAX_FFM_BLOCKS; i++){
		wbm->block_len[i]=0;
		wbm->block_ptr[i] = (uint8_t *)malloc(g_mixer_vars->frame_size);
	}

	return wbm;
}

void ZG_Music_FreeWaveBuffer(ZG_BufferWaveMusic *wbm){
	if(wbm!=NULL){
		for(unsigned i = 0; i < ZG_MAX_FFM_BLOCKS; i++){
			free(wbm->block_ptr[i]);// = malloc(SPLAYER_FRAME_SIZE);
		}

		free(wbm);
	}
}


ZG_MUSIC ZG_Music_Load(const char *file){

	ZG_MUSIC sp_n_loaded_music;
	ZG_MixerSound *sp_info;

	if(strlen(file) < 4){
		ZG_LOG_ERRORF("Invalid filename");
		return ZG_INVALID_IDX;
	}

	if(g_mixer_vars == NULL){
		ZG_LOG_ERRORF("Mixer not init");
		return ZG_INVALID_IDX;
	}

	if((sp_n_loaded_music = ZG_Music_GetFreeBlock())==ZG_INVALID_IDX){
		ZG_LOG_ERRORF( "Max sounds reached!");
		return ZG_INVALID_IDX;
	}

	sp_info=&g_mixer_vars->musics.loaded[sp_n_loaded_music];



	if(strncmp("wav", (file+strlen(file))-3, 3) == 0){
		if(ZG_MusicWav_Load(sp_info,file)){
			return sp_n_loaded_music++;
		}
	}

	if(ZG_MusicXmp_IsFileSupported(file)){
		if(ZG_MusicXmp_Load(sp_info,file)){
			return sp_n_loaded_music++;
		}
	}
#ifdef __WITH_FFMPEG__
	else if(ZG_MusicFFmpeg_IsFileSupported(file)){
		if(ZG_MusicFFmpeg_Load(sp_info,file)){
			return sp_n_loaded_music++;
		}
	}
#endif

	ZG_LOG_ERROR("file extension \"%s\" is unsupported",(file+strlen(file)-3));
	return ZG_INVALID_IDX;
}

ZG_MUSIC ZG_Music_LoadFromMemory(unsigned char *ptr,size_t size){

	ZG_MUSIC sp_n_loaded_music;
	ZG_MixerSound *sp_info;

	if(g_mixer_vars == NULL){
		ZG_LOG_ERRORF("Mixer not init");
		return ZG_INVALID_IDX;
	}

	if((sp_n_loaded_music = ZG_Music_GetFreeBlock())==ZG_INVALID_IDX){
		ZG_LOG_ERRORF( "Max sounds reached!");
		return ZG_INVALID_IDX;
	}

	sp_info=&g_mixer_vars->musics.loaded[sp_n_loaded_music];

	// to be implemented (get signatures from memory and try to load)...
	if((ZG_MusicWav_LoadFromMemory(sp_info,ptr,size)) != 0){
		return sp_n_loaded_music++;
	}
	// try to load xmp module

	if(ZG_MusicXmp_LoadFromMemory(sp_info,ptr,size)){ // let's try ffmpeg
		return sp_n_loaded_music++;
	}
#ifdef __WITH_FFMPEG__
	if(ZG_MusicFFmpeg_LoadFromMemory(sp_info,ptr,size)){
		return sp_n_loaded_music++;
	}
#endif

	return ZG_INVALID_IDX;
}


bool ZG_Music_Seek(ZG_MUSIC id, uint_t t_seek){


	if(!ZG_Music_IsValid(id)){
		ZG_LOG_ERRORF("ID not valid!");
		return false;
	}

	ZG_MixerSound *sp_info = &g_mixer_vars->musics.loaded[id];

	switch(sp_info->type)
	{
	default:
		break;
#ifdef __WITH_FFMPEG__
	case ZG_SOUND_TYPE_FFMPEG:

		ZG_MusicFFmpeg_Seek(sp_info,t_seek);//(ZG_MusicFFmpeg*)g_mixer_vars->musics.loaded[id].data);
		break;
#endif
	case ZG_SOUND_TYPE_WAV:
		sp_info->request_seek = 0;
		break;
	case ZG_SOUND_TYPE_XMP:

		ZG_MusicXmp_Seek(sp_info,t_seek);
		break;
	}

	return false;
}

bool ZG_Music_Play(ZG_MUSIC id){
	ZG_MUSIC idx_play;

	if(!ZG_Music_IsValid(id)){
		ZG_LOG_ERRORF("ID not valid!");
		return false;
	}

	if(ZG_Music_IsPlaying(id)) {// reset?...
		ZG_Music_Stop(id);
	}

	idx_play = ZG_Music_GetFreeSlotToPlay();

	if(idx_play != ZG_INVALID_IDX){

		ZG_MixerSound *sp_info = &g_mixer_vars->musics.loaded[id];
		//ZG_MusicXmp *s_xmp;

		switch(sp_info->type){
		case ZG_SOUND_TYPE_XMP:
			ZG_MusicXmp_Seek(sp_info,0);
			ZG_MusicXmp_Play(sp_info);
			break;
		case ZG_SOUND_TYPE_FFMPEG:
#ifdef __WITH_FFMPEG__
			ZG_MusicFFmpeg_Seek(sp_info,0);
#endif
			break;
		case ZG_SOUND_TYPE_WAV:
			break;
		};

		sp_info->position_play=0;
		sp_info->volume = 1;
		sp_info->n_slot_play=idx_play;
		sp_info->playing=true;
		sp_info->paused = false;

		g_mixer_vars->musics.playing[idx_play]=sp_info;


	}
	else{
		ZG_LOG_ERRORF( "Max playing musics!");
		return false;
	}

	g_mixer_vars->musics.n_playing++;

	return true;
}

bool ZG_Music_SetVolume(ZG_MUSIC id, float vol){
	if(!ZG_Music_IsValid(id)){
		ZG_LOG_ERRORF("ID not valid!");
		return false;
	}
	ZG_MixerSound *sp_info = &g_mixer_vars->musics.loaded[id];
	sp_info->volume = MAX(MIN(vol,1),0);

	return true;
}

float ZG_Music_GetVolume(int id){
	if(!ZG_Music_IsValid(id)){
		ZG_LOG_ERRORF("ID not valid!");
		return 1;
	}

	ZG_MixerSound *sp_info = &g_mixer_vars->musics.loaded[id];
	return sp_info->volume;
}

uint32_t ZG_Music_GetDuration(ZG_MUSIC id){
	if(!ZG_Music_IsValid(id)){
		ZG_LOG_ERRORF("ID not valid!");
		return 0;
	}
	return g_mixer_vars->musics.loaded[id].duration;
}

bool ZG_Music_Pause(ZG_MUSIC id){

	if(!ZG_Music_IsValid(id)){
		ZG_LOG_ERRORF("ID not valid!");
		return false;
	}

	g_mixer_vars->musics.loaded[id].paused=true;
	return true;
}

bool ZG_Music_Resume(ZG_MUSIC id){

	if(!ZG_Music_IsValid(id)){
		ZG_LOG_ERRORF("ID not valid!");
		return false;
	}

	g_mixer_vars->musics.loaded[id].paused=false;

	return true;
}

bool ZG_Music_Stop(ZG_MUSIC id){

	if(!ZG_Music_IsValid(id)){
		ZG_LOG_ERRORF("ID not valid!");
		return false;
	}

	g_mixer_vars->musics.loaded[id].request_seek = 0;
	g_mixer_vars->musics.loaded[id].playing=false;
	while(ZG_Music_IsPlaying(id)){
		SDL_Delay(1);
	}

	memset(g_mixer_vars->musics.loaded[id].wave_buffer->block_len,0,sizeof(g_mixer_vars->musics.loaded[id].wave_buffer->block_len));
	g_mixer_vars->musics.loaded[id].wave_buffer->n_read_block = g_mixer_vars->musics.loaded[id].wave_buffer->n_write_block = 0;

	return true;
}

bool ZG_Music_IsPlaying(ZG_MUSIC id){
	int i=0;
	int m_found = 0;

	if(!ZG_Music_IsValid(id)){
		ZG_LOG_ERRORF("ID not valid!");
		return false;
	}

	for( i = 0; i < ZG_MAX_PLAY_MUSICS && !m_found; i++){
		if(g_mixer_vars->musics.playing[i]==&g_mixer_vars->musics.loaded[id])
			m_found=1;
	}
	return m_found;
}

bool ZG_Music_IsPaused(ZG_MUSIC id){
	if(!ZG_Music_IsValid(id)){
		ZG_LOG_ERRORF("ID not valid!");
		return false;
	}
	return g_mixer_vars->musics.loaded[id].paused==true;
}

bool ZG_Music_UpdateAudio(ZG_MixerSound *sp_info){
	ZG_BufferWaveMusic *wave_buffer = sp_info->wave_buffer;
	if(!sp_info->playing)
		return false;

	uint8_t n_current_block = wave_buffer->n_write_block&ZG_MSK_MAX_FFM_BLOCKS;
	if(wave_buffer->n_read_block == ((n_current_block+1) &ZG_MSK_MAX_FFM_BLOCKS))// && (sffmpeg->block_len[n_current_block] == SPLAYER_FRAME_SIZE)){
	{
		// full buffer wait until sb card has already readed all bytes ...
		return 0;
	}

	switch(sp_info->type){
	case ZG_SOUND_TYPE_XMP:
		ZG_MusicXmp_Update(sp_info);
		break;

	case ZG_SOUND_TYPE_WAV:

		ZG_MusicWav_Update(sp_info);

		// writ enew
		break;
#ifdef __WITH_FFMPEG__
	case ZG_SOUND_TYPE_FFMPEG:
		ZG_MusicFFmpeg_Update(sp_info);
		break;
#endif
	}

	SDL_Delay(1);

	return true;
}

void ZG_Music_Update(void){
	for(int i = 0; i < ZG_MAX_MUSICS; i++){
		if(g_mixer_vars->musics.loaded[i].type != ZG_SOUND_TYPE_NONE){
			if(g_mixer_vars->musics.loaded[i].playing){
				ZG_Music_UpdateAudio(&g_mixer_vars->musics.loaded[i]);
			}
		}
	}
}

uint32_t ZG_Music_GetCurrentTime(int id){

	if(!ZG_Music_IsValid(id)){
		ZG_LOG_ERRORF("ID not valid!");
		return 0;
	}
#ifdef __WITH_FFMPEG__
	ZG_MusicFFmpeg *sffmpeg;
#endif
	ZG_MixerSound *sp_info = &g_mixer_vars->musics.loaded[id];
	ZG_MusicXmp *s_xmp;
	struct xmp_frame_info frame_info;

	switch(sp_info->type)
	{
	default:
	case ZG_SOUND_TYPE_WAV:
		return ZG_Mixer_LengthSamplesToTime(sp_info->position_play);
#ifdef __WITH_FFMPEG__
	case ZG_SOUND_TYPE_FFMPEG:
		sffmpeg = (ZG_MusicFFmpeg *)sp_info->data;
		return (sffmpeg->currentPts);//readingPacket.pts*1000)/ZG_MIXER_FREQUENCY;
#endif
	case ZG_SOUND_TYPE_XMP:
		s_xmp = (ZG_MusicXmp *)sp_info->data;
		xmp_get_frame_info(s_xmp->ctx, &frame_info);
		return frame_info.time;
	}

	return 0;
}

void ZG_Music_Unload(ZG_MUSIC id){

	ZG_MixerSound *sp_info=NULL;

	if(!ZG_Music_IsValid(id)){
		ZG_LOG_ERRORF("ID not valid!");
		return;
	}

	ZG_Music_Stop(id);

	sp_info=&g_mixer_vars->musics.loaded[id];

	ZG_SoundType type =sp_info->type;
	sp_info->type=0; // make it unusued...
	SDL_Delay(25);

	//ZG_MixerSound *sp_info = (ZG_MixerSound *)id;

	ZG_BufferWaveMusic *wave_buffer = sp_info->wave_buffer;

	if(wave_buffer!=NULL){
		ZG_Music_FreeWaveBuffer(wave_buffer);
	}

	switch(type){
	default:
		ZG_LOG_ERROR("Unknown type? %i",type);
		return;
	case ZG_SOUND_TYPE_XMP:
		ZG_MusicXmp_Unload(sp_info);
		break;
	case ZG_SOUND_TYPE_WAV:
		ZG_MusicWav_Unload(sp_info);
		break;
#ifdef __WITH_FFMPEG__
	case ZG_SOUND_TYPE_FFMPEG:
		ZG_MusicFFmpeg_Unload(sp_info);
		break;
#endif
	}

	memset(sp_info,0, sizeof(ZG_MixerSound));
}

void ZG_Music_StopAll(void){
	//MIXER_stopThread();
	ZG_MUSIC m;

	for(m = 0; m < ZG_MAX_MUSICS; m++){
		if(g_mixer_vars->musics.loaded[m].type != ZG_SOUND_TYPE_NONE){
			ZG_Music_Stop(m);
		}
	}
}

void ZG_Music_UnloadAll(void){
	//MIXER_stopThread();
	ZG_MUSIC m;

	for(m = 0; m < ZG_MAX_MUSICS; m++){
		if(g_mixer_vars->musics.loaded[m].type != ZG_SOUND_TYPE_NONE){
			ZG_Music_Unload(m);
		}
	}
}
