#ifndef __ZG_RECTANGLE4F_H__
#define __ZG_RECTANGLE4F_H__

typedef struct ZG_Rectangle4f ZG_Rectangle4f;

struct ZG_Rectangle4f{
	float x1,y1,x2,y2;
};

ZG_Rectangle4f  ZG_Rectangle4f_New4f(float x1,float y1,float x2,float y2);


#endif
