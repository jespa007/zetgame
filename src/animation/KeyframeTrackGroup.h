#ifndef __KEYFRAME_TRACK_GROUP_H__
#define __KEYFRAME_TRACK_GROUP_H__

// A way to describe packed units for each type (example translation,rotation,etc ) in order to decompose in 1d
/*#define KEYFRAME_TRACK_GROUP_COMPONENT_X (0x1 << 0)
#define KEYFRAME_TRACK_GROUP_COMPONENT_Y (0x1 << 1)
#define KEYFRAME_TRACK_GROUP_COMPONENT_Z (0x1 << 2)
#define KEYFRAME_TRACK_GROUP_COMPONENT_W (0x1 << 3)
*/



typedef struct KeyframeTrackGroup KeyframeTrackGroup;

struct KeyframeTrackGroup{
	float * keyframe_points;
	size_t  keyframe_points_count;
};

//---------------------------------------------------------------------------------------------------------------------
// STATIC
bool KeyframeTrackGroup_DecomposeXYZW(
		  const float * v_input
		, size_t v_input_len
		, KeyframeTrackGroup * vx
		, KeyframeTrackGroup * vy
		, KeyframeTrackGroup * vz
		, KeyframeTrackGroup * vw
	);

void KeyframeTrackGroup_FreeXYZW(KeyframeTrackGroup ** vx
		, KeyframeTrackGroup ** vy
		, KeyframeTrackGroup ** vz
		, KeyframeTrackGroup ** vw
);

//---------------------------------------------------------------------------------------------------------------------
// PUBLIC
KeyframeTrackGroup *KeyframeTrackGroup_New(void);
void KeyframeTrackGroup_Delete(KeyframeTrackGroup *_this);


#endif
