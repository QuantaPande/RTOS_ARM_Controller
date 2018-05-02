
#define  OS_CPU_GLOBALS
#include "..\APP\config.h"

/*OSTaskStkInit	*/


        OS_STK *OSTaskStkInit (void (*task)(void *pd), void *pdata, OS_STK *ptos, INT16U opt)
{
    OS_STK *stk;

    opt    = opt;                          
    stk    = ptos;                          

                                           
    *stk = (OS_STK) task;                   /*  pc  */
    *--stk = (OS_STK) task;                 /*  lr  */

    *--stk = 0;                             /*  r12  */
    *--stk = 0;                             /*  r11  */
    *--stk = 0;                             /*  r10  */
    *--stk = 0;                             /*  r9   */
    *--stk = 0;                             /*  r8   */
    *--stk = 0;                             /*  r7   */
    *--stk = 0;                             /*  r6   */
    *--stk = 0;                             /*  r5   */
    *--stk = 0;                             /*  r4   */
    *--stk = 0;                             /*  r3   */
    *--stk = 0;                             /*  r2   */
    *--stk = 0;                             /*  r1   */
    *--stk = (unsigned int) pdata;          
    *--stk = (USER_USING_MODE|0x00);        
    *--stk = 0;                             

    return (stk);
}


/* SWI_Exception */
#if OS_SELF_EN > 0 
extern int const _OSFunctionAddr[];
extern int const _UsrFunctionAddr[];
#endif
        void SWI_Exception(int SWI_Num, int *Regs)
{
    OS_TCB   *ptcb;
    unsigned int temp;	   
						
    
    switch(SWI_Num)
    {
        //case 0x00:                    
        //    break;
        //case 0x01:                   
        //    break;
        case 0x02:                     
            __asm
            {
                MRS     temp, SPSR
                ORR     temp, temp, #NoInt
                MSR     SPSR_c, temp
            }
            OsEnterSum++;
            break;
        case 0x03:                      
            if (--OsEnterSum == 0)
            {
                __asm
                {
                    MRS     temp, SPSR
                    BIC     temp, temp, #NoInt
                    MSR     SPSR_c, temp
                }
            }
            break;
#if OS_SELF_EN > 0 
        case 0x40:
                                        
            Regs[0] =  _OSFunctionAddr[Regs[0]];
            break;
        case 0x41:
                                        
            Regs[0] =  _UsrFunctionAddr[Regs[0]];
            break;
        case 0x42:                     
            OSIntNesting++;
            break;
        case 0x43:                      
            if (OSTCBHighRdy == OSTCBCur)
            {
                Regs[0] = 0;
            }
            else
            {
                Regs[0] = 1;
            }
            break;
#endif
        case 0x80:                     
            __asm
            {
                MRS     temp, SPSR
                BIC     temp, temp, #0x1f
                ORR     temp, temp, #SYS32Mode    
                MSR     SPSR_c, temp
            }
            break;
        case 0x81:                      
            __asm
            {
                MRS     temp, SPSR
                BIC     temp, temp, #0x1f
                ORR     temp, temp, #USR32Mode    
                MSR     SPSR_c, temp
            }
            break;
        case 0x82:                      
            if (Regs[0] <= OS_LOWEST_PRIO)
            {
                ptcb = OSTCBPrioTbl[Regs[0]];
                if (ptcb != NULL)
                {
                    ptcb -> OSTCBStkPtr[1] &= ~(1 << 5);
                }
            }
            break;
        case 0x83:                      
            if (Regs[0] <= OS_LOWEST_PRIO)
            {
                ptcb = OSTCBPrioTbl[Regs[0]];
                if (ptcb != NULL)
                {
                    ptcb -> OSTCBStkPtr[1] |= (1 << 5);
                }
            }
            break;
        default:
            break;
    }
}

/* OSStartHighRdy */

        void OSStartHighRdy(void)
{
    _OSStartHighRdy();
}



#if OS_CPU_HOOKS_EN
/*
*********************************************************************************************************
*                                       OS INITIALIZATION HOOK
*                                            (BEGINNING)
*
* Description: This function is called by OSInit() at the beginning of OSInit().
*
* Arguments  : none
*
* Note(s)    : 1) Interrupts should be disabled during this call.
*********************************************************************************************************
*/
#if OS_VERSION > 203
void OSInitHookBegin (void)
{
}
#endif

/*
*********************************************************************************************************
*                                       OS INITIALIZATION HOOK
*                                               (END)
*
* Description: This function is called by OSInit() at the end of OSInit().
*
* Arguments  : none
*
* Note(s)    : 1) Interrupts should be disabled during this call.
*********************************************************************************************************
*/
#if OS_VERSION > 203
void OSInitHookEnd (void)
{
}
#endif


/*
*********************************************************************************************************
*                                          TASK CREATION HOOK
*
* Description: This function is called when a task is created.
*
* Arguments  : ptcb   is a pointer to the task control block of the task being created.
*
* Note(s)    : 1) Interrupts are disabled during this call.
*********************************************************************************************************
*/
void OSTaskCreateHook (OS_TCB *ptcb)
{
    ptcb = ptcb;                       /* Prevent compiler warning                                     */
}


/*
*********************************************************************************************************
*                                           TASK DELETION HOOK
*
* Description: This function is called when a task is deleted.
*
* Arguments  : ptcb   is a pointer to the task control block of the task being deleted.
*
* Note(s)    : 1) Interrupts are disabled during this call.
*********************************************************************************************************
*/
void OSTaskDelHook (OS_TCB *ptcb)
{
    ptcb = ptcb;                       /* Prevent compiler warning                                     */
}

/*
*********************************************************************************************************
*                                           TASK SWITCH HOOK
*
* Description: This function is called when a task switch is performed.  This allows you to perform other
*              operations during a context switch.
*
* Arguments  : none
*
* Note(s)    : 1) Interrupts are disabled during this call.
*              2) It is assumed that the global pointer 'OSTCBHighRdy' points to the TCB of the task that
*                 will be 'switched in' (i.e. the highest priority task) and, 'OSTCBCur' points to the 
*                 task being switched out (i.e. the preempted task).
*********************************************************************************************************
*/
void OSTaskSwHook (void)
{
}

/*
*********************************************************************************************************
*                                           STATISTIC TASK HOOK
*
* Description: This function is called every second by uC/OS-II's statistics task.  This allows your 
*              application to add functionality to the statistics task.
*
* Arguments  : none
*********************************************************************************************************
*/
void OSTaskStatHook (void)
{
}

/*
*********************************************************************************************************
*                                           OSTCBInit() HOOK
*
* Description: This function is called by OSTCBInit() after setting up most of the TCB.
*
* Arguments  : ptcb    is a pointer to the TCB of the task being created.
*
* Note(s)    : 1) Interrupts may or may not be ENABLED during this call.
*********************************************************************************************************
*/
#if OS_VERSION > 203
void OSTCBInitHook (OS_TCB *ptcb)
{
    ptcb = ptcb;                                           /* Prevent Compiler warning                 */
}
#endif


/*
*********************************************************************************************************
*                                               TICK HOOK
*
* Description: This function is called every tick.
*
* Arguments  : none
*
* Note(s)    : 1) Interrupts may or may not be ENABLED during this call.
*********************************************************************************************************
*/
void OSTimeTickHook (void)
{
}


/*
*********************************************************************************************************
*                                             IDLE TASK HOOK
*
* Description: This function is called by the idle task.  This hook has been added to allow you to do  
*              such things as STOP the CPU to conserve power.
*
* Arguments  : none
*
* Note(s)    : 1) Interrupts are enabled during this call.
*********************************************************************************************************
*/
#if OS_VERSION >= 251
void OSTaskIdleHook (void)
{
}
#endif
#endif
/*********************************************************************************************************
**                            End Of File
********************************************************************************************************/

