
// ZG_Entity
#include "animation/ZG_AnimationManager.c"
#include "ecs/animation/component/ZG_ACMaterialAnimation.c"
#include "ecs/animation/component/ZG_ACTransformAnimation.c"
#include "ecs/collision/ZG_CollisionSystem.c"
#include "ecs/entity/component/ZG_ECGeometry.c"
#include "ecs/entity/component/ZG_ECMaterial.c"
#include "ecs/entity/component/ZG_ECSpriteRenderer.c"
#include "ecs/entity/component/ZG_ECTextBoxRenderer.c"
#include "ecs/entity/component/ZG_ECTexture.c"
#include "ecs/entity/component/ZG_ECTransform.c"
#include "ecs/entity/ZG_Entity.c"
#include "entity/ZG_EntityArchetype.c"
#include "entity/ZG_EntityArchetype.c"





//#include "component/ZG_ECCamera.c"
//#include "component/ZG_ECText2d.c"

//#include "component/ECTweenTransform.c"
//#include "component/ECViewer2d.c"
//#include "entity/ZG_Archetype.c"

bool ZG_ECS_Init(void){
	if(!ZG_EntityManager_Init()){
		return false;
	}

	if(!ZG_AnimationSystem_Init()){
		return false;
	}



	return true;
}

void ZG_ECS_DeInit(void){
	ZG_EntityManager_DeInit();

	ZG_AnimationSystem_DeInit();


}
