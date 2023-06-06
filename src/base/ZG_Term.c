#include "_zg_base_.h"


void ZG_Term_SetColor(FILE *std_type, TermEffect attr, TermColor fg, TermColor bg)
{
	char command[13]={0};

	/* Command is the control command to the terminal */
	sprintf(command, "%c[%d;%d;%dm", 0x1B, attr, fg + 30, bg + 40);
	fprintf(std_type,"%s", command);
}

void ZG_Term_ResetColor(FILE *std_type)
{
	int attr = ZG_TERM_EFFECT_RESET;
	int fg = ZG_TERM_COLOR_BLACK;
	int bg= ZG_TERM_COLOR_WHITE;

	char command[13]={0};

	/* Command is the control command to the terminal */
	sprintf(command, "%c[%d;%d;%dm", 0x1B, attr, fg + 30, bg + 40);
	fprintf(std_type, "%s", command);
}

