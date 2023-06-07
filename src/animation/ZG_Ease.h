#ifndef __ZG_EASE_H__
#define __ZG_EASE_H__

typedef enum{
	ZG_EASE_LINEAR=0,
	ZG_EASE_IN_SINE,
	ZG_EASE_OUT_SINE,
	ZG_EASE_IN_OUT_SINE,
	ZG_EASE_IN_QUAD,
	ZG_EASE_OUT_QUAD,
	ZG_EASE_IN_OUT_QUAD,
	ZG_EASE_IN_CUBIC,
	ZG_EASE_OUT_CUBIC,
	ZG_EASE_IN_OUT_CUBIC,
	ZG_EASE_IN_QUART,
	ZG_EASE_OUT_QUART,
	ZG_EASE_IN_OUT_QUART,
	ZG_EASE_IN_QUINT,
	ZG_EASE_OUT_QUINT,
	ZG_EASE_IN_OUT_QUINT,
	ZG_EASE_IN_EXPO,
	ZG_EASE_OUT_EXPO,
	ZG_EASE_IN_OUT_EXPO,
	ZG_EASE_IN_CIRC,
	ZG_EASE_OUT_CIRC,
	ZG_EASE_IN_OUT_CIRC,
	ZG_EASE_IN_BACK,
	ZG_EASE_OUT_BACK,
	ZG_EASE_IN_OUT_BACK,
	ZG_EASE_IN_ELASTIC,
	ZG_EASE_OUT_ELASTIC,
	ZG_EASE_IN_OUT_ELASTIC,
	ZG_EASE_IN_BOUNCE,
	ZG_EASE_OUT_BOUNCE,
	ZG_EASE_IN_OUT_BOUNCE,

}Ease;


float ZG_Ease_Linear( float t );
float ZG_Ease_InSine( float t );
float ZG_Ease_OutSine( float t );
float ZG_Ease_InOutSine( float t );
float ZG_Ease_InQuad( float t );
float ZG_Ease_OutQuad( float t );
float ZG_Ease_InOutQuad( float t );
float ZG_Ease_InCubic( float t );
float ZG_Ease_OutCubic( float t );
float ZG_Ease_InOutCubic( float t );
float ZG_Ease_InQuart( float t );
float ZG_Ease_OutQuart( float t );
float ZG_Ease_InOutQuart( float t );
float ZG_Ease_InQuint( float t );
float ZG_Ease_OutQuint( float t );
float ZG_Ease_InOutQuint( float t );
float ZG_Ease_InExpo( float t );
float ZG_Ease_OutExpo( float t );
float ZG_Ease_InOutExpo( float t );
float ZG_Ease_InCirc( float t );
float ZG_Ease_OutCirc( float t );
float ZG_Ease_InOutCirc( float t );
float ZG_Ease_InBack( float t );
float ZG_Ease_OutBack( float t );
float ZG_Ease_InOutBack( float t );
float ZG_Ease_InElastic( float t );
float ZG_Ease_OutElastic( float t );
float ZG_Ease_InOutElastic( float t );
float ZG_Ease_InBounce( float t );
float ZG_Ease_OutBounce( float t );
float ZG_Ease_InOutBounce( float t );

#endif
