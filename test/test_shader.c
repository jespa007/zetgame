#include "zetgame.h"

int main(int argc, char *argv[]){
	ZG_UNUSUED_PARAM(argc);
	ZG_UNUSUED_PARAM(argv);

	ZG_Init(NULL);

	ZG_Shader *s=Shader_Load(
		"../../../test/data/shaders/sample"
	);



	do{
		ZG_Graphics_BeginRender();

	if(s!=NULL){
	Shader_Use(s);
	}
	   ZG_Graphics_EndRender();

		ZG_Input_Update();
	}while(!K_ESC);


	Shader_Delete(s);

	ZG_DeInit();

	return 0;
}
