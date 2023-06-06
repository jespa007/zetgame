#ifndef __ZG_SHAPE_H__
#define __ZG_SHAPE_H__

typedef struct ZG_Shape ZG_Shape;


	//SHAPE_TRANSFORM3D=0x1<<0,
#define	ZG_SHAPE_GEOMETRY		(0x1<<1)
#define	ZG_SHAPE_APPEARANCE	(0x1<<2)


struct ZG_Shape{
	ZG_Geometry		*geometry;
	ZG_Appearance  	*appearance;
	void 			*data;
};

ZG_Shape * 	ZG_Shape_New(void);
void 		ZG_Shape_SetAppearance(ZG_Shape *_this,ZG_Appearance *appearance);
void 		ZG_Shape_Draw(ZG_Shape *_this, ZG_Transform *transform);
void 		ZG_Shape_Delete(ZG_Shape *shape);
#endif
