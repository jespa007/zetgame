#ifndef _AC_MATERIAL_ANIMATION_H__
#define _AC_MATERIAL_ANIMATION_H__

typedef struct ACMaterialAnimation ACMaterialAnimation;

struct ACMaterialAnimation{
	AComponentHeader header;
	void 			*data;
};

AComponentList	ACMaterialAnimation_RequiredComponents(void);
void			ACMaterialAnimation_Setup(void *_this, Entity *_entity);
void			ACMaterialAnimation_Init(void *_this,Entity *_entity);

void			ACMaterialAnimation_StartAction(
		void *_this
		, MaterialAction *action
		, int repeat
);

void 			ACMaterialAnimation_StartTween(
	void *_this
	, MaterialComponent _material_component
	, Ease _ease
	, float _from
	, float _to
	, uint32_t _duration
	, int _repeat
);

void 			ACMaterialAnimation_Update(void *_this);
void 			ACMaterialAnimation_Destroy(void *_this);

#endif
