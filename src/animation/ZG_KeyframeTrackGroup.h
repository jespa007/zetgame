#ifndef __ZG_KEYFRAME_TRACK_GROUP_H__
#define __ZG_KEYFRAME_TRACK_GROUP_H__

// A way to describe packed units for each type (example translation,rotation,etc ) in order to decompose in 1d
/*#define KEYFRAME_TRACK_GROUP_COMPONENT_X (0x1 << 0)
#define KEYFRAME_TRACK_GROUP_COMPONENT_Y (0x1 << 1)
#define KEYFRAME_TRACK_GROUP_COMPONENT_Z (0x1 << 2)
#define KEYFRAME_TRACK_GROUP_COMPONENT_W (0x1 << 3)
*/



typedef struct ZG_KeyframeTrackGroup ZG_KeyframeTrackGroup;

struct ZG_KeyframeTrackGroup{
	float * keyframe_points;
	size_t  keyframe_points_count;
};

//---------------------------------------------------------------------------------------------------------------------
// STATIC
bool ZG_KeyframeTrackGroup_DecomposeXYZW(
		  const float * v_input
		, size_t v_input_len
		, ZG_KeyframeTrackGroup * vx
		, ZG_KeyframeTrackGroup * vy
		, ZG_KeyframeTrackGroup * vz
		, ZG_KeyframeTrackGroup * vw
	);

void ZG_KeyframeTrackGroup_FreeXYZW(ZG_KeyframeTrackGroup ** vx
		, ZG_KeyframeTrackGroup ** vy
		, ZG_KeyframeTrackGroup ** vz
		, ZG_KeyframeTrackGroup ** vw
);

//---------------------------------------------------------------------------------------------------------------------
// PUBLIC
ZG_KeyframeTrackGroup *ZG_KeyframeTrackGroup_New(void);
void ZG_KeyframeTrackGroup_Delete(ZG_KeyframeTrackGroup *_this);


#endif
