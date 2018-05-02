
#ifdef  OS_CPU_GLOBALS
#define OS_CPU_EXT
#else
#define OS_CPU_EXT  extern
#endif

/**********************************************************************************************************
*                   
**********************************************************************************************************/

typedef unsigned char  BOOLEAN;                 /*                                */
typedef unsigned char  INT8U;                   /*                       */
typedef signed   char  INT8S;                   /*                        */
typedef unsigned short INT16U;                  /*                      */
typedef signed   short INT16S;                  /*                       */
typedef unsigned int   INT32U;                  /*                       */
typedef signed   int   INT32S;                  /*                       */
typedef float          FP32;                    /*                */
typedef double         FP64;                    /*                */

typedef INT32U         OS_STK;                  /*                        */

/*  */
#define BYTE           INT8S
#define UBYTE          INT8U
#define WORD           INT16S
#define UWORD          INT16U
#define LONG           INT32S
#define ULONG          INT32U

/* *********************************************************************************************************
*                
**********************************************************************************************************/
#define     OS_CRITICAL_METHOD     2            /*  */


__swi(0x00) void OS_TASK_SW(void);              /*         */
__swi(0x01) void _OSStartHighRdy(void);         /*         */
__swi(0x02) void OS_ENTER_CRITICAL(void);       /*                     */
__swi(0x03) void OS_EXIT_CRITICAL(void);        /*                   */

__swi(0x40) void *GetOSFunctionAddr(int Index); /*        */
__swi(0x41) void *GetUsrFunctionAddr(int Index);/*     */
__swi(0x42) void OSISRBegin(void);              /*            */
__swi(0x43) int  OSISRNeedSwap(void);           /*        */

__swi(0x80) void ChangeToSYSMode(void);         /*        */
__swi(0x81) void ChangeToUSRMode(void);         /*         */
__swi(0x82) void TaskIsARM(INT8U prio);         /*           */
__swi(0x83) void TaskIsTHUMB(INT8U prio);       /*         */

#define OS_STK_GROWTH    1                      /*           */

#define     USR32Mode       0x10                /*                */
#define     SYS32Mode       0x1f                /*                    */
#define     NoInt           0x80

#ifndef USER_USING_MODE
#define USER_USING_MODE USR32Mode               /*                */
#endif

#ifndef OS_SELF_EN
#define OS_SELF_EN     0                        /*  */    
#endif


OS_CPU_EXT INT32U OsEnterSum;                   /*  */


/*********************************************************************************************************
**                            End Of File
********************************************************************************************************/
