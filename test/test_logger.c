#include "../ZetgineCore.h"


int main(int argc, char *argv[]){

	ZetgineCore_Init(NULL);

	Logger *l=Logger_New();
	PartialLogger *lp1=PartialLogger_New(); // will write every 100ms
	PartialLogger *lp2=PartialLogger_New(); // will write every 200ms


	Logger_Start(l);

	do{

	}while(K_ESC);

	SDL_Delay(5000);

	Logger_Stop(l);

	ZetgineCore_DeInit();

#ifdef __MEMMANAGER__
	MEMMGR_print_status();
#endif

	return 0;
}
