#ifndef __ZG_MUSIC_H__
#define __ZG_MUSIC_H__


typedef int ZG_MUSIC;

ZG_MUSIC ZG_Music_Load(const char *file);
ZG_MUSIC ZG_Music_LoadFromMemory(unsigned char *ptr,size_t size);
bool ZG_Music_Play(ZG_MUSIC id);
bool ZG_Music_Pause(ZG_MUSIC id);
bool ZG_Music_Resume(ZG_MUSIC id);
bool ZG_Music_IsPlaying(ZG_MUSIC id);
bool ZG_Music_IsPaused(ZG_MUSIC id);
bool ZG_Music_Stop(ZG_MUSIC id);

/**
 * Set volume
 * vol: 0 - 1
 */
bool ZG_Music_SetVolume(ZG_MUSIC id, float vol);

/**
 * Get current time in ms
 */

uint_t	ZG_Music_GetCurrentTime(ZG_MUSIC id);
float	ZG_Music_GetVolume(ZG_MUSIC id);
uint_t 	ZG_Music_GetDuration(ZG_MUSIC id);

/**
 * Request time to seek in ms.
 */
 bool ZG_Music_Seek(ZG_MUSIC id, uint_t t_seek);


/**
* unload music
*/
void ZG_Mixer_Unload(ZG_MUSIC id);

#endif
