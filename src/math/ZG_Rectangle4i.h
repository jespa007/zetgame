#ifndef __ZG_RECTANGLE4I_H__
#define __ZG_RECTANGLE4I_H__

typedef struct ZG_Rectangle4i ZG_Rectangle4i;

struct ZG_Rectangle4i{
	int x1,y1,x2,y2;
};

ZG_Rectangle4i  ZG_Rectangle4i_New(void);
ZG_Rectangle4i  ZG_Rectangle4i_New4i(int x1,int y1,int x2,int y2);


#endif
