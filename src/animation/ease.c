#include "zg_animation.h"

float ease_linear( float t ){
	return t;
}

float ease_in_sine( float t ) {
	return sin( 1.5707963 * t );
}

float ease_out_sine( float t ) {
	return 1 + sin( 1.5707963 * (--t) );
}

float ease_in_out_sine( float t ) {
	return 0.5 * (1 + sin( 3.1415926 * (t - 0.5) ) );
}

float ease_in_quad( float t ) {
    return t * t;
}

float ease_out_quad( float t ) {
    return t * (2 - t);
}

float ease_in_out_quad( float t ) {
    return t < 0.5 ? 2 * t * t : t * (4 - 2 * t) - 1;
}

float ease_in_cubic( float t ) {
    return t * t * t;
}

float ease_out_cubic( float t ) {
    return 1 + (t-1) * t * t;
}

float ease_in_out_cubic( float t ) {
    return t < 0.5 ? 4 * t * t * t : 1 + (t-1) * (2 * (t-1)) * (2 * t);
}

float ease_in_quart( float t ) {
    t *= t;
    return t * t;
}

float ease_out_quart( float t ) {
    t = (t-1) * t;
    return 1 - t * t;
}

float ease_in_out_quart( float t ) {
    if( t < 0.5 ) {
        t *= t;
        return 8 * t * t;
    } else {
        t = (t-1) * t;
        return 1 - 8 * t * t;
    }
}

float ease_in_quint( float t ) {
    float t2 = t * t;
    return t * t2 * t2;
}

float ease_out_quint( float t ) {
    float t2 = (t-1) * t;
    return 1 + t * t2 * t2;
}

float ease_in_out_quint( float t ) {
    float t2;
    if( t < 0.5 ) {
        t2 = t * t;
        return 16 * t * t2 * t2;
    } else {
        t2 = (t-1) * t;
        return 1 + 16 * t * t2 * t2;
    }
}

float ease_in_expo( float t ) {
    return (pow( 2, 8 * t ) - 1) / 255;
}

float ease_out_expo( float t ) {
    return 1 - pow( 2, -8 * t );
}

float ease_in_out_expo( float t ) {
    if( t < 0.5 ) {
        return (pow( 2, 16 * t ) - 1) / 510;
    } else {
        return 1 - 0.5 * pow( 2, -16 * (t - 0.5) );
    }
}

float ease_in_circ( float t ) {
    return 1 - sqrt( 1 - t );
}

float ease_out_circ( float t ) {
    return sqrt( t );
}

float ease_in_out_circ( float t ) {
    if( t < 0.5 ) {
        return (1 - sqrt( 1 - 2 * t )) * 0.5;
    } else {
        return (1 + sqrt( 2 * t - 1 )) * 0.5;
    }
}

float ease_in_back( float t ) {
    return t * t * (2.70158 * t - 1.70158);
}

float ease_out_back( float t ) {
    return 1 + (t-1) * t * (2.70158 * t + 1.70158);
}

float ease_in_out_back( float t ) {
    if( t < 0.5 ) {
        return t * t * (7 * t - 2.5) * 2;
    } else {
        return 1 + (t-1) * t * 2 * (7 * t + 2.5);
    }
}

float ease_in_elastic( float t ) {
    float t2 = t * t;
    return t2 * t2 * sin( t * PI * 4.5 );
}

float ease_out_elastic( float t ) {
    float t2 = (t - 1) * (t - 1);
    return 1 - t2 * t2 * cos( t * PI * 4.5 );
}

float ease_in_out_elastic( float t ) {
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

float ease_in_bounce( float t ) {
    return pow( 2, 6 * (t - 1) ) * fabs( sin( t * PI * 3.5 ) );
}

float ease_out_bounce( float t ) {
    return 1 - pow( 2, -6 * t ) * fabs( cos( t * PI * 3.5 ) );
}

float ease_in_out_bounce( float t ) {
    if( t < 0.5 ) {
        return 8 * pow( 2, 8 * (t - 1) ) * fabs( sin( t * PI * 7 ) );
    } else {
        return 1 - 8 * pow( 2, -8 * t ) * fabs( sin( t * PI * 7 ) );
    }
}
