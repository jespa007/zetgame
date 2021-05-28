/*
 * splayer_base.h
 *
 *  Created on: Sep 22, 2014
 *      Author: jespada
 */

#ifndef __MIXER_H__
#define __MIXER_H__


#define INVALID_SOUND_IDX -1

/**
 *  Get error on load/play sound
 */

typedef enum{
	AUDIO_FORMAT_INT_16=0,
	AUDIO_FORMAT_FLOAT_32
}AudioFormat;


bool Mixer_Init(AudioFormat format);//int freq, int keyframe_tracks);

/**
* unload music
*/
void Mixer_Update(void);


/**
 * stop and unload all sounds.
 */
 void Mixer_UnloadAll(void);


 void Mixer_StopAll(void);

/**
 * Deinit splayer
 */
 void Mixer_DeInit(void);


#endif
