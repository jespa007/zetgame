#include "@zg_animation.h"

float ZG_Ease_Linear( float t ){
	return t;
}

float ZG_Ease_InSine( float t ) {
	return sin( 1.5707963 * t );
}

float ZG_Ease_OutSine( float t ) {
	return 1 + sin( 1.5707963 * (--t) );
}

float ZG_Ease_InOutSine( float t ) {
	return 0.5 * (1 + sin( 3.1415926 * (t - 0.5) ) );
}

float ZG_Ease_InQuad( float t ) {
    return t * t;
}

float ZG_Ease_OutQuad( float t ) {
    return t * (2 - t);
}

float ZG_Ease_InOutQuad( float t ) {
    return t < 0.5 ? 2 * t * t : t * (4 - 2 * t) - 1;
}

float ZG_Ease_InCubic( float t ) {
    return t * t * t;
}

float ZG_Ease_OutCubic( float t ) {
    return 1 + (t-1) * t * t;
}

float ZG_Ease_InOutCubic( float t ) {
    return t < 0.5 ? 4 * t * t * t : 1 + (t-1) * (2 * (t-1)) * (2 * t);
}

float ZG_Ease_InQuart( float t ) {
    t *= t;
    return t * t;
}

float ZG_Ease_OutQuart( float t ) {
    t = (t-1) * t;
    return 1 - t * t;
}

float ZG_Ease_InOutQuart( float t ) {
    if( t < 0.5 ) {
        t *= t;
        return 8 * t * t;
    } else {
        t = (t-1) * t;
        return 1 - 8 * t * t;
    }
}

float ZG_Ease_InQuint( float t ) {
    float t2 = t * t;
    return t * t2 * t2;
}

float ZG_Ease_OutQuint( float t ) {
    float t2 = (t-1) * t;
    return 1 + t * t2 * t2;
}

float ZG_Ease_InOutQuint( float t ) {
    float t2;
    if( t < 0.5 ) {
        t2 = t * t;
        return 16 * t * t2 * t2;
    } else {
        t2 = (t-1) * t;
        return 1 + 16 * t * t2 * t2;
    }
}

float ZG_Ease_InExpo( float t ) {
    return (pow( 2, 8 * t ) - 1) / 255;
}

float ZG_Ease_OutExpo( float t ) {
    return 1 - pow( 2, -8 * t );
}

float ZG_Ease_InOutExpo( float t ) {
    if( t < 0.5 ) {
        return (pow( 2, 16 * t ) - 1) / 510;
    } else {
        return 1 - 0.5 * pow( 2, -16 * (t - 0.5) );
    }
}

float ZG_Ease_InCirc( float t ) {
    return 1 - sqrt( 1 - t );
}

float ZG_Ease_OutCirc( float t ) {
    return sqrt( t );
}

float ZG_Ease_InOutCirc( float t ) {
    if( t < 0.5 ) {
        return (1 - sqrt( 1 - 2 * t )) * 0.5;
    } else {
        return (1 + sqrt( 2 * t - 1 )) * 0.5;
    }
}

float ZG_Ease_InBack( float t ) {
    return t * t * (2.70158 * t - 1.70158);
}

float ZG_Ease_OutBack( float t ) {
    return 1 + (t-1) * t * (2.70158 * t + 1.70158);
}

float ZG_Ease_InOutBack( float t ) {
    if( t < 0.5 ) {
        return t * t * (7 * t - 2.5) * 2;
    } else {
        return 1 + (t-1) * t * 2 * (7 * t + 2.5);
    }
}

float ZG_Ease_InElastic( float t ) {
    float t2 = t * t;
    return t2 * t2 * sin( t * PI * 4.5 );
}

float ZG_Ease_OutElastic( float t ) {
    float t2 = (t - 1) * (t - 1);
    return 1 - t2 * t2 * cos( t * PI * 4.5 );
}

float ZG_Ease_InOutElastic( float t ) {
    float t2;
    if( t < 0.45 ) {
        t2 = t * t;
        return 8 * t2 * t2 * sin( t * PI * 9 );
    } else if( t < 0.55 ) {
        return 0.5 + 0.75 * sin( t * PI * 4 );
    } else {
        t2 = (t - 1) * (t - 1);
        return 1 - 8 * t2 * t2 * sin( t * PI * 9 );
    }
}

float ZG_Ease_InBounce( float t ) {
    return pow( 2, 6 * (t - 1) ) * fabs( sin( t * PI * 3.5 ) );
}

float ZG_Ease_OutBounce( float t ) {
    return 1 - pow( 2, -6 * t ) * fabs( cos( t * PI * 3.5 ) );
}

float ZG_Ease_InOutBounce( float t ) {
    if( t < 0.5 ) {
        return 8 * pow( 2, 8 * (t - 1) ) * fabs( sin( t * PI * 7 ) );
    } else {
        return 1 - 8 * pow( 2, -8 * t ) * fabs( sin( t * PI * 7 ) );
    }
}
