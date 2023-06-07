#include "zetgame.h"


int main(int argc, char *argv[]){

	ZG_SetupParams default_setup={
			.graphics_api=ZG_GRAPHICS_API_GL
			,.width=ZG_DEFAULT_SCREEN_WIDTH
			,.height=ZG_DEFAULT_SCREEN_HEIGHT
			,.wcaption_title="test_mixer"
			,.wposx=SDL_WINDOWPOS_CENTERED
			,.wposy=80
			,.graphic_properties=0
	};


	uint_t	 request_forward_seek=0
			,request_backward_seek=0;

	ZG_Init(&default_setup);

	ZG_SAMPLE 	id_effect = ZG_INVALID_SOUND_IDX;
	ZG_MUSIC 	id_wav = ZG_INVALID_SOUND_IDX
			 ,id_ogg = ZG_INVALID_SOUND_IDX
			 ,id_xm = ZG_INVALID_SOUND_IDX;


	if(argc == 2 && !strcmp(argv[1],"from_mem")){

		ZG_Log_InfoF("Load from memory");

		ZG_BufferByte *bb_effect = ZG_File_Read("../../../test/data/samples/sample.wav");
		ZG_BufferByte *bb_wav = ZG_File_Read("../../../test/data/musics/music.wav");
		ZG_BufferByte *bb_ogg = ZG_File_Read("../../../test/data/musics/music.ogg");
		ZG_BufferByte *bb_xm = ZG_File_Read("../../../test/data/musics/music.xm");

		id_effect = ZG_Sample_LoadFromMemory(bb_effect->ptr, bb_effect->len);
		id_wav = ZG_Music_LoadFromMemory(bb_wav->ptr, bb_wav->len);
		id_ogg = ZG_Music_LoadFromMemory(bb_ogg->ptr, bb_ogg->len);
		id_xm = ZG_Music_LoadFromMemory(bb_xm->ptr, bb_xm->len);

		if(bb_effect!=NULL)ZG_BufferByte_Delete(bb_effect);
		if(bb_wav!=NULL)ZG_BufferByte_Delete(bb_wav);
		if(bb_ogg!=NULL)ZG_BufferByte_Delete(bb_ogg);
		if(bb_xm!=NULL)ZG_BufferByte_Delete(bb_xm);

	}else{
		id_effect = ZG_Sample_Load("../../../test/data/samples/sample.wav");
		id_wav = ZG_Music_Load("../../../test/data/musics/music.wav");
		id_ogg = ZG_Music_Load("../../../test/data/musics/music.ogg");
		id_xm = ZG_Music_Load("../../../test/data/musics/music.xm");

	}

	ZG_Log_Info("duration 0: %2i:%02i s",ZG_Sample_GetDuration(id_effect)/60000		,(int)(((float)(ZG_Sample_GetDuration(id_effect))/60000.0f)*60.0f));
	ZG_Log_Info("duration 1: %2i:%02i s",ZG_Music_GetDuration(id_wav)/60000		,(int)(((float)(ZG_Music_GetDuration(id_wav))/60000.0f)*60.0f));
	ZG_Log_Info("duration 2: %2i:%02i s",ZG_Music_GetDuration(id_xm)/60000		,(int)(((float)(ZG_Music_GetDuration(id_xm))/60000.0f)*60.0f));
	ZG_Log_Info("duration 3: %2i:%02i s",ZG_Music_GetDuration(id_ogg)/60000	,(int)(((float)(ZG_Music_GetDuration(id_ogg))/60000.0f)*60.0f));

	do{
		ZG_Graphics_BeginRender();

		request_forward_seek=0;
		request_backward_seek=0;

		if(K_0){
			ZG_Log_InfoF("play effect");
			ZG_Sample_Play(id_effect);

		}
		if(K_1){

			if(ZG_Music_IsPlaying(id_wav)&& !ZG_Music_IsPaused(id_wav)){
				ZG_Log_InfoF("Pause wav");
				ZG_Music_Pause(id_wav);
			}else{
				if(ZG_Music_IsPaused(id_wav)){
					ZG_Music_Resume(id_wav);
					ZG_Log_InfoF("Resume wav");
				}
				else{
					ZG_Log_InfoF("Play wav");
					ZG_Music_Play(id_wav);
				}
			}
		}
		if(K_2){
			if(ZG_Music_IsPlaying(id_xm)&& !ZG_Music_IsPaused(id_xm)){
				ZG_Log_InfoF("Pause xm");
				ZG_Music_Pause(id_xm);
			}else{
				if(ZG_Music_IsPaused(id_xm)){
					ZG_Music_Resume(id_xm);
					ZG_Log_InfoF("Resume xm");
				}
				else{
					ZG_Log_InfoF("Play xm");
					ZG_Music_Play(id_xm);
				}
			}
		}
		if(K_3){
			if(ZG_Music_IsPlaying(id_ogg)&& !ZG_Music_IsPaused(id_ogg)){
				ZG_Log_InfoF("Pause ogg");
				ZG_Music_Stop(id_ogg);
			}else{
				if(ZG_Music_IsPaused(id_ogg)){
					ZG_Music_Resume(id_ogg);
					ZG_Log_InfoF("Resume ogg");
				}
				else{
					ZG_Log_InfoF("Play ogg");
					ZG_Music_Play(id_ogg);
				}
			}
		}


		if(K_RIGHT)	request_forward_seek=1;
		if(K_LEFT)	request_backward_seek=1;


		if(request_forward_seek){
			ZG_Music_Seek(id_wav,ZG_Music_GetCurrentTime(id_wav)+1000);
			ZG_Music_Seek(id_ogg,ZG_Music_GetCurrentTime(id_ogg)+1000);
			ZG_Music_Seek(id_xm,ZG_Music_GetCurrentTime(id_xm)+1000);
		}

		if(request_backward_seek){
			ZG_Music_Seek(id_wav,ZG_Music_GetCurrentTime(id_wav)-1000);
			ZG_Music_Seek(id_ogg,ZG_Music_GetCurrentTime(id_ogg)-1000);
			ZG_Music_Seek(id_xm,ZG_Music_GetCurrentTime(id_xm)-1000);
		}

		ZG_Mixer_Update();

		ZG_Graphics_EndRender();
		ZG_Input_Update();
	}while(!K_ESC);

	ZG_DeInit();


    return 0;

}
