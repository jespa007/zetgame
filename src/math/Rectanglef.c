
#include "zg_math.h"

Rectanglef  Rectanglef_New4f(float x1,float y1,float x2,float y2){
	return (Rectanglef){
		 .x1=x1
		,.y1=y1
		,.x2=x2
		,.y2=y2
	};
}
