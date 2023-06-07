#ifndef __ZG_KEYFRAME_POINT_H__
#define __ZG_KEYFRAME_POINT_H__

#define ZG_I1D_IDX_TIME 		0 // independent variable (t)
#define ZG_I1D_IDX_VALUE 		1 // independent variable (t)
#define ZG_I1D_POINT_SIZE		2 // T + value

typedef struct ZG_KeyframePoint ZG_KeyframePoint;

struct ZG_KeyframePoint{

	// By reference
	float 		point[ZG_I1D_POINT_SIZE],
				control_point_1[ZG_I1D_POINT_SIZE],
				control_point_2[ZG_I1D_POINT_SIZE],

				inc_fx[ZG_I1D_POINT_SIZE];

	//------------- FOR COS INTERPOLATOR ---------------------------
	float inc_one_over_diff_interval;
	//--------------------------------------------------------------
};

ZG_KeyframePoint * ZG_KeyframePoint_New(void);
void 			ZG_KeyframePoint_Delete(ZG_KeyframePoint * _this);


#endif
