
#include "component/ECTransform.c"
#include "component/ECSpriteRenderer.c"
#include "component/ECAnimationTransform.c"
#include "component/ECAnimationMaterial.c"

#include "entity/Entity.c"
#include "entity/EntitySpriteRenderer.c"
#include "entity/EntityAnimationTransform.c"
#include "entity/EntityAnimationMaterial.c"



//#include "component/ECCamera.c"
//#include "component/ECText2d.c"

//#include "component/ECTweenTransform.c"
//#include "component/ECViewer2d.c"
//#include "entity/EntityType.c"
#include "system/ESSystem.c"

bool ECS_Init(void){
	if(!ESSystem_Init()){
		return false;
	}

	return true;
}

void ECS_DeInit(void){
	ESSystem_DeInit();
}
