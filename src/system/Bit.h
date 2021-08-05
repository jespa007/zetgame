#ifndef __BIT_H__
#define __BIT_H__

//  Operacions  de  bit.
#define  TST_BIT(x,  b)  ((((x))    &  (0x1  <<  (b)))   !=0				   		  )
#define  SET_BIT(x,  b)  ( ((x))    =  ((unsigned)(x))    |      (0x1        <<  (b))   )
#define  CLR_BIT(x,  b)  ((((x))    =  ((unsigned)(x))    &      (~    (0x1  <<  (b)))) )
#define  XOR_BIT(x,  b)  (  (x)     =  ((unsigned)(x)     ^      (0x1        <<  (b)))  )


#define  TST_MSK(x,  msk)  ((((x))    &  ((msk)))   	  !=	  0	   )
#define  SET_MSK(x,  msk)  ( ((x))    =  ((unsigned)(x))    |      (msk) )
#define  CLR_MSK(x,  msk)  (  (x)     =  ((unsigned)(x))    &     ~(msk) )
#define  XOR_MSK(x,  msk)  (  (x)     =  ((unsigned)(x)     ^      (msk)))

unsigned int Bit_NextPow2(unsigned int v);

#endif

