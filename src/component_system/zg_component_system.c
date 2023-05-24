
// Entity
#include "entity/Entity.c"
#include "entity/EntityManager.c"
#include "entity/EntitySystem.c"



// Entity Component
#include "entity/component/ECTransform.c"
#include "entity/component/ECGeometry.c"
#include "entity/component/ECMaterial.c"
#include "entity/component/ECTexture.c"
#include "entity/component/ECSpriteRenderer.c"
#include "entity/component/ECTextBoxRenderer.c"




// Animation
#include "animation/AnimationSystem.c"

// Animation Component
#include "animation/component/ACTransformAnimation.c"
#include "animation/component/ACMaterialAnimation.c"


// Collision
#include "collision/CollisionSystem.c"





//#include "component/ECCamera.c"
//#include "component/ECText2d.c"

//#include "component/ECTweenTransform.c"
//#include "component/ECViewer2d.c"
//#include "entity/EntityManager.c"

bool CS_Init(void){
	if(!EntitySystem_Init()){
		return false;
	}

	if(!AnimationSystem_Init()){
		return false;
	}



	return true;
}

void CS_DeInit(void){
	EntitySystem_DeInit();

	AnimationSystem_DeInit();


}
