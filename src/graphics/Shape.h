#ifndef __SHAPE_H__
#define __SHAPE_H__

typedef struct Shape Shape;


	//SHAPE_TRANSFORM3D=0x1<<0,
#define	SHAPE_GEOMETRY		(0x1<<1)
#define	SHAPE_APPEARANCE	(0x1<<2)


struct Shape{
	void *data;
	Geometry	*geometry;
	Appearance  *appearance;
};

Shape * Shape_New(void);
void 	Shape_SetAppearance(Shape *_this,Appearance *appearance);
void 	Shape_Draw(Shape *_this, Transform *transform);
void 	Shape_Delete(Shape *shape);
#endif
