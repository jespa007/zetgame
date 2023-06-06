#ifndef __ZG_MATERIAL_ANIMATION_H__
#define __ZG_MATERIAL_ANIMATION_H__


typedef struct ZG_MaterialAnimation ZG_MaterialAnimation;

struct ZG_MaterialAnimation{

	void *data;
};

// STATIC
Action 				*	ZG_MaterialAnimation_NewAction(void);


// PUBLIC
ZG_MaterialAnimation 	*	ZG_MaterialAnimation_New(void);
void					ZG_MaterialAnimation_StartAction(
		ZG_MaterialAnimation *_this
		, MaterialAction *_action
		,uint32_t _start_time
		, int _repeat
);
void					ZG_MaterialAnimation_StartTween(
	ZG_MaterialAnimation *_this
	, uint32_t _start_time
	, MaterialComponent _material_component
	, Ease _ease
	, float _from
	, float _to
	, uint32_t _duration
	, int _repeat
);
void 					ZG_MaterialAnimation_Update(ZG_MaterialAnimation *_this, ZG_Material *_transform);
void 					ZG_MaterialAnimation_Delete(ZG_MaterialAnimation *_this);

#endif
