#include "../ZetGame.h"

int main(int argc, char *argv[]){
	ZetGame_Init(NULL);
	do{
		Graphics_BeginRender();

		glBegin(GL_TRIANGLES); // draw something with the texture on
			glVertex2f(-1,1);
			glVertex2f(1,-1);
			glVertex2f(-1,-1);
	   glEnd();

	   Graphics_EndRender();

		Input_Update();
	}while(!K_ESC);



	ZetGame_DeInit();

#ifdef __MEMMANAGER__
	MEMMGR_print_status();
#endif

	return 0;
}
