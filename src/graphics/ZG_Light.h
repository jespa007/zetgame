#ifndef __LIGHT_H__
#define __LIGHT_H__

typedef struct ZG_Light ZG_Light;

struct ZG_Light{
	Shape 			*shape;
};

ZG_Light * ZG_Light_New(void);
void ZG_Light_Delete(ZG_Light *_this);

#endif
