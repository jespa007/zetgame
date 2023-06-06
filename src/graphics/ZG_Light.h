#ifndef __LIGHT_H__
#define __LIGHT_H__

typedef struct Light Light;

struct Light{
	Shape 			*shape;
};

Light * Light_New(void);
void Light_Delete(Light *_this);

#endif
