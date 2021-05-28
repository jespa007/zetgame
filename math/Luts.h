#pragma once


#define LUTS_SIZE						65536
#define LUTS_DEGREE_360					LUTS_SIZE
#define LUTS_DEGREE_180					(DEGREE_360>>1)
#define LUTS_DEGREE_90					(DEGREE_360>>2)
#define LUTS_SIZE_MASK					(LUTS_SIZE-1)
#define INC_DA_DG 						182.0444444444444f
#define LUTS_OFFSET(s) 					((int)((unsigned)((int)(s)))&LUTS_SIZE_MASK)
#define LUTS_RAD_2_FIXED(rad) 			((int)((double)(rad*LUTS_DEGREE_180)/PI))
#define LUTS_DEGREES_2_FIXED(degree)	(((int)((degree)*INC_DA_DG))&LUTS_SIZE_MASK)


extern float Luts_Sin[LUTS_SIZE];
extern float Luts_Cos[LUTS_SIZE];


void Luts_Init(void);



