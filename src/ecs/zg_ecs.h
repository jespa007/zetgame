#ifndef __ZG_ECS_H__
#define __ZG_ECS_H__


#include "graphics/zg_graphics.h"
#include "mmedia/zg_mmedia.h"
#include "animation/zg_animation.h"

typedef unsigned int ComponentId;

// Entity
typedef struct EntitySystem EntitySystem;
typedef struct EntityManager EntityManager;
typedef struct Entity Entity;



// base
#include "../ecs/entity/component/EComponent.h"
#include "../ecs/animation/component/AComponent.h"

// entity
#include "../ecs/entity/EntitySystem.h"
#include "../ecs/entity/EntityManager.h"
#include "../ecs/entity/Entity.h"



// Entity Component
#include "../ecs/entity/component/ECTransform.h"
#include "../ecs/entity/component/ECGeometry.h"
#include "../ecs/entity/component/ECMaterial.h"
#include "../ecs/entity/component/ECTexture.h"
#include "../ecs/entity/component/ECSpriteRenderer.h"
#include "../ecs/entity/component/ECTextBoxRenderer.h"


// Animation
#include "../ecs/animation/AnimationSystem.h"

// Animation Component
#include "../ecs/animation/component/ACTransformAnimation.h"
#include "../ecs/animation/component/ACMaterialAnimation.h"



// Collision
#include "../ecs/collision/CollisionSystem.h"


//#include "component/ECCamera.h"
//#include "component/ECText2d.h"

// TWEENS
//#include "component/ECTweenTransform.h"
//#include "component/ECViewer2d.h"
//#include "entity/EntityManager.h"

bool ECS_Init(void);
void ECS_DeInit(void);

#endif
