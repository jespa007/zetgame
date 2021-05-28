#ifndef __MUSIC_H__
#define __MUSIC_H__


typedef int MUSIC;

MUSIC Music_Load(const char *file);
MUSIC Music_LoadFromMemory(unsigned char *ptr,size_t size);
bool Music_Play(MUSIC id);
bool Music_Pause(MUSIC id);
bool Music_Resume(MUSIC id);
bool Music_IsPlaying(MUSIC id);
bool Music_IsPaused(MUSIC id);
bool Music_Stop(MUSIC id);

/**
 * Set volume
 * vol: 0 - 1
 */
bool Music_SetVolume(MUSIC id, float vol);

/**
 * Get current time in ms
 */

uint_t	Music_GetCurrentTime(MUSIC id);
float	Music_GetVolume(MUSIC id);
uint_t 	Music_GetDuration(MUSIC id);

/**
 * Request time to seek in ms.
 */
 bool Music_Seek(MUSIC id, uint_t t_seek);


/**
* unload music
*/
void Mixer_Unload(MUSIC id);

#endif
