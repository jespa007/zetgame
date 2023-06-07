#ifndef __ZG_LUTS_H__
#define __ZG_LUTS_H__



#define ZG_LUTS_SIZE						65536
#define ZG_LUTS_DEGREE_360					ZG_LUTS_SIZE
#define ZG_LUTS_DEGREE_180					(DEGREE_360>>1)
#define ZG_LUTS_DEGREE_90					(DEGREE_360>>2)
#define ZG_LUTS_SIZE_MASK					(ZG_LUTS_SIZE-1)
#define ZG_INC_DA_DG 						182.0444444444444f
#define ZG_LUTS_OFFSET(s) 					((int)((unsigned)((int)(s)))&ZG_LUTS_SIZE_MASK)
#define ZG_LUTS_RAD_2_FIXED(rad) 			((int)((double)(rad*ZG_LUTS_DEGREE_180)/PI))
#define ZG_LUTS_DEGREES_2_FIXED(degree)	    (((int)((degree)*ZG_INC_DA_DG))&ZG_LUTS_SIZE_MASK)


extern float ZG_Luts_Sin[ZG_LUTS_SIZE];
extern float ZG_Luts_Cos[ZG_LUTS_SIZE];


void ZG_Luts_Init(void);



#endif