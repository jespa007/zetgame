#include "../ZetGame.h"


int main(int argc, char *argv[]){

	ZetGameSetupParams default_setup={
			.graphics_api=GRAPHICS_API_GL
			,.width=ZG_DEFAULT_SCREEN_WIDTH
			,.height=ZG_DEFAULT_SCREEN_HEIGHT
			,.wcaption_title="test_mixer"
			,.wposx=SDL_WINDOWPOS_CENTERED
			,.wposy=80
			,.graphic_properties=0
	};


	uint_t	 request_forward_seek=0
			,request_backward_seek=0;

	ZetGame_Init(&default_setup);

	SAMPLE 	id_effect = INVALID_SOUND_IDX;
	MUSIC 	id_wav = INVALID_SOUND_IDX
			 ,id_ogg = INVALID_SOUND_IDX
			 ,id_xm = INVALID_SOUND_IDX;


	if(argc == 2 && !strcmp(argv[1],"from_mem")){

		Log_Info("Load from memory");

		BufferByte *bb_effect = File_Read("data/samples/sample.wav");
		BufferByte *bb_wav = File_Read("data/musics/music.wav");
		BufferByte *bb_ogg = File_Read("data/musics/music.ogg");
		BufferByte *bb_xm = File_Read("data/musics/music.xm");

		id_effect = Sample_LoadFromMemory(bb_effect->ptr, bb_effect->len);
		id_wav = Music_LoadFromMemory(bb_wav->ptr, bb_wav->len);
		id_ogg = Music_LoadFromMemory(bb_ogg->ptr, bb_ogg->len);
		id_xm = Music_LoadFromMemory(bb_xm->ptr, bb_xm->len);

		if(bb_effect!=NULL)BufferByte_Delete(bb_effect);
		if(bb_wav!=NULL)BufferByte_Delete(bb_wav);
		if(bb_ogg!=NULL)BufferByte_Delete(bb_ogg);
		if(bb_xm!=NULL)BufferByte_Delete(bb_xm);

	}else{
		id_effect = Sample_Load("data/samples/sample.wav");
		id_wav = Music_Load("data/musics/music.wav");
		id_ogg = Music_Load("data/musics/music.ogg");
		id_xm = Music_Load("data/musics/music.xm");
	}

	Log_Info("duration 0: %2i:%02i s",Sample_GetDuration(id_effect)/60000		,(int)(((float)(Sample_GetDuration(id_effect))/60000.0f)*60.0f));
	Log_Info("duration 1: %2i:%02i s",Music_GetDuration(id_wav)/60000		,(int)(((float)(Music_GetDuration(id_wav))/60000.0f)*60.0f));
	Log_Info("duration 2: %2i:%02i s",Music_GetDuration(id_xm)/60000		,(int)(((float)(Music_GetDuration(id_xm))/60000.0f)*60.0f));
	Log_Info("duration 3: %2i:%02i s",Music_GetDuration(id_ogg)/60000	,(int)(((float)(Music_GetDuration(id_ogg))/60000.0f)*60.0f));

	do{
		Graphics_BeginRender();

		request_forward_seek=0;
		request_backward_seek=0;

		if(K_0){
			Log_Info("play effect");
			Sample_Play(id_effect);

		}
		if(K_1){

			if(Music_IsPlaying(id_wav)&& !Music_IsPaused(id_wav)){
				Log_Info("Pause wav");
				Music_Pause(id_wav);
			}else{
				if(Music_IsPaused(id_wav)){
					Music_Resume(id_wav);
					Log_Info("Resume wav");
				}
				else{
					Log_Info("Play wav");
					Music_Play(id_wav);
				}
			}
		}
		if(K_2){
			if(Music_IsPlaying(id_xm)&& !Music_IsPaused(id_xm)){
				Log_Info("Pause xm");
				Music_Pause(id_xm);
			}else{
				if(Music_IsPaused(id_xm)){
					Music_Resume(id_xm);
					Log_Info("Resume xm");
				}
				else{
					Log_Info("Play xm");
					Music_Play(id_xm);
				}
			}
		}
		if(K_3){
			if(Music_IsPlaying(id_ogg)&& !Music_IsPaused(id_ogg)){
				Log_Info("Pause ogg");
				Music_Stop(id_ogg);
			}else{
				if(Music_IsPaused(id_ogg)){
					Music_Resume(id_ogg);
					Log_Info("Resume ogg");
				}
				else{
					Log_Info("Play ogg");
					Music_Play(id_ogg);
				}
			}
		}


		if(K_RIGHT)	request_forward_seek=1;
		if(K_LEFT)	request_backward_seek=1;


		if(request_forward_seek){
			Music_Seek(id_wav,Music_GetCurrentTime(id_wav)+1000);
			Music_Seek(id_ogg,Music_GetCurrentTime(id_ogg)+1000);
			Music_Seek(id_xm,Music_GetCurrentTime(id_xm)+1000);
		}

		if(request_backward_seek){
			Music_Seek(id_wav,Music_GetCurrentTime(id_wav)-1000);
			Music_Seek(id_ogg,Music_GetCurrentTime(id_ogg)-1000);
			Music_Seek(id_xm,Music_GetCurrentTime(id_xm)-1000);
		}

		Mixer_Update();

		Graphics_EndRender();
		Input_Update();
	}while(!K_ESC);

	ZetGame_DeInit();


#if defined(__MEMMANAGER__)
  MEMMGR_print_status();
#endif

    return 0;

};
