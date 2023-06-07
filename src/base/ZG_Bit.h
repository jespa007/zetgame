#ifndef __ZG_BIT_H__
#define __ZG_BIT_H__

//  Operacions  de  bit.
#define  ZG_TST_BIT(x,  b)  ((((x))    &  (0x1  <<  (b)))   !=0				   		  )
#define  ZG_SET_BIT(x,  b)  ( ((x))    =  ((unsigned)(x))    |      (0x1        <<  (b))   )
#define  ZG_CLR_BIT(x,  b)  ((((x))    =  ((unsigned)(x))    &      (~    (0x1  <<  (b)))) )
#define  ZG_XOR_BIT(x,  b)  (  (x)     =  ((unsigned)(x)     ^      (0x1        <<  (b)))  )


#define  ZG_ST_MSK(x,  msk)  ((((x))    &  ((msk)))   	  !=	  0	   )
#define  ZG_SET_MSK(x,  msk)  ( ((x))    =  ((unsigned)(x))    |      (msk) )
#define  ZG_CLR_MSK(x,  msk)  (  (x)     =  ((unsigned)(x))    &     ~(msk) )
#define  ZG_XOR_MSK(x,  msk)  (  (x)     =  ((unsigned)(x)     ^      (msk)))

unsigned int ZG_Bit_NextPow2(unsigned int v);

#endif

