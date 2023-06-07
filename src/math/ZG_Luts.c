#include "_zg_math_.h"


float ZG_Luts_Sin[ZG_LUTS_SIZE]={0};
float ZG_Luts_Cos[ZG_LUTS_SIZE]={0};


void ZG_Luts_Init(void)
{

	float angle = 0.0f;
	float inc_angle_lut_table=360.0f/ZG_LUTS_SIZE;

	for(int i = 0; i < ZG_LUTS_SIZE; i++)
	{
		ZG_Luts_Sin[i] = (float)sinf((angle*PI)/180.0f);
		ZG_Luts_Cos[i] = (float)cosf((angle*PI)/180.0f);

		angle += inc_angle_lut_table;

	}

}



