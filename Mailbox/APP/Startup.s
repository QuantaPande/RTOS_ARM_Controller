
FIQ_STACK_LEGTH         EQU         0
IRQ_STACK_LEGTH         EQU         9*8             
ABT_STACK_LEGTH         EQU         0
UND_STACK_LEGTH         EQU         0

NoInt       EQU 0x80

USR32Mode   EQU 0x10
SVC32Mode   EQU 0x13
SYS32Mode   EQU 0x1f
IRQ32Mode   EQU 0x12
FIQ32Mode   EQU 0x11

PINSEL2     EQU 0xE002C014



    IMPORT  FIQ_Exception                   
    IMPORT  __main                          
    IMPORT  TargetResetInit                 

    IMPORT  SoftwareInterrupt


    EXPORT  Reset
    EXPORT  __rt_div0
    EXPORT  __user_initial_stackheap
	EXPORT bottom_of_heap
	EXPORT	StackUsr
    
	CODE32

    PRESERVE8	 

    AREA    vectors,CODE,READONLY

    ARM

Reset
        LDR     PC, ResetAddr
        LDR     PC, UndefinedAddr
        LDR     PC, SWI_Addr
        LDR     PC, PrefetchAddr
        LDR     PC, DataAbortAddr
        DCD     0xb9205f80
        LDR     PC, [PC, #-0xff0]
        LDR     PC, FIQ_Addr

ResetAddr           DCD     ResetInit
UndefinedAddr       DCD     Undefined
SWI_Addr            DCD     SoftwareInterrupt
PrefetchAddr        DCD     PrefetchAbort
DataAbortAddr       DCD     DataAbort
Nouse               DCD     0
IRQ_Addr            DCD     0
FIQ_Addr            DCD     FIQ_Handler


Undefined
        B       Undefined


PrefetchAbort
        B       PrefetchAbort


DataAbort
        B       DataAbort


FIQ_Handler
        STMFD   SP!, {R0-R3, LR}
        BL      FIQ_Exception
        LDMFD   SP!, {R0-R3, LR}
        SUBS    PC,  LR,  #4



	IF :DEF: EN_CRP
        IF  . >= 0x1fc
        INFO    1,"\nThe data at 0x000001fc must be 0x87654321.\nPlease delete some source before this line."
        ENDIF
CrpData
    WHILE . < 0x1fc
    NOP
    WEND
CrpData1
    DCD     0x87654321          ;/*When the Data is 0x87654321,user code be protected.  */
    ENDIF



InitStack    
        MOV     R0, LR


        MSR     CPSR_c, #0xd2
        LDR     SP, StackIrq

        MSR     CPSR_c, #0xd1
        LDR     SP, StackFiq

        MSR     CPSR_c, #0xd7
        LDR     SP, StackAbt

        MSR     CPSR_c, #0xdb
        LDR     SP, StackUnd

        MSR     CPSR_c, #0xdf
        LDR     SP, =StackUsr

        MOV     LR, R0
        MOV     PC, LR
		

ResetInit       
        BL      InitStack               
        BL      TargetResetInit        
                                        
        B       __main          



__user_initial_stackheap    
    LDR   R0,=bottom_of_heap
    MOV   PC,LR



__rt_div0

        B       __rt_div0

StackIrq           DCD     IrqStackSpace + (IRQ_STACK_LEGTH - 1)* 4
StackFiq           DCD     FiqStackSpace + (FIQ_STACK_LEGTH - 1)* 4
StackAbt           DCD     AbtStackSpace + (ABT_STACK_LEGTH - 1)* 4
StackUnd           DCD     UndtStackSpace + (UND_STACK_LEGTH - 1)* 4


        AREA    MyStacks, DATA, NOINIT, ALIGN=2
IrqStackSpace           SPACE   IRQ_STACK_LEGTH * 4  
FiqStackSpace           SPACE   FIQ_STACK_LEGTH * 4  
AbtStackSpace           SPACE   ABT_STACK_LEGTH * 4  
UndtStackSpace          SPACE   UND_STACK_LEGTH * 4  




        AREA    Heap, DATA, NOINIT
bottom_of_heap    SPACE   1

        AREA    Stacks, DATA, NOINIT
StackUsr		  SPACE   1

    END
;/*********************************************************************************************************
;**                            End Of File
;********************************************************************************************************/
