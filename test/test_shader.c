#include "zetgame.h"

int main(int argc, char *argv[]){
	ZG_UNUSUED_PARAM(argc);
	ZG_UNUSUED_PARAM(argv);

	ZG_Init(NULL);

	ZG_Shader *s=ZG_Shader_Load(
		"../../../test/data/shaders/sample"
	);



	do{
		ZG_Graphics_BeginRender();

	if(s!=NULL){
		ZG_Shader_Use(s);
	}
	   ZG_Graphics_EndRender();

		ZG_Input_Update();
	}while(!ZG_KP_ESC);


	ZG_Shader_Delete(s);

	ZG_DeInit();

	return 0;
}
