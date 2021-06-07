#ifndef __ZG_ECS_H__
#define __ZG_ECS_H__




#include "graphics/zg_graphics.h"
#include "mmedia/zg_mmedia.h"

#include "component/ECTransform.h"
#include "component/ECGeometry.h"
#include "component/ECMaterial.h"
#include "component/ECTexture.h"

#include "component/ECSpriteRenderer.h"
#include "component/ECAnimationTransform.h"
#include "component/ECAnimationMaterial.h"

#include "entity/EntityComponent.h"
#include "entity/Entity.h"

#include "entity/EntityTransform.h"
#include "entity/EntitySpriteRenderer.h"
#include "entity/EntityAnimationTransform.h"
#include "entity/EntityAnimationMaterial.h"


//#include "component/ECCamera.h"
//#include "component/ECText2d.h"

// TWEENS
//#include "component/ECTweenTransform.h"
//#include "component/ECViewer2d.h"
//#include "entity/EntityType.h"
#include "system/ESSystem.h"

bool ECS_Init(void);
void ECS_DeInit(void);

#endif
