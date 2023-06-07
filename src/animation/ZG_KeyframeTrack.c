#include "_zg_animation_.h"

#define ZG_CAST_BEZIER_POINT(p) ((ZG_KeyframePoint *)(p))


//---------------------------------------------------------------------------------------------------
// PUBLIC
ZG_KeyframeTrack * ZG_KeyframeTrack_New(void)
{
	ZG_KeyframeTrack *int1d = ZG_NEW(ZG_KeyframeTrack);
	int1d->ease = ZG_EASE_LINEAR;
	int1d->minx_interval = FLT_MAX,
	int1d->maxx_interval = FLT_MIN,
	int1d->rangex_interval   = FLT_MIN;
	int1d->rangex_interval_pow2 = 0;
	int1d->keyframe_points=ZG_List_New();
	int1d->dxdt_pow2 =  1;

	return int1d;

}


//---------------------------------------------------------------------------------------------------
// STATIC

Ease 	ZG_KeyframeTrack_ParseInterpolatorType(const char * interpolator_type_str){
	Ease ease=ZG_EASE_LINEAR;

	if(ZG_STRCMP(interpolator_type_str,==,"LINEAR")) {
		ease=ZG_EASE_LINEAR;
	}else if(ZG_STRCMP(interpolator_type_str,==,"IN_OUT_SINE")) {
		ease=ZG_EASE_IN_OUT_SINE;
	}else {
		ZG_Log_ErrorF("Invalid EASE type \"%s\": Valid ones are \"LINEAR\" or \"IN_OUT_SINE\"");
	}


	return ease;
}


void ZG_KeyframeTrack_CalculeFx(
	  ZG_KeyframePoint * result
	, const float  ini_point[ZG_I1D_POINT_SIZE]
	, const  float   ini_control_point_1[ZG_I1D_POINT_SIZE]
	, const  float  ini_control_point_2[ZG_I1D_POINT_SIZE]
	, float iv_ini
	, const float  end_point[ZG_I1D_POINT_SIZE]
	, const  float  end_control_point_1[ZG_I1D_POINT_SIZE]
    , const  float  end_control_point_2[ZG_I1D_POINT_SIZE]
    , float iv_end)
{

	ZG_UNUSUED_PARAM(ini_control_point_1);
	ZG_UNUSUED_PARAM(ini_control_point_2);
	ZG_UNUSUED_PARAM(end_control_point_1);
	ZG_UNUSUED_PARAM(end_control_point_2);

	//
	// PRE: Assumes that result, ini_point && end_point is equal to n_dimension

	float diff = (iv_end-iv_ini);
	float inc_one_over_diff_interval = 1.0f/(diff);

	result->inc_fx[ZG_I1D_IDX_VALUE] = (end_point[ZG_I1D_IDX_VALUE]-ini_point[ZG_I1D_IDX_VALUE])/diff;
	result->inc_one_over_diff_interval = inc_one_over_diff_interval;
}
//---------------------------------------------------------------------------------------------------
// PRIVATE
void ZG_KeyframeTrack_SetEase(ZG_KeyframeTrack * _this, Ease _ease){
	_this->ease=_ease;
}
//---
void ZG_KeyframeTrack_Clear(ZG_KeyframeTrack *_this)
{
	for(unsigned i = 0; i < _this->keyframe_points->count; i++)
	{
		ZG_KeyframePoint_Delete(_this->keyframe_points->items[i]);
	}

	ZG_List_Clear(_this->keyframe_points);
}
//---
void ZG_KeyframeTrack_AddKeyframesFloat(ZG_KeyframeTrack *_this,const float * _points, size_t _points_count)
{
	// PRE:
	// point[x1,x2,...xn-1]: Dependen var (x,y,z, etc).
	// point[xn]: Independent var (time).
	if(_points_count==0){ // no keyframe_points...
		return;
	}

	if(_points_count % ZG_I1D_POINT_SIZE != 0)
	{
		ZG_Log_ErrorF("Error! Data length is not equal to dimension+1");
		return;
	}

	int number_keyframes = _points_count/ZG_I1D_POINT_SIZE;
	ZG_KeyframePoint *new_data=NULL, *previus_data=NULL;

	// Get the independent point...
	float x_ini, x_actual, x_old;
	float x_end;


	int i;
	int length_offset = ZG_I1D_POINT_SIZE;
	x_actual = _points[ZG_I1D_IDX_TIME];


	x_old = x_actual-1.0f;
	//x_actual = point[N_DIMENSION];
	int actual_offset = 0;


	for(i = 0; i < number_keyframes; i++)
	{
		x_actual = _points[actual_offset + ZG_I1D_IDX_TIME];

		if(x_actual < x_old)
		{
			ZG_Log_Error("Malformed list point at index %i (independent variable is not ascendent) (last independent x: %f,current independent x: %f)",i,x_old, x_actual);

			return;
		}

		new_data = ZG_KeyframePoint_New();

		// copy first point...
		new_data->point[ZG_I1D_IDX_TIME] 		  = _points[actual_offset + ZG_I1D_IDX_TIME];
		new_data->point[ZG_I1D_IDX_VALUE] 		  = _points[actual_offset + ZG_I1D_IDX_VALUE];

		// Interpolates with prevous point...
		if(i > 0)
		{
			previus_data = _this->keyframe_points->items[i-1];
			ZG_KeyframeTrack_CalculeFx(previus_data,
					previus_data->point,
					previus_data->control_point_1,
					previus_data->control_point_2,
					previus_data->point[ZG_I1D_IDX_TIME],
					new_data->point,
					new_data->control_point_1,
					new_data->control_point_2,
					new_data->point[ZG_I1D_IDX_TIME]
			);

			_this->keyframe_points->items[i-1] = previus_data;
		}

		ZG_List_Insert(_this->keyframe_points,i, new_data);
		x_old = x_actual;
		actual_offset += length_offset;
	}

	x_end = _points[(number_keyframes-1)*length_offset+ ZG_I1D_IDX_TIME];
	x_ini = _points[(0)*length_offset +              ZG_I1D_IDX_TIME];

	_this->minx_interval = MIN(_this->minx_interval, x_ini);
	_this->maxx_interval = MAX(_this->maxx_interval, x_end);

	_this->rangex_interval = _this->maxx_interval-_this->minx_interval;
}
//---
void ZG_KeyframeTrack_SetKeyframes(ZG_KeyframeTrack *_this,float * _points, size_t _points_count)
{
	ZG_KeyframeTrack_Clear(_this);
	ZG_KeyframeTrack_AddKeyframesFloat(_this,_points,_points_count);

	// optimization independent var as pow2
	_this->rangex_interval_pow2=ZG_Bit_NextPow2(_this->rangex_interval);
	_this->dxdt_pow2=(float)_this->rangex_interval_pow2/_this->rangex_interval;
	_this->properties|=ZG_KEYFRAME_TRACK_PROPERTY_TIME_POW_2;

	// TODO: set all independent keyframe_points 1/maxx_interval_mask;
}
//---
void ZG_KeyframeTrack_AddKeyframesBezier(ZG_KeyframeTrack * _this,ZG_List * _keyframe_points)
{
	// PRE:
	// point[x1,x2,...xn-1]: Dependen var (x,y,z, etc).
	// point[xn]: Independent var (time).
	int x_actual=-1;
	int x_old=-1;


	ZG_KeyframePoint *new_data,*first_point, *last_point;

	for(unsigned i = 0; i < _keyframe_points->count; i++)
	{
		ZG_KeyframePoint * bp_actual=ZG_CAST_BEZIER_POINT(_keyframe_points->items[i]);

		x_actual = bp_actual->point[ZG_I1D_IDX_TIME];//actual_offset + 2*ZG_I1D_IDX_TIME];

		if(x_actual < x_old)
		{
			ZG_Log_Error("Malformed list point at index %i (independent variable is not ascendent) (p: %i,a: %i)",i,x_old, x_actual);

			return;
		}

		new_data = ZG_KeyframePoint_New();

		for(unsigned j=0; j< ZG_I1D_POINT_SIZE; j++){
			new_data->control_point_1[j] 	= bp_actual->control_point_1[j];
			new_data->point[j]				= bp_actual->point[j];
			new_data->control_point_2[j] 	= bp_actual->control_point_2[j];
		}

		// Interpolates with prevous point...
		if(i > 0)
		{
			ZG_KeyframePoint * previus_data = _this->keyframe_points->items[i-1];
			ZG_KeyframeTrack_CalculeFx(previus_data,
					previus_data->point,
					previus_data->control_point_1,
					previus_data->control_point_2,
					previus_data->point[ZG_I1D_IDX_TIME],
					new_data->point,
					new_data->control_point_1,
					new_data->control_point_2,
					new_data->point[ZG_I1D_IDX_TIME]
			);
		}

		ZG_List_Insert(_this->keyframe_points,i, new_data);

		x_old = x_actual;
	}

	first_point=_keyframe_points->items[0];
	last_point=_keyframe_points->items[_keyframe_points->count-1];

	_this->minx_interval = MIN(_this->minx_interval, last_point->point[ZG_I1D_IDX_TIME]);
	_this->maxx_interval = MAX(_this->maxx_interval, first_point->point[ZG_I1D_IDX_TIME]);

	_this->rangex_interval = _this->maxx_interval-_this->minx_interval;
}
//---
bool ZG_KeyframeTrack_Interpolate(ZG_KeyframeTrack * _this,float _t, float * _value)
{
	bool ok=false;

	// And then get the point...
	if(_this->keyframe_points->count < 1) // ---> No values ...
	{
		return false;
	}

	if(_this->keyframe_points->count == 1) // ---> No interpolation, holds same value all time...
	{
		*_value = ZG_CAST_BEZIER_POINT(_this->keyframe_points->items[0])->point[ZG_I1D_IDX_VALUE];
		return true;
	}

	if(_t <= _this->minx_interval)  // return first value...
	{

		*_value = ZG_CAST_BEZIER_POINT(_this->keyframe_points->items[0])->point[1];
		return true;
	}

	if(_t >= _this->maxx_interval) // return last value...
	{
		*_value = ZG_CAST_BEZIER_POINT(_this->keyframe_points->items[_this->keyframe_points->count-1])->point[1];
		return true;
	}

	// search span and interpolate usign ease function
	for(int i = 0; i <_this->keyframe_points->count-1; i++)
	{
		ok=true;
		ZG_KeyframePoint *point_i1 = (ZG_KeyframePoint *)_this->keyframe_points->items[i];
		ZG_KeyframePoint *point_i2 = (ZG_KeyframePoint *)_this->keyframe_points->items[i+1];
		float interpolate_value=0;

		if(_t >= point_i1->point[ZG_I1D_IDX_TIME] && //.independent_variable &&
				_t < point_i2->point[ZG_I1D_IDX_TIME]  //.independent_variable
		) // Calcule interpolated point...
		{
			float normalized_t=(_t-point_i1->point[ZG_I1D_IDX_TIME])*point_i1->inc_one_over_diff_interval;
			switch(_this->ease)
			{
			/*case EASE_BEZIER:
				ZG_Log_Error("Bezier interpolator not implemented");
				break;*/
			default:
			case ZG_EASE_LINEAR:
				interpolate_value=ZG_Ease_Linear(normalized_t);
				break;

			case ZG_EASE_IN_OUT_SINE:
				interpolate_value=ZG_Ease_InOutSine(normalized_t);
				break;
			}

			*_value=point_i1->point[ZG_I1D_IDX_VALUE] + interpolate_value*(point_i2->point[ZG_I1D_IDX_VALUE]-point_i1->point[ZG_I1D_IDX_VALUE]);
		}
	}

	return ok;
}
//---
void ZG_KeyframeTrack_Delete(ZG_KeyframeTrack *_this)
{
	ZG_KeyframeTrack_Clear(_this);
	ZG_List_Delete(_this->keyframe_points);
	ZG_FREE(_this);
}

