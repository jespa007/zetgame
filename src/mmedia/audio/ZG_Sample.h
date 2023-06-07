#ifndef __ZG_SAMPLE_H__
#define __ZG_SAMPLE_H__

typedef int ZG_SAMPLE;

ZG_SAMPLE	ZG_Sample_Load(const char *wav_file);
ZG_SAMPLE 	ZG_Sample_LoadFromMemory(unsigned char *wav_ptr,uint32_t size);
bool 	    ZG_Sample_Play(ZG_SAMPLE id);
bool 	    ZG_Sample_Stop(ZG_SAMPLE id);
bool 	    ZG_Sample_SetVolume(ZG_SAMPLE id, float vol);
uint32_t 	ZG_Sample_GetDuration(ZG_SAMPLE id);

void 	    ZG_Sample_Unload(ZG_SAMPLE id);
/**
 * Get volume song by its id
 */
 float 	    ZG_Sample_GetVolume(int id);


#endif
