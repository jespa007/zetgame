
#include "@zg_math.h"

ZG_Rectangle4f  ZG_Rectangle4f_New4f(float x1,float y1,float x2,float y2){
	return (ZG_Rectangle4f){
		 .x1=x1
		,.y1=y1
		,.x2=x2
		,.y2=y2
	};
}
