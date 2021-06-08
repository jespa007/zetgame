#include "ecs/zg_ecs.h"


void ECMaterial_Setup(void *_this){
	ECMaterial *ec_material=_this;
	ec_material->material=Material_New();
}

void ECMaterial_Destroy(void *_this){
	ECMaterial *ec_material=_this;
	Material_Delete(ec_material->material);
}
