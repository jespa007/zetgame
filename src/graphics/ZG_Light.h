#ifndef __ZG_LIGHT_H__
#define __ZG_LIGHT_H__

typedef struct ZG_Light ZG_Light;

struct ZG_Light{
	ZG_Shape 			*shape;
};

ZG_Light * ZG_Light_New(void);
void ZG_Light_Delete(ZG_Light *_this);

#endif
