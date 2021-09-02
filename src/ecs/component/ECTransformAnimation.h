#ifndef _EC_TRANSFORM_ANIMATION_H__
#define _EC_TRANSFORM_ANIMATION_H__

typedef struct ECTransformAnimation ECTransformAnimation;

struct ECTransformAnimation{
	EComponentHeader header;
	void 			*data;
};

EComponentList 	ECTransformAnimation_RequiredComponents(void);
void			ECTransformAnimation_Setup(void *_this, Entity *_entity);
void			ECTransformAnimation_Init(void *_this,Entity *_entity);
void 			ECTransformAnimation_StartTween(
					void *_this
					, uint8_t _idx_channel
					, Ease _ease
					, float _from
					, float _to
					, uint32_t _duration
					, int _repeat);

void 			ECTransformAnimation_Update(void *_this);
void 			ECTransformAnimation_Destroy(void *_this);

#endif
