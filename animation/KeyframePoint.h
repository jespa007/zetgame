#ifndef __KEYFRAME_POINT_H__
#define __KEYFRAME_POINT_H__

#define I1D_IDX_TIME 		0 // independent variable (t)
#define I1D_IDX_VALUE 		1 // independent variable (t)
#define I1D_POINT_SIZE		2 // T + value

typedef struct KeyframePoint KeyframePoint;

struct KeyframePoint{

	// By reference
	float 		point[I1D_POINT_SIZE],
				control_point_1[I1D_POINT_SIZE],
				control_point_2[I1D_POINT_SIZE],

				inc_fx[I1D_POINT_SIZE];

	//------------- FOR COS INTERPOLATOR ---------------------------
	float inc_one_over_diff_interval;
	//--------------------------------------------------------------
};

KeyframePoint * KeyframePoint_New(void);
void 			KeyframePoint_Delete(KeyframePoint * _this);


#endif
