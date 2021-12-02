#include "ZetGame.h"



/*

void createcircle (float k, float r, float h) {

  glBegin(GL_LINES);
    for (int i = 0; i < 180; i++)
    {
    	float x,y,rot;
    x = r * cos(i) - h;
    y = r * sin(i) + k;
    glVertex3f(x + k,y - h,0);

    x = r * cos(i + 0.1) - h;
    y = r * sin(i + 0.1) + k;
    glVertex3f(x + k,y - h,0);
    }
    glEnd();
}*/

int main(int argc, char *argv[]){

	UNUSUED_PARAM(argc);
	UNUSUED_PARAM(argv);

	ZetGame_Init(NULL);

	do{
		Graphics_BeginRender();

		Geometry_Draw(Geometry_DefaultCircle());
		Geometry_Draw(Geometry_DefaultRectangle());


		Graphics_EndRender();

		Input_Update();
	}while(!K_ESC);


	ZetGame_DeInit();

#ifdef __MEMMANAGER__
	MEMMGR_print_status();
#endif

	return 0;
}
