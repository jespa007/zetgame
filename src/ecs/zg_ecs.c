
#include "component/ECTransform.c"
#include "component/ECTransformAnimation.c"
#include "component/ECGeometry.c"
#include "component/ECMaterial.c"
#include "component/ECMaterialAnimation.c"
#include "component/ECTexture.c"
#include "component/ECSpriteRenderer.c"



#include "entity/Entity.c"
#include "entity/EntityManager.c"




//#include "component/ECCamera.c"
//#include "component/ECText2d.c"

//#include "component/ECTweenTransform.c"
//#include "component/ECViewer2d.c"
//#include "entity/EntityManager.c"
#include "system/EntitySystem.c"

bool ECS_Init(void){
	if(!EntitySystem_Init()){
		return false;
	}

	return true;
}

void ECS_DeInit(void){
	EntitySystem_DeInit();
}
