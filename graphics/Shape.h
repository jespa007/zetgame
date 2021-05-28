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
void 	Shape_SetAppearance(Shape *shape,Appearance *appearance);
//void 	Shape_SetTranslate3f(Shape *shape,float x, float y, float z);
//void 	Shape_SetRotate3f(Shape *shape,float x, float y, float z);
void 	Shape_Draw(Shape *shape, Transform *transform);
void	Shape_DrawRectangle(float width, float height, bool text_coords);
void 	Shape_Delete(Shape *shape);
#endif
