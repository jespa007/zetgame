#ifndef _ZG_ACMATERIAL_ANIMATION_H__
#define _ZG_ACMATERIAL_ANIMATION_H__

struct ZG_ACMaterialAnimation{
	ZG_AComponentHeader header;
	void 			*data;
};


//----
void			ZG_ACMaterialAnimation_Setup(ZG_AComponentHeader *_this, ZG_ComponentId _id);
void 			ZG_ACMaterialAnimation_Update(ZG_AComponentHeader *_this);
void 			ZG_ACMaterialAnimation_Destroy(ZG_AComponentHeader *_this);
//----


void			ZG_ACMaterialAnimation_StartAction(
	ZG_ACMaterialAnimation *_this
	, ZG_MaterialAction *action
	, int repeat
);

void 			ZG_ACMaterialAnimation_StartTween(
	ZG_ACMaterialAnimation *_this
	, ZG_MaterialComponent _material_component
	, ZG_Ease _ease
	, float _from
	, float _to
	, uint32_t _duration
	, int _repeat
);

void	ZG_ACMaterialAnimation_UpdateECMaterial(
	ZG_ACMaterialAnimation *_this
	, ZG_ECMaterial *_material
);

#endif
