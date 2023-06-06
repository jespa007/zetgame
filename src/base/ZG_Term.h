// ccolor.hpp
#ifndef __ZG_ZG_TERM_H__
#define __ZG_ZG_TERM_H__


typedef enum{
	ZG_TERM_EFFECT_RESET=	0,
	ZG_TERM_EFFECT_BRIGHT=	1,
	ZG_TERM_EFFECT_DIM=	2,
	ZG_TERM_EFFECT_UNDERLINE=3,
	ZG_TERM_EFFECT_BLINK=	4,
	ZG_TERM_EFFECT_REVERSE=7,
	ZG_TERM_EFFECT_HIDDEN=	8

}TermEffect;

typedef enum{
	ZG_TERM_COLOR_BLACK= 	0,
	ZG_TERM_COLOR_RED=	1,
	ZG_TERM_COLOR_GREEN=	2,
	ZG_TERM_COLOR_YELLOW=	3,
	ZG_TERM_COLOR_BLUE=	4,
	ZG_TERM_COLOR_MAGENTA=5,
	ZG_TERM_COLOR_CYAN=	6,
	ZG_TERM_COLOR_WHITE=	7
}TermColor;

void  ZG_Term_SetColor(FILE *std_type, TermEffect attr, TermColor fg, TermColor bg);
void  ZG_Term_ResetColor(FILE *std_type);


#endif

