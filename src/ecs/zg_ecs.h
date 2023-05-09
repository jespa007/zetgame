#ifndef __ZG_ECS_H__
#define __ZG_ECS_H__


#include "graphics/zg_graphics.h"
#include "mmedia/zg_mmedia.h"
#include "animation/zg_animation.h"

#include "entity/Entity.h"

#include "component/EComponent.h"
#include "component/ECTransform.h"
#include "component/ECTransformAnimation.h"
#include "component/ECGeometry.h"
#include "component/ECMaterial.h"
#include "component/ECMaterialAnimation.h"
#include "component/ECTexture.h"
#include "component/ECSpriteRenderer.h"









//#include "component/ECCamera.h"
//#include "component/ECText2d.h"

// TWEENS
//#include "component/ECTweenTransform.h"
//#include "component/ECViewer2d.h"
//#include "entity/EntityManager.h"
#include "system/EntitySystem.h"

bool ECS_Init(void);
void ECS_DeInit(void);

#endif
