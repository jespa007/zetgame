#ifndef __MATERIAL_ANIMATION_H__
#define __MATERIAL_ANIMATION_H__


typedef struct MaterialAnimation MaterialAnimation;

struct MaterialAnimation{

	void *data;
};

// STATIC
Action 				*	MaterialAnimation_NewAction(void);


// PUBLIC
MaterialAnimation 	*	MaterialAnimation_New(void);
void					MaterialAnimation_StartAction(
		MaterialAnimation *_this
		, MaterialAction *_action
		,uint32_t _start_time
		, int _repeat
);
void					MaterialAnimation_StartTween(
	MaterialAnimation *_this
	, uint32_t _start_time
	, MaterialComponent _material_component
	, Ease _ease
	, float _from
	, float _to
	, uint32_t _duration
	, int _repeat
);
void 					MaterialAnimation_Update(MaterialAnimation *_this, Material *_transform);
void 					MaterialAnimation_Delete(MaterialAnimation *_this);

#endif
