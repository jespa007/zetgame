#ifndef __EASE_H__
#define __EASE_H__

typedef enum{
	EASE_LINEAR=0,
	EASE_IN_SINE,
	EASE_OUT_SINE,
	EASE_IN_OUT_SINE,
	EASE_IN_QUAD,
	EASE_OUT_QUAD,
	EASE_IN_OUT_QUAD,
	EASE_IN_CUBIC,
	EASE_OUT_CUBIC,
	EASE_IN_OUT_CUBIC,
	EASE_IN_QUART,
	EASE_OUT_QUART,
	EASE_IN_OUT_QUART,
	EASE_IN_QUINT,
	EASE_OUT_QUINT,
	EASE_IN_OUT_QUINT,
	EASE_IN_EXPO,
	EASE_OUT_EXPO,
	EASE_IN_OUT_EXPO,
	EASE_IN_CIRC,
	EASE_OUT_CIRC,
	EASE_IN_OUT_CIRC,
	EASE_IN_BACK,
	EASE_OUT_BACK,
	EASE_IN_OUT_BACK,
	EASE_IN_ELASTIC,
	EASE_OUT_ELASTIC,
	EASE_IN_OUT_ELASTIC,
	EASE_IN_BOUNCE,
	EASE_OUT_BOUNCE,
	EASE_IN_OUT_BOUNCE,

}Ease;


float ease_linear( float t );
float ease_in_sine( float t );
float ease_out_sine( float t );
float ease_in_out_sine( float t );
float ease_in_quad( float t );
float ease_out_quad( float t );
float ease_in_out_quad( float t );
float ease_in_cubic( float t );
float ease_out_cubic( float t );
float ease_in_out_cubic( float t );
float ease_in_quart( float t );
float ease_out_quart( float t );
float ease_in_out_quart( float t );
float ease_in_quint( float t );
float ease_out_quint( float t );
float ease_in_out_quint( float t );
float ease_in_expo( float t );
float ease_out_expo( float t );
float ease_in_out_expo( float t );
float ease_in_circ( float t );
float ease_out_circ( float t );
float ease_in_out_circ( float t );
float ease_in_back( float t );
float ease_out_back( float t );
float ease_in_out_back( float t );
float ease_in_elastic( float t );
float ease_out_elastic( float t );
float ease_in_out_elastic( float t );
float ease_in_bounce( float t );
float ease_out_bounce( float t );
float ease_in_out_bounce( float t );

#endif
