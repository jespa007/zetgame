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


float ZN_Ease_Linear( float t );
float ZN_Ease_InSine( float t );
float ZN_Ease_OutSine( float t );
float ZN_Ease_InOutSine( float t );
float ZN_Ease_InQuad( float t );
float ZN_Ease_OutQuad( float t );
float ZN_Ease_InOutQuad( float t );
float ZN_Ease_InCubic( float t );
float ZN_Ease_OutCubic( float t );
float ZN_Ease_InOutCubic( float t );
float ZN_Ease_InQuart( float t );
float ZN_Ease_OutQuart( float t );
float ZN_Ease_InOutQuart( float t );
float ZN_Ease_InQuint( float t );
float ZN_Ease_OutQuint( float t );
float ZN_Ease_InOutQuint( float t );
float ZN_Ease_InExpo( float t );
float ZN_Ease_OutExpo( float t );
float ZN_Ease_InOutExpo( float t );
float ZN_Ease_InCirc( float t );
float ZN_Ease_OutCirc( float t );
float ZN_Ease_InOutCirc( float t );
float ZN_Ease_InBack( float t );
float ZN_Ease_OutBack( float t );
float ZN_Ease_InOutBack( float t );
float ZN_Ease_InElastic( float t );
float ZN_Ease_OutElastic( float t );
float ZN_Ease_InOutElastic( float t );
float ZN_Ease_InBounce( float t );
float ZN_Ease_OutBounce( float t );
float ZN_Ease_InOutBounce( float t );

#endif
