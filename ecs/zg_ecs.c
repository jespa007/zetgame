
#include "component/ECTransform.c"
#include "component/ECTransformAnimation.c"
#include "component/ECSpriteRenderer.c"
#include "component/ECGeometry.c"
#include "component/ECMaterial.c"
#include "component/ECMaterialAnimation.c"
#include "component/ECTexture.c"

#include "entity/Entity.c"
#include "entity/EntitySpriteRenderer.c"
#include "entity/EntityTransformAnimation.c"
#include "entity/EntityMaterialAnimation.c"



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
