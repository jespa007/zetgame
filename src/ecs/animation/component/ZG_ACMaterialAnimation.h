#ifndef _ZG_ACMATERIAL_ANIMATION_H__
#define _ZG_ACMATERIAL_ANIMATION_H__

struct ZG_ACMaterialAnimation{
	ZG_AComponentHeader header;
	void 			*data;
};

void			ZG_ACMaterialAnimation_Setup(void *_this, ZG_ComponentId _idx);
void			ZG_ACMaterialAnimation_Init(void *_this);

void			ZG_ACMaterialAnimation_StartAction(
		void *_this
		, ZG_MaterialAction *action
		, int repeat
);

void 			ZG_ACMaterialAnimation_StartTween(
	void *_this
	, ZG_MaterialComponent _material_component
	, ZG_Ease _ease
	, float _from
	, float _to
	, uint32_t _duration
	, int _repeat
);

void 			ZG_ACMaterialAnimation_Update(void *_this);
void 			ZG_ACMaterialAnimation_Destroy(void *_this);

#endif
