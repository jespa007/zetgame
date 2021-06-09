#ifndef __ZG_ECS_H__
#define __ZG_ECS_H__




#include "graphics/zg_graphics.h"
#include "mmedia/zg_mmedia.h"

#include "component/EComponent.h"
#include "component/ECTransform.h"
#include "component/ECTransformAnimation.h"
#include "component/ECGeometry.h"
#include "component/ECMaterial.h"
#include "component/ECMaterialAnimation.h"
#include "component/ECTexture.h"
#include "component/ECSpriteRenderer.h"





#include "entity/Entity.h"



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
