#ifndef __ZG_ECS_H__
#define __ZG_ECS_H__


#include "graphics/_zg_graphics_.h"
#include "mmedia/_zg_mmedia_.h"
#include "animation/_zg_animation_.h"



// ZG_Component
typedef unsigned int ZG_EComponentId;

// ZG_Entity
typedef unsigned int ZG_Entity;
typedef unsigned int ZG_EntityManager;
typedef unsigned int ZG_EntityArchetype;




// base
#include "ecs/entity/component/ZG_EComponent.h"
#include "ecs/animation/component/ZG_AComponent.h"

// entity
#include "entity/ZG_EntityArchetype.h"
#include "entity/ZG_EntityArchetype.h"
#include "ecs/entity/ZG_Entity.h"



// ZG_Entity Component
#include "ecs/entity/component/ZG_ECTransform.h"
#include "ecs/entity/component/ZG_ECGeometry.h"
#include "ecs/entity/component/ZG_ECMaterial.h"
#include "ecs/entity/component/ZG_ECTexture.h"
#include "ecs/entity/component/ZG_ECSpriteRenderer.h"
#include "ecs/entity/component/ZG_ECTextBoxRenderer.h"


// Animation
#include "animation/ZG_AnimationManager.h"

// Animation Component
#include "ecs/animation/component/ZG_ACTransformAnimation.h"
#include "ecs/animation/component/ZG_ACMaterialAnimation.h"



// Collision
#include "ecs/collision/ZG_CollisionSystem.h"


//#include "component/ZG_ECCamera.h"
//#include "component/ZG_ECText2d.h"

// TWEENS
//#include "component/ECTweenTransform.h"
//#include "component/ECViewer2d.h"
//#include "entity/ZG_Archetype.h"

bool ZG_ECS_Init(void);
void ZG_ECS_DeInit(void);

#endif
