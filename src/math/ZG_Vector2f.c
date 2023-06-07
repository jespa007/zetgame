#include "_zg_math_.h"

ZG_Vector2f ZG_Vector2f_New(float x, float y){
	ZG_Vector2f v={.x=x,.y=y};
	return v;
}

ZG_Vector2f ZG_Vector2f_Zeros(void){
	ZG_Vector2f v={.x=0,.y=0};
	return v;
}

float ZG_Vector2f_Distance(ZG_Vector2f *p1, ZG_Vector2f *p2){

	float ox=fabs(p2->x - p1->x);
	float oy=fabs(p2->y - p1->y);

	return sqrt(ox*ox + oy*oy);
}

void  		ZG_Vector2f_LeastSquaresFitLinear(ZG_List *lst_points2d,float * gain, float *offset){
	//Gives best fit of data to line Y = MC + B
	float x1, y1, xy, x2, J;
	int n_points = lst_points2d->count;
	int i;

	x1 = 0.0f;
	y1 = 0.0f;
	xy = 0.0f;
	x2 = 0.0f;
	*gain = 0;
	*offset = 0;

	for (i = 0; i < n_points; i++)
	{
		ZG_Vector2f *p=lst_points2d->items[i];
		x1 = x1 + p->x;
		y1 = y1 + p->y;
		xy = xy + p->x * p->y;
		x2 = x2 + p->x * p->x;
	}

	J = ((float)n_points * x2) - (x1 * x1);
	if (J != 0.0)
	{
		*gain = (((float)n_points * xy) - (x1 * y1)) / J;
		*gain = floor(1.0E3 * (*gain) + 0.5f) / 1.0E3f;
		*offset = ((y1 * x2) - (x1 * xy)) / J;
		*offset = floor(1.0E3f * (*offset) + 0.5f) / 1.0E3f;
	}

}
