#ifndef __ZG_KEYFRAME_TRACK_H__
#define __ZG_KEYFRAME_TRACK_H__



//--------------------- PROPERTIES ------
#define	ZG_KEYFRAME_TRACK_PROPERTY_TIME_POW_2 	(0x1 << 0)



typedef struct ZG_KeyframeTrack ZG_KeyframeTrack;

struct ZG_KeyframeTrack{

	ZG_List 	* 	keyframe_points;

	float		minx_interval,// = 0.0f,
				maxx_interval,// = 1.0f,
				dxdt_pow2,
				rangex_interval;//   = 1.0f;

	uint32_t	rangex_interval_pow2;
	bool 		configured_interpolator;
	ZG_Ease 		ease;

	uint32_t 	properties;

};

//--------------------------------------------------------------------------------------------------------
// STATIC
void ZG_KeyframeTrack_CalculeFx(ZG_KeyframePoint * result
	, const float  ini_point[ZG_I1D_POINT_SIZE]
	, const float  ini_control_point_1[ZG_I1D_POINT_SIZE]
	, const float  ini_control_point_2[ZG_I1D_POINT_SIZE]
	, float iv_ini
	, const float end_point[ZG_I1D_POINT_SIZE]
	, const float end_control_point_1[ZG_I1D_POINT_SIZE]
	, const float end_control_point_2[ZG_I1D_POINT_SIZE]
	, float iv_end);

//--------------------------------------------------------------------------------------------------------
// STATIC
ZG_Ease 	ZG_KeyframeTrack_ParseInterpolatorType(const char * _ease);

//--------------------------------------------------------------------------------------------------------
// PUBLIC
ZG_KeyframeTrack *ZG_KeyframeTrack_New(void);

void			ZG_KeyframeTrack_SetEase(ZG_KeyframeTrack * _this, ZG_Ease _ease);
void 			ZG_KeyframeTrack_Clear(ZG_KeyframeTrack * _this);
void			ZG_KeyframeTrack_SetEase(ZG_KeyframeTrack * _this, ZG_Ease _ease);
void 			ZG_KeyframeTrack_SetKeyframes(ZG_KeyframeTrack * _this,float * _point, size_t _point_count);
void 			ZG_KeyframeTrack_AddKeyframesFloat(ZG_KeyframeTrack * _this,const float * _points, size_t _points_count);
void 			ZG_KeyframeTrack_AddKeyframesBezier(ZG_KeyframeTrack * _this,ZG_List * _point);
bool 			ZG_KeyframeTrack_Interpolate(ZG_KeyframeTrack * _this, float _t, float * _result);

void 			ZG_KeyframeTrack_Delete(ZG_KeyframeTrack *_this);


#endif
