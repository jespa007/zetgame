#ifndef __SAMPLE_H__
#define __SAMPLE_H__

typedef int SAMPLE;

SAMPLE	Sample_Load(const char *wav_file);
SAMPLE 	Sample_LoadFromMemory(unsigned char *wav_ptr,uint32_t size);
bool 	Sample_Play(SAMPLE id);
bool 	Sample_Stop(SAMPLE id);
bool 	Sample_SetVolume(SAMPLE id, float vol);
uint32_t 	Sample_GetDuration(SAMPLE id);

void 	Sample_Unload(SAMPLE id);
/**
 * Get volume song by its id
 */
 float 	Sample_GetVolume(int id);


#endif
