
// Entity
#include "../ecs/animation/AnimationSystem.c"
#include "../ecs/animation/component/ACMaterialAnimation.c"
#include "../ecs/animation/component/ACTransformAnimation.c"
#include "../ecs/collision/CollisionSystem.c"
#include "../ecs/entity/component/ECGeometry.c"
#include "../ecs/entity/component/ECMaterial.c"
#include "../ecs/entity/component/ECSpriteRenderer.c"
#include "../ecs/entity/component/ECTextBoxRenderer.c"
#include "../ecs/entity/component/ECTexture.c"
#include "../ecs/entity/component/ECTransform.c"
#include "../ecs/entity/Entity.c"
#include "../ecs/entity/EntityManager.c"
#include "../ecs/entity/EntitySystem.c"





//#include "component/ECCamera.c"
//#include "component/ECText2d.c"

//#include "component/ECTweenTransform.c"
//#include "component/ECViewer2d.c"
//#include "entity/EntityManager.c"

bool ECS_Init(void){
	if(!EntitySystem_Init()){
		return false;
	}

	if(!AnimationSystem_Init()){
		return false;
	}



	return true;
}

void ECS_DeInit(void){
	EntitySystem_DeInit();

	AnimationSystem_DeInit();


}
