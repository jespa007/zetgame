#ifndef __ZG_CS_H__
#define __ZG_CS_H__


#include "graphics/zg_graphics.h"
#include "mmedia/zg_mmedia.h"
#include "animation/zg_animation.h"

// Entity
typedef struct EntitySystem EntitySystem;
typedef struct EntityManager EntityManager;
typedef struct Entity Entity;



// base
#include "entity/component/EComponent.h"
#include "animation/component/AComponent.h"

// entity
#include "entity/EntitySystem.h"
#include "entity/EntityManager.h"
#include "entity/Entity.h"



// Entity Component
#include "entity/component/ECTransform.h"
#include "entity/component/ECGeometry.h"
#include "entity/component/ECMaterial.h"
#include "entity/component/ECTexture.h"
#include "entity/component/ECSpriteRenderer.h"
#include "entity/component/ECTextBoxRenderer.h"


// Animation
#include "animation/AnimationSystem.h"

// Animation Component
#include "animation/component/ACTransformAnimation.h"
#include "animation/component/ACMaterialAnimation.h"



// Collision
#include "collision/CollisionSystem.h"


//#include "component/ECCamera.h"
//#include "component/ECText2d.h"

// TWEENS
//#include "component/ECTweenTransform.h"
//#include "component/ECViewer2d.h"
//#include "entity/EntityManager.h"

bool CS_Init(void);
void CS_DeInit(void);

#endif
