
#ifndef __CONFIG_H 
#define __CONFIG_H

#ifndef TRUE
#define TRUE  1
#endif

#ifndef FALSE
#define FALSE 0
#endif

typedef unsigned char  uint8;                   /* defined for unsigned 8-bits integer variable   */
typedef signed   char  int8;                    /* defined for signed 8-bits integer variable	 */
typedef unsigned short uint16;                  /* defined for unsigned 16-bits integer variable  */
typedef signed   short int16;                   /* defined for signed 16-bits integer variable 	*/
typedef unsigned int   uint32;                  /* defined for unsigned 32-bits integer variable  */
typedef signed   int   int32;                   /* defined for signed 32-bits integer variable 	 */
typedef float          fp32;                    /* single precision floating point variable (32bits) */
typedef double         fp64;                    /* double precision floating point variable (64bits) */

/********************************/
/*      uC/OS-II specital code  */
/********************************/

#define     USER_USING_MODE    0x10                    /*  User mode ,ARM 32BITS CODE         */
                                           /*  Chosen one from 0x10,0x30,0x1f,0x3f.    */
#include "Includes.h"


/********************************/
/*      ARM specital code       */
/********************************/
//This segment should not be modify

#include    "LPC214x.h"

/********************************/
/*Application Program Configurations*/
/********************************/
//This segment could be modified as needed.
#include    <stdio.h>
#include    <ctype.h>
#include    <stdlib.h>
#include    <setjmp.h>
#include    <rt_misc.h>
#include 		<stdlib.h>

/********************************/
/*Configuration of the example */
/********************************/
/* System configuration          */

#define Fosc            12000000                    //Crystal frequence,10MHz~25MHz��should be the same as actual status. 
						   
#define Fcclk           (Fosc * 5)                  //System frequence,should be (1~32)multiples of Fosc,and should be equal or less  than 60MHz. 
						   
#define Fcco            (Fcclk * 4)                 //CCO frequence,should be 2、4、8、16 multiples of Fcclk, ranged from 156MHz to 320MHz. 
						    
#define Fpclk           (Fcclk / 4) * 1             //VPB clock frequence , must be 1、2、4 multiples of (Fcclk / 4).
						   

#include    "target.h"              //This line may not be deleted 


#endif
/*********************************************************************************************************
**                            End Of File
********************************************************************************************************/
