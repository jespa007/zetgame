#include "_zg_base_.h"
#define ZG_VAR_LOG(l) ((l) == ZG_LOG_LEVEL_ERROR?"ERR": (l)==ZG_LOG_LEVEL_WARNING?"WRN": (l) == ZG_LOG_LEVEL_INFO ? "INF" : (l) == ZG_LOG_LEVEL_DEBUG ? "DBG" : "NAN" )
#define ZG_MAX_LOGOUT_STRING 1024
	
static bool g_ansi_escape=true;

void  ZG_Log_SetAnsiEscape(bool _use){
	g_ansi_escape=_use;
}

void  ZG_Log_Print(const  char  *file,  int  line, ZG_LogLevel level, const  char  *text_in,...) {

	char  text_out[MAX_PATH]={0};
	ZG_VARGS(text_out, text_in);

	FILE *std_type=stdout;
	char filename[MAX_PATH]={"unknow"};

	switch(level)
	{
		case ZG_LOG_LEVEL_WARNING:
	  case ZG_LOG_LEVEL_ERROR:
		std_type=stderr;
	break;
	  default:
	break;
	}

	if(file != NULL){
		char *filename_aux = ZG_Path_GetFilename(file);
		if(filename_aux != NULL){
			strcpy(filename,filename_aux);
			free(filename_aux);
		}

	}

	if(!g_ansi_escape){
		#ifdef _WIN32
		  int colors[7];
		  colors[0] = FOREGROUND_RED   | FOREGROUND_INTENSITY;//FOREGROUND_BLUE  | FOREGROUND_RED   | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
		  colors[1] = FOREGROUND_GREEN | FOREGROUND_RED   | FOREGROUND_INTENSITY;
		  colors[2] = FOREGROUND_BLUE  | FOREGROUND_GREEN | FOREGROUND_RED   | FOREGROUND_INTENSITY;
		  colors[3] = FOREGROUND_RED   | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
		  colors[4] = FOREGROUND_BLUE  | FOREGROUND_INTENSITY;
		  colors[5] = FOREGROUND_GREEN | FOREGROUND_INTENSITY;
		  colors[6] = FOREGROUND_RED   | FOREGROUND_INTENSITY;
		  SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), colors[level]);
		#endif
	}else{

		  switch(level)
		  {
			case ZG_LOG_LEVEL_ERROR:
				ZG_Term_SetColor(std_type,ZG_TERM_EFFECT_BRIGHT, ZG_TERM_COLOR_WHITE, ZG_TERM_COLOR_RED);
				break;
			case ZG_LOG_LEVEL_WARNING:
				ZG_Term_SetColor(std_type,ZG_TERM_EFFECT_BRIGHT,ZG_TERM_COLOR_WHITE, ZG_TERM_COLOR_YELLOW);
				break;
			default:

			break;
		  }
	}

	if(file == NULL)
		fprintf(std_type,"%s",text_out);
	else{
		fprintf(std_type,"[ %20s:%04i - %3s]=* %s",filename,line,ZG_VAR_LOG(level),text_out);
		//fprintf(stderr,"[ %30s:%04i - %3s]=%s",filename.c_str(),line,VAR_LOG(level),text);
	}

	 if(!g_ansi_escape){
	#ifdef _WIN32
		// restore ...
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_BLUE  | FOREGROUND_GREEN | FOREGROUND_RED );
	#endif
	 }else{
	  ZG_Term_SetColor(std_type,ZG_TERM_EFFECT_BRIGHT, ZG_TERM_COLOR_WHITE, ZG_TERM_COLOR_BLACK);
	 }

    fprintf(std_type,"\n");


	  // to ensure all data has been write...
	  fflush(std_type);
}




