#include "zg_animation.h"

#define CAST_BEZIER_POINT(p) ((KeyframePoint *)(p))


//---------------------------------------------------------------------------------------------------
// PUBLIC
KeyframeTrack * KeyframeTrack_New(void)
{
	KeyframeTrack *int1d = ZG_NEW(KeyframeTrack);
	int1d->ease = EASE_LINEAR;
	int1d->minx_interval = FLT_MAX,
	int1d->maxx_interval = FLT_MIN,
	int1d->rangex_interval   = FLT_MIN;
	int1d->rangex_interval_pow2 = 0;
	int1d->keyframe_points=List_New();
	int1d->dxdt_pow2 =  1;

	return int1d;

}


//---------------------------------------------------------------------------------------------------
// STATIC

Ease 	KeyframeTrack_ParseInterpolatorType(const char * interpolator_type_str){
	Ease ease=EASE_LINEAR;

	if(STRCMP(interpolator_type_str,==,"LINEAR")) {
		ease=EASE_LINEAR;
	}else if(STRCMP(interpolator_type_str,==,"IN_OUT_SINE")) {
		ease=EASE_IN_OUT_SINE;
	}else {
		Log_ErrorF("Invalid EASE type \"%s\": Valid ones are \"LINEAR\" or \"IN_OUT_SINE\"");
	}


	return ease;
}


void KeyframeTrack_CalculeFx(
	  KeyframePoint * result
	, const float  ini_point[I1D_POINT_SIZE]
	, const  float   ini_control_point_1[I1D_POINT_SIZE]
	, const  float  ini_control_point_2[I1D_POINT_SIZE]
	, float iv_ini
	, const float  end_point[I1D_POINT_SIZE]
	, const  float  end_control_point_1[I1D_POINT_SIZE]
    , const  float  end_control_point_2[I1D_POINT_SIZE]
    , float iv_end)
{

	UNUSUED_PARAM(ini_control_point_1);
	UNUSUED_PARAM(ini_control_point_2);
	UNUSUED_PARAM(end_control_point_1);
	UNUSUED_PARAM(end_control_point_2);

	//
	// PRE: Assumes that result, ini_point && end_point is equal to n_dimension

	float diff = (iv_end-iv_ini);
	float inc_one_over_diff_interval = 1.0f/(diff);

	result->inc_fx[I1D_IDX_VALUE] = (end_point[I1D_IDX_VALUE]-ini_point[I1D_IDX_VALUE])/diff;
	result->inc_one_over_diff_interval = inc_one_over_diff_interval;
}
//---------------------------------------------------------------------------------------------------
// PRIVATE
void KeyframeTrack_SetEase(KeyframeTrack * _this, Ease _ease){
	_this->ease=_ease;
}
//---
void KeyframeTrack_Clear(KeyframeTrack *_this)
{
	for(unsigned i = 0; i < _this->keyframe_points->count; i++)
	{
		KeyframePoint_Delete(_this->keyframe_points->items[i]);
	}

	List_Clear(_this->keyframe_points);
}
//---
void KeyframeTrack_AddKeyframesFloat(KeyframeTrack *_this,const float * _points, size_t _points_count)
{
	// PRE:
	// point[x1,x2,...xn-1]: Dependen var (x,y,z, etc).
	// point[xn]: Independent var (time).
	if(_points_count==0){ // no keyframe_points...
		return;
	}

	if(_points_count % I1D_POINT_SIZE != 0)
	{
		Log_ErrorF("Error! Data length is not equal to dimension+1");
		return;
	}

	int number_keyframes = _points_count/I1D_POINT_SIZE;
	KeyframePoint *new_data=NULL, *previus_data=NULL;

	// Get the independent point...
	float x_ini, x_actual, x_old;
	float x_end;


	int i;
	int length_offset = I1D_POINT_SIZE;
	x_actual = _points[I1D_IDX_TIME];


	x_old = x_actual-1.0f;
	//x_actual = point[N_DIMENSION];
	int actual_offset = 0;


	for(i = 0; i < number_keyframes; i++)
	{
		x_actual = _points[actual_offset + I1D_IDX_TIME];

		if(x_actual < x_old)
		{
			Log_Error("Malformed list point at index %i (independent variable is not ascendent) (last independent x: %f,current independent x: %f)",i,x_old, x_actual);

			return;
		}

		new_data = KeyframePoint_New();

		// copy first point...
		new_data->point[I1D_IDX_TIME] 		  = _points[actual_offset + I1D_IDX_TIME];
		new_data->point[I1D_IDX_VALUE] 		  = _points[actual_offset + I1D_IDX_VALUE];

		// Interpolates with prevous point...
		if(i > 0)
		{
			previus_data = _this->keyframe_points->items[i-1];
			KeyframeTrack_CalculeFx(previus_data,
					previus_data->point,
					previus_data->control_point_1,
					previus_data->control_point_2,
					previus_data->point[I1D_IDX_TIME],
					new_data->point,
					new_data->control_point_1,
					new_data->control_point_2,
					new_data->point[I1D_IDX_TIME]
			);

			_this->keyframe_points->items[i-1] = previus_data;
		}

		List_Insert(_this->keyframe_points,i, new_data);
		x_old = x_actual;
		actual_offset += length_offset;
	}

	x_end = _points[(number_keyframes-1)*length_offset+ I1D_IDX_TIME];
	x_ini = _points[(0)*length_offset +              I1D_IDX_TIME];

	_this->minx_interval = MIN(_this->minx_interval, x_ini);
	_this->maxx_interval = MAX(_this->maxx_interval, x_end);

	_this->rangex_interval = _this->maxx_interval-_this->minx_interval;
}
//---
void KeyframeTrack_SetKeyframes(KeyframeTrack *_this,float * _points, size_t _points_count)
{
	KeyframeTrack_Clear(_this);
	KeyframeTrack_AddKeyframesFloat(_this,_points,_points_count);

	// optimization independent var as pow2
	_this->rangex_interval_pow2=Bit_NextPow2(_this->rangex_interval);
	_this->dxdt_pow2=(float)_this->rangex_interval_pow2/_this->rangex_interval;
	_this->properties|=KEYFRAME_TRACK_PROPERTY_TIME_POW_2;

	// TODO: set all independent keyframe_points 1/maxx_interval_mask;
}
//---
void KeyframeTrack_AddKeyframesBezier(KeyframeTrack * _this,List * _keyframe_points)
{
	// PRE:
	// point[x1,x2,...xn-1]: Dependen var (x,y,z, etc).
	// point[xn]: Independent var (time).
	int x_actual=-1;
	int x_old=-1;


	KeyframePoint *new_data,*first_point, *last_point;

	for(unsigned i = 0; i < _keyframe_points->count; i++)
	{
		KeyframePoint * bp_actual=CAST_BEZIER_POINT(_keyframe_points->items[i]);

		x_actual = bp_actual->point[I1D_IDX_TIME];//actual_offset + 2*I1D_IDX_TIME];

		if(x_actual < x_old)
		{
			Log_Error("Malformed list point at index %i (independent variable is not ascendent) (p: %i,a: %i)",i,x_old, x_actual);

			return;
		}

		new_data = KeyframePoint_New();

		for(unsigned j=0; j< I1D_POINT_SIZE; j++){
			new_data->control_point_1[j] 	= bp_actual->control_point_1[j];
			new_data->point[j]				= bp_actual->point[j];
			new_data->control_point_2[j] 	= bp_actual->control_point_2[j];
		}

		// Interpolates with prevous point...
		if(i > 0)
		{
			KeyframePoint * previus_data = _this->keyframe_points->items[i-1];
			KeyframeTrack_CalculeFx(previus_data,
					previus_data->point,
					previus_data->control_point_1,
					previus_data->control_point_2,
					previus_data->point[I1D_IDX_TIME],
					new_data->point,
					new_data->control_point_1,
					new_data->control_point_2,
					new_data->point[I1D_IDX_TIME]
			);
		}

		List_Insert(_this->keyframe_points,i, new_data);

		x_old = x_actual;
	}

	first_point=_keyframe_points->items[0];
	last_point=_keyframe_points->items[_keyframe_points->count-1];

	_this->minx_interval = MIN(_this->minx_interval, last_point->point[I1D_IDX_TIME]);
	_this->maxx_interval = MAX(_this->maxx_interval, first_point->point[I1D_IDX_TIME]);

	_this->rangex_interval = _this->maxx_interval-_this->minx_interval;
}
//---
bool KeyframeTrack_Interpolate(KeyframeTrack * _this,float _t, float * _value)
{
	bool ok=false;

	// And then get the point...
	if(_this->keyframe_points->count < 1) // ---> No values ...
	{
		return false;
	}

	if(_this->keyframe_points->count == 1) // ---> No interpolation, holds same value all time...
	{
		*_value = CAST_BEZIER_POINT(_this->keyframe_points->items[0])->point[I1D_IDX_VALUE];
		return true;
	}

	if(_t <= _this->minx_interval)  // return first value...
	{

		*_value = CAST_BEZIER_POINT(_this->keyframe_points->items[0])->point[1];
		return true;
	}

	if(_t >= _this->maxx_interval) // return last value...
	{
		*_value = CAST_BEZIER_POINT(_this->keyframe_points->items[_this->keyframe_points->count-1])->point[1];
		return true;
	}

	// search span and interpolate usign ease function
	for(int i = 0; i <_this->keyframe_points->count-1; i++)
	{
		ok=true;
		KeyframePoint *point_i1 = (KeyframePoint *)_this->keyframe_points->items[i];
		KeyframePoint *point_i2 = (KeyframePoint *)_this->keyframe_points->items[i+1];
		float interpolate_value=0;

		if(_t >= point_i1->point[I1D_IDX_TIME] && //.independent_variable &&
				_t < point_i2->point[I1D_IDX_TIME]  //.independent_variable
		) // Calcule interpolated point...
		{
			float normalized_t=(_t-point_i1->point[I1D_IDX_TIME])*point_i1->inc_one_over_diff_interval;
			switch(_this->ease)
			{
			/*case EASE_BEZIER:
				Log_Error("Bezier interpolator not implemented");
				break;*/
			default:
			case EASE_LINEAR:
				interpolate_value=ease_linear(normalized_t);
				break;

			case EASE_IN_OUT_SINE:
				interpolate_value=ease_in_out_sine(normalized_t);
				break;
			}

			*_value=point_i1->point[I1D_IDX_VALUE] + interpolate_value*(point_i2->point[I1D_IDX_VALUE]-point_i1->point[I1D_IDX_VALUE]);
		}
	}

	return ok;
}
//---
void KeyframeTrack_Delete(KeyframeTrack *_this)
{
	KeyframeTrack_Clear(_this);
	List_Delete(_this->keyframe_points);
	ZG_FREE(_this);
}

