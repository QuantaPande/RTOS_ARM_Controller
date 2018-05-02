
SVC_STACK_LEGTH     EQU         32

NoInt       EQU 0x80

USR32Mode   EQU 0x10
SVC32Mode   EQU 0x13
SYS32Mode   EQU 0x1f
IRQ32Mode   EQU 0x12
FIQ32Mode   EQU 0x11


T_bit               EQU         0x20

    CODE32
    PRESERVE8
    AREA    |subr|, CODE, READONLY

            IMPORT  OSTCBCur                    
            IMPORT  OSTCBHighRdy                
            IMPORT  OSPrioCur                   
            IMPORT  OSPrioHighRdy              
            IMPORT  OSTaskSwHook               
            IMPORT  OSRunning                   

            IMPORT  OsEnterSum                  
            IMPORT  SWI_Exception               
            
            EXPORT  __OSStartHighRdy            
            EXPORT  OSIntCtxSw                  
            EXPORT  SoftwareInterrupt           


;/*SoftwareInterrupt*/

SoftwareInterrupt
        LDR     SP, StackSvc            
        STMFD   SP!, {R0-R3, R12, LR}
        MOV     R1, SP                  

        MRS     R3, SPSR
        TST     R3, #T_bit              
        LDRNEH  R0, [LR,#-2]            
        BICNE   R0, R0, #0xff00
        LDREQ   R0, [LR,#-4]            
        BICEQ   R0, R0, #0xFF000000
                                        
        CMP     R0, #1
        LDRLO   PC, =OSIntCtxSw
        LDREQ   PC, =__OSStartHighRdy   

        BL      SWI_Exception
        
        LDMFD   SP!, {R0-R3, R12, PC}^
        
StackSvc           DCD     (SvcStackSpace + SVC_STACK_LEGTH * 4 - 4)

;/* OSIntCtxSw */
OSIntCtxSw
                                                    
        LDR     R2, [SP, #20]                      
        LDR     R12, [SP, #16]                      
        MRS     R0, CPSR

        MSR     CPSR_c, #(NoInt | SYS32Mode)
        MOV     R1, LR
        STMFD   SP!, {R1-R2}                        
        STMFD   SP!, {R4-R12}                       

        MSR     CPSR_c, R0
        LDMFD   SP!, {R4-R7}                       
        ADD     SP, SP, #8                          
        
        MSR     CPSR_c, #(NoInt | SYS32Mode)
        STMFD   SP!, {R4-R7}                        
        
        LDR     R1, =OsEnterSum                     
        LDR     R2, [R1]
        STMFD   SP!, {R2, R3}                       

                                                    
        LDR     R1, =OSTCBCur
        LDR     R1, [R1]
        STR     SP, [R1]

        BL      OSTaskSwHook                        
                                                    
        LDR     R4, =OSPrioCur
        LDR     R5, =OSPrioHighRdy
        LDRB    R6, [R5]
        STRB    R6, [R4]
                                                    
        LDR     R6, =OSTCBHighRdy
        LDR     R6, [R6]
        LDR     R4, =OSTCBCur
        STR     R6, [R4]
OSIntCtxSw_1
                                                    
        LDR     R4, [R6]
        ADD     SP, R4, #68                         
        LDR     LR, [SP, #-8]
        MSR     CPSR_c, #(NoInt | SVC32Mode)        
        MOV     SP, R4                              

        LDMFD   SP!, {R4, R5}                       
                                                   
        LDR     R3, =OsEnterSum
        STR     R4, [R3]
    
        MSR     SPSR_cxsf, R5                       
        LDMFD   SP!, {R0-R12, LR, PC }^             

;/* OSStartHighRdy */

__OSStartHighRdy
        MSR     CPSR_c, #(NoInt | SYS32Mode)
                                                
        LDR     R4, =OSRunning
        MOV     R5, #1
        STRB    R5, [R4]

        BL      OSTaskSwHook                    

        LDR     R6, =OSTCBHighRdy
        LDR     R6, [R6]
        B       OSIntCtxSw_1

        AREA    SWIStacks, DATA, NOINIT,ALIGN=2
SvcStackSpace      SPACE   SVC_STACK_LEGTH * 4  

    END
;/*********************************************************************************************************
;**                            End Of File
;********************************************************************************************************/
