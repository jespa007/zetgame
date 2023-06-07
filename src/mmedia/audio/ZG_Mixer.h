/*
 * splayer_base.h
 *
 *  Created on: Sep 22, 2014
 *      Author: jespada
 */

#ifndef __ZG_MIXER_H__
#define __ZG_MIXER_H__


#define ZG_INVALID_SOUND_IDX -1

/**
 *  Get error on load/play sound
 */

typedef enum{
	ZG_AUDIO_FORMAT_INT_16=0,
	ZG_AUDIO_FORMAT_FLOAT_32
}ZG_AudioFormat;


bool ZG_Mixer_Init(void);//int freq, int keyframe_tracks);

/**
* unload music
*/
void ZG_Mixer_Update(void);


/**
 * stop and unload all sounds.
 */
 void ZG_Mixer_UnloadAll(void);


 void ZG_Mixer_StopAll(void);

/**
 * Deinit splayer
 */
 void ZG_Mixer_DeInit(void);


#endif
