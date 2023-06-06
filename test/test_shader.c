#include "zetgame.h"

int main(int argc, char *argv[]){
	UNUSUED_PARAM(argc);
	UNUSUED_PARAM(argv);

	ZetGame_Init(NULL);

	ZG_Shader *s=Shader_Load(
		"../../../test/data/shaders/sample"
	);



	do{
		Graphics_BeginRender();

	if(s!=NULL){
	Shader_Use(s);
	}
	   Graphics_EndRender();

		Input_Update();
	}while(!K_ESC);


	Shader_Delete(s);

	ZetGame_DeInit();

	return 0;
}
