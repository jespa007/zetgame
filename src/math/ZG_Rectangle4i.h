#ifndef __RECTANGLE4I_H__
#define __RECTANGLE4I_H__

typedef struct Rectangle4i Rectangle4i;

struct Rectangle4i{
	int x1,y1,x2,y2;
};

Rectangle4i  Rectangle4i_New(void);
Rectangle4i  Rectangle4i_New4i(int x1,int y1,int x2,int y2);


#endif
