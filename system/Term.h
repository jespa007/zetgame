// ccolor.hpp
#ifndef __TERM_H__
#define __TERM_H__


typedef enum{
	TERM_EFFECT_RESET=	0,
	TERM_EFFECT_BRIGHT=	1,
	TERM_EFFECT_DIM=	2,
	TERM_EFFECT_UNDERLINE=3,
	TERM_EFFECT_BLINK=	4,
	TERM_EFFECT_REVERSE=7,
	TERM_EFFECT_HIDDEN=	8

}TermEffect;

typedef enum{
	TERM_COLOR_BLACK= 	0,
	TERM_COLOR_RED=	1,
	TERM_COLOR_GREEN=	2,
	TERM_COLOR_YELLOW=	3,
	TERM_COLOR_BLUE=	4,
	TERM_COLOR_MAGENTA=5,
	TERM_COLOR_CYAN=	6,
	TERM_COLOR_WHITE=	7
}TermColor;

void  Term_SetColor(FILE *std_type, TermEffect attr, TermColor fg, TermColor bg);
void  Term_ResetColor(FILE *std_type);


#endif

