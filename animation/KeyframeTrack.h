#ifndef __KEYFRAME_TRACK_H__
#define __KEYFRAME_TRACK_H__



//--------------------- PROPERTIES ------
#define	KEYFRAME_TRACK_PROPERTY_TIME_POW_2 	(0x1 << 0)



typedef struct KeyframeTrack KeyframeTrack;

struct KeyframeTrack{

	List 	* 	keyframe_points;

	float		minx_interval,// = 0.0f,
				maxx_interval,// = 1.0f,
				dxdt_pow2,
				rangex_interval;//   = 1.0f;

	uint32_t	rangex_interval_pow2;
	bool 		configured_interpolator;
	Ease 		ease;

	uint32_t 	properties;

};

//--------------------------------------------------------------------------------------------------------
// STATIC
void KeyframeTrack_CalculeFx(KeyframePoint * result
	, const float  ini_point[I1D_POINT_SIZE]
	, const float  ini_control_point_1[I1D_POINT_SIZE]
	, const float  ini_control_point_2[I1D_POINT_SIZE]
	, float iv_ini
	, const float end_point[I1D_POINT_SIZE]
	, const float end_control_point_1[I1D_POINT_SIZE]
	, const float end_control_point_2[I1D_POINT_SIZE]
	, float iv_end);

//--------------------------------------------------------------------------------------------------------
// STATIC
Ease 	KeyframeTrack_ParseInterpolatorType(const char * _ease);

//--------------------------------------------------------------------------------------------------------
// PUBLIC
KeyframeTrack *KeyframeTrack_New(void);

void			KeyframeTrack_SetEase(KeyframeTrack * _this, Ease _ease);
void 			KeyframeTrack_Clear(KeyframeTrack * _this);
void			KeyframeTrack_SetEase(KeyframeTrack * _this, Ease _ease);
void 			KeyframeTrack_SetKeyframes(KeyframeTrack * _this,float * _point, size_t _point_count);
void 			KeyframeTrack_AddKeyframesFloat(KeyframeTrack * _this,const float * _keyframe_points, size_t _keyframe_points_count);
void 			KeyframeTrack_AddKeyframesBezier(KeyframeTrack * _this,List * _point);
bool 			KeyframeTrack_Interpolate(KeyframeTrack * _this, float _t, float * _result);

void 			KeyframeTrack_Delete(KeyframeTrack *_this);


#endif
