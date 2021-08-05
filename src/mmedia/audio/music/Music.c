#include "mmedia/audio/zg_audio.h"

#include "mmedia/audio/music/MusicFFmpeg.c"
#include "mmedia/audio/music/MusicWav.c"
#include "mmedia/audio/music/MusicXmp.c"



void Music_AudioCallback(uint8_t *stream, uint_t len){

	int i=0;
	MixerMusics * musics=&g_mixer_vars->musics;

	for(i=0; i < MAX_PLAY_MUSICS; i++){

		MixerSound *sp_info=musics->playing[i];
		if(sp_info!=NULL){

			if(sp_info->playing) {
				if(!sp_info->paused){ // if not is paused...
					BufferWaveMusic *wave_buffer = sp_info->wave_buffer;

					if((((wave_buffer->n_read_block+1)&MASK_MAX_FFM_BLOCKS )!= wave_buffer->n_write_block) && (wave_buffer->block_len[wave_buffer->n_read_block]==len)){
						SDL_MixAudio(stream
								, wave_buffer->block_ptr[wave_buffer->n_read_block]
								, len
								, SDL_MIX_MAXVOLUME*sp_info->volume);

						wave_buffer->block_len[wave_buffer->n_read_block]=0;
						wave_buffer->n_read_block = (wave_buffer->n_read_block+1)&MASK_MAX_FFM_BLOCKS;
					}
				}
			}else{
				musics->playing[i]=NULL;
			}
		}
	}

}

bool Music_IsValid(MUSIC id){
	if((id >= 0) && ((id) < MAX_MUSICS)){
		SOUND_TYPE type= g_mixer_vars->musics.loaded[id].type;

		return 		type == SOUND_TYPE_XMP
				|| 	type == SOUND_TYPE_FFMPEG
				|| 	type == SOUND_TYPE_WAV;

	}
	return false;
}

MUSIC Music_GetFreeSlotToPlay(void){
	MUSIC music = 0;
	for(music=0; music < MAX_PLAY_MUSICS; music++){
		if(g_mixer_vars->musics.playing[music]==NULL) {
			return music;
		}
	}
	return INVALID_SOUND_IDX;
}

MUSIC Music_GetFreeBlock(void){
	MUSIC music = 0;
	for(music=0; music < MAX_MUSICS; music++){
		if(g_mixer_vars->musics.loaded[music].type == SOUND_TYPE_NONE){
			return music;
		}
	}
	return INVALID_SOUND_IDX;
}

BufferWaveMusic * Music_NewWaveBuffer(void){
	BufferWaveMusic *wbm = (BufferWaveMusic *) malloc(sizeof(BufferWaveMusic));


	wbm->n_read_block=0;
	wbm->n_write_block=0;

	for(unsigned i = 0; i < MAX_FFM_BLOCKS; i++){
		wbm->block_len[i]=0;
		wbm->block_ptr[i] = (uint8_t *)malloc(g_mixer_vars->frame_size);
	}

	return wbm;
}

void Music_FreeWaveBuffer(BufferWaveMusic *wbm){
	if(wbm!=NULL){
		for(unsigned i = 0; i < MAX_FFM_BLOCKS; i++){
			free(wbm->block_ptr[i]);// = malloc(SPLAYER_FRAME_SIZE);
		}

		free(wbm);
	}
}


MUSIC Music_Load(const char *file){

	MUSIC sp_n_loaded_music;
	MixerSound *sp_info;

	if(strlen(file) < 4){
		Log_Error("Invalid filename");
		return INVALID_SOUND_IDX;
	}

	if(g_mixer_vars == NULL){
		Log_Error("Mixer not init");
		return INVALID_SOUND_IDX;
	}

	if((sp_n_loaded_music = Music_GetFreeBlock())==INVALID_SOUND_IDX){
		Log_Error( "Max sounds reached!");
		return INVALID_SOUND_IDX;
	}

	sp_info=&g_mixer_vars->musics.loaded[sp_n_loaded_music];



	if(strncmp("wav", (file+strlen(file))-3, 3) == 0){
		if(MusicWav_Load(sp_info,file)){
			return sp_n_loaded_music++;
		}
	}

	if(MusicXmp_IsFileSupported(file)){
		if(MusicXmp_Load(sp_info,file)){
			return sp_n_loaded_music++;
		}
	}
#ifdef __WITH_FFMPEG__
	else if(MusicFFmpeg_IsFileSupported(file)){
		if(MusicFFmpeg_Load(sp_info,file)){
			return sp_n_loaded_music++;
		}
	}
#endif

	Log_Error("file extension \"%s\" is unsupported",(file+strlen(file)-3));
	return INVALID_SOUND_IDX;
}

MUSIC Music_LoadFromMemory(unsigned char *ptr,size_t size){

	MUSIC sp_n_loaded_music;
	MixerSound *sp_info;

	if(g_mixer_vars == NULL){
		Log_Error("Mixer not init");
		return INVALID_SOUND_IDX;
	}

	if((sp_n_loaded_music = Music_GetFreeBlock())==INVALID_SOUND_IDX){
		Log_Error( "Max sounds reached!");
		return INVALID_SOUND_IDX;
	}

	sp_info=&g_mixer_vars->musics.loaded[sp_n_loaded_music];

	// to be implemented (get signatures from memory and try to load)...
	if((MusicWav_LoadFromMemory(sp_info,ptr,size)) != 0){
		return sp_n_loaded_music++;
	}
	// try to load xmp module

	if(MusicXmp_LoadFromMemory(sp_info,ptr,size)){ // let's try ffmpeg
		return sp_n_loaded_music++;
	}
#ifdef __WITH_FFMPEG__
	if(MusicFFmpeg_LoadFromMemory(sp_info,ptr,size)){
		return sp_n_loaded_music++;
	}
#endif

	return INVALID_SOUND_IDX;
}


bool Music_Seek(MUSIC id, uint_t t_seek){


	if(!Music_IsValid(id)){
		Log_Error("ID not valid!");
		return false;
	}

	MixerSound *sp_info = &g_mixer_vars->musics.loaded[id];

	switch(sp_info->type)
	{
	default:
		break;
#ifdef __WITH_FFMPEG__
	case SOUND_TYPE_FFMPEG:

		MusicFFmpeg_Seek(sp_info,t_seek);//(MusicFFmpeg*)g_mixer_vars->musics.loaded[id].data);
		break;
#endif
	case SOUND_TYPE_WAV:
		sp_info->request_seek = 0;
		break;
	case SOUND_TYPE_XMP:

		MusicXmp_Seek(sp_info,t_seek);
		break;
	}

	return false;
}

bool Music_Play(MUSIC id){
	MUSIC idx_play;

	if(!Music_IsValid(id)){
		Log_Error("ID not valid!");
		return false;
	}

	if(Music_IsPlaying(id)) {// reset?...
		Music_Stop(id);
	}

	idx_play = Music_GetFreeSlotToPlay();

	if(idx_play != INVALID_SOUND_IDX){

		MixerSound *sp_info = &g_mixer_vars->musics.loaded[id];
		//MusicXmp *s_xmp;

		switch(sp_info->type){
		case SOUND_TYPE_XMP:
			MusicXmp_Seek(sp_info,0);
			MusicXmp_Play(sp_info);
			break;
		case SOUND_TYPE_FFMPEG:
#ifdef __WITH_FFMPEG__
			MusicFFmpeg_Seek(sp_info,0);
#endif
			break;
		case SOUND_TYPE_WAV:
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
		Log_Error( "Max playing musics!");
		return false;
	}

	g_mixer_vars->musics.n_playing++;

	return true;
}

bool Music_SetVolume(MUSIC id, float vol){
	if(!Music_IsValid(id)){
		Log_Error("ID not valid!");
		return false;
	}
	MixerSound *sp_info = &g_mixer_vars->musics.loaded[id];
	sp_info->volume = MAX(MIN(vol,1),0);

	return true;
}

float Music_GetVolume(int id){
	if(!Music_IsValid(id)){
		Log_Error("ID not valid!");
		return 1;
	}

	MixerSound *sp_info = &g_mixer_vars->musics.loaded[id];
	return sp_info->volume;
}

uint32_t Music_GetDuration(MUSIC id){
	if(!Music_IsValid(id)){
		Log_Error("ID not valid!");
		return 0;
	}
	return g_mixer_vars->musics.loaded[id].duration;
}

bool Music_Pause(MUSIC id){

	if(!Music_IsValid(id)){
		Log_Error("ID not valid!");
		return false;
	}

	g_mixer_vars->musics.loaded[id].paused=true;
	return true;
}

bool Music_Resume(MUSIC id){

	if(!Music_IsValid(id)){
		Log_Error("ID not valid!");
		return false;
	}

	g_mixer_vars->musics.loaded[id].paused=false;

	return true;
}

bool Music_Stop(MUSIC id){

	if(!Music_IsValid(id)){
		Log_Error("ID not valid!");
		return false;
	}

	g_mixer_vars->musics.loaded[id].request_seek = 0;
	g_mixer_vars->musics.loaded[id].playing=false;
	while(Music_IsPlaying(id)){
		SDL_Delay(1);
	}

	memset(g_mixer_vars->musics.loaded[id].wave_buffer->block_len,0,sizeof(g_mixer_vars->musics.loaded[id].wave_buffer->block_len));
	g_mixer_vars->musics.loaded[id].wave_buffer->n_read_block = g_mixer_vars->musics.loaded[id].wave_buffer->n_write_block = 0;

	return true;
}

bool Music_IsPlaying(MUSIC id){
	int i=0;
	int m_found = 0;

	if(!Music_IsValid(id)){
		Log_Error("ID not valid!");
		return false;
	}

	for( i = 0; i < MAX_PLAY_MUSICS && !m_found; i++){
		if(g_mixer_vars->musics.playing[i]==&g_mixer_vars->musics.loaded[id])
			m_found=1;
	}
	return m_found;
}

bool Music_IsPaused(MUSIC id){
	if(!Music_IsValid(id)){
		Log_Error("ID not valid!");
		return false;
	}
	return g_mixer_vars->musics.loaded[id].paused==true;
}

bool Music_UpdateAudio(MixerSound *sp_info){
	BufferWaveMusic *wave_buffer = sp_info->wave_buffer;
	if(!sp_info->playing)
		return false;

	uint8_t n_current_block = wave_buffer->n_write_block&MASK_MAX_FFM_BLOCKS;
	if(wave_buffer->n_read_block == ((n_current_block+1) &MASK_MAX_FFM_BLOCKS))// && (sffmpeg->block_len[n_current_block] == SPLAYER_FRAME_SIZE)){
	{
		// full buffer wait until sb card has already readed all bytes ...
		return 0;
	}

	switch(sp_info->type){
	case SOUND_TYPE_XMP:
		MusicXmp_Update(sp_info);
		break;

	case SOUND_TYPE_WAV:

		MusicWav_Update(sp_info);

		// writ enew
		break;
#ifdef __WITH_FFMPEG__
	case SOUND_TYPE_FFMPEG:
		MusicFFmpeg_Update(sp_info);
		break;
#endif
	}

	SDL_Delay(1);

	return true;
}

void Music_Update(void){
	for(int i = 0; i < MAX_MUSICS; i++){
		if(g_mixer_vars->musics.loaded[i].type != SOUND_TYPE_NONE){
			if(g_mixer_vars->musics.loaded[i].playing){
				Music_UpdateAudio(&g_mixer_vars->musics.loaded[i]);
			}
		}
	}
}

uint32_t Music_GetCurrentTime(int id){

	if(!Music_IsValid(id)){
		Log_Error("ID not valid!");
		return 0;
	}
#ifdef __WITH_FFMPEG__
	MusicFFmpeg *sffmpeg;
#endif
	MixerSound *sp_info = &g_mixer_vars->musics.loaded[id];
	MusicXmp *s_xmp;
	struct xmp_frame_info frame_info;

	switch(sp_info->type)
	{
	default:
	case SOUND_TYPE_WAV:
		return Mixer_LengthSamplesToTime(sp_info->position_play);
#ifdef __WITH_FFMPEG__
	case SOUND_TYPE_FFMPEG:
		sffmpeg = (MusicFFmpeg *)sp_info->data;
		return (sffmpeg->currentPts);//readingPacket.pts*1000)/SPLAYER_FREQUENCY;
#endif
	case SOUND_TYPE_XMP:
		s_xmp = (MusicXmp *)sp_info->data;
		xmp_get_frame_info(s_xmp->ctx, &frame_info);
		return frame_info.time;
	}

	return 0;
}

void Music_Unload(MUSIC id){

	MixerSound *sp_info=NULL;

	if(!Music_IsValid(id)){
		Log_Error("ID not valid!");
		return;
	}

	Music_Stop(id);

	sp_info=&g_mixer_vars->musics.loaded[id];

	SOUND_TYPE type =sp_info->type;
	sp_info->type=0; // make it unusued...
	SDL_Delay(25);

	//MixerSound *sp_info = (MixerSound *)id;

	BufferWaveMusic *wave_buffer = sp_info->wave_buffer;

	if(wave_buffer!=NULL){
		Music_FreeWaveBuffer(wave_buffer);
	}

	switch(type){
	default:
		Log_Error("Unknown type? %i",type);
		return;
	case SOUND_TYPE_XMP:
		MusicXmp_Unload(sp_info);
		break;
	case SOUND_TYPE_WAV:
		MusicWav_Unload(sp_info);
		break;
#ifdef __WITH_FFMPEG__
	case SOUND_TYPE_FFMPEG:
		MusicFFmpeg_Unload(sp_info);
		break;
#endif
	}

	memset(sp_info,0, sizeof(MixerSound));
}

void Music_StopAll(void){
	//MIXER_stopThread();
	MUSIC m;

	for(m = 0; m < MAX_MUSICS; m++){
		if(g_mixer_vars->musics.loaded[m].type != SOUND_TYPE_NONE){
			Music_Stop(m);
		}
	}
}

void Music_UnloadAll(void){
	//MIXER_stopThread();
	MUSIC m;

	for(m = 0; m < MAX_MUSICS; m++){
		if(g_mixer_vars->musics.loaded[m].type != SOUND_TYPE_NONE){
			Music_Unload(m);
		}
	}
}
