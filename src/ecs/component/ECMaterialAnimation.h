#ifndef _EC_MATERIAL_ANIMATION_H__
#define _EC_MATERIAL_ANIMATION_H__

typedef struct ECMaterialAnimation ECMaterialAnimation;

struct ECMaterialAnimation{
	EComponentHeader header;
	void 			*data;
};

EComponentList	ECMaterialAnimation_RequiredComponents(void);
void			ECMaterialAnimation_Setup(void *_this, Entity *_entity);
void			ECMaterialAnimation_Init(void *_this,Entity *_entity);

void			ECMaterialAnimation_StartAction(
		void *_this
		, MaterialAction *action
		, int repeat
);

void 			ECMaterialAnimation_StartTween(
	void *_this
	, MaterialComponent _material_component
	, Ease _ease
	, float _from
	, float _to
	, uint32_t _duration
	, int _repeat
);

void 			ECMaterialAnimation_Update(void *_this);
void 			ECMaterialAnimation_Destroy(void *_this);

#endif
