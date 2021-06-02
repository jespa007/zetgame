#ifndef __EC_RENDERABLE_H__
#define __EC_RENDERABLE_H__

typedef struct ECRenderable ECRenderable;

struct ECRenderable{
	Entity *entity;
	Geometry *geometry;
	Appearance *appearance;
};

ECRenderable *ECRenderable_New(Entity *_entity);
//void ECRenderable_SetQuadPixels(uint16 width, uint16 height);
void ECRenderable_Update(ECRenderable *_this);
void ECRenderable_Delete(ECRenderable *_this);

#endif
