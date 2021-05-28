#include "zg_math.h"


float Luts_Sin[LUTS_SIZE]={0};
float Luts_Cos[LUTS_SIZE]={0};


void Luts_Init(void)
{

	float angle = 0.0f;
	float inc_angle_lut_table=360.0f/LUTS_SIZE;

	for(int i = 0; i < LUTS_SIZE; i++)
	{
		Luts_Sin[i] = (float)sinf((angle*PI)/180.0f);
		Luts_Cos[i] = (float)cosf((angle*PI)/180.0f);

		angle += inc_angle_lut_table;

	}

}



