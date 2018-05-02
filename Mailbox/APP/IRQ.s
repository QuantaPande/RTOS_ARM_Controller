
NoInt       EQU 0x80

USR32Mode   EQU 0x10
SVC32Mode   EQU 0x13
SYS32Mode   EQU 0x1f
IRQ32Mode   EQU 0x12
FIQ32Mode   EQU 0x11



        IMPORT  OSIntCtxSw                     
        IMPORT  OSIntExit                       
        IMPORT  OSTCBCur
        IMPORT  OSTCBHighRdy
        IMPORT  OSIntNesting                    
        IMPORT  StackUsr
        IMPORT  OsEnterSum

    CODE32

    PRESERVE8
    AREA    IRQ,CODE,READONLY

    MACRO
$IRQ_Label HANDLER $IRQ_Exception_Function

        EXPORT  $IRQ_Label                      
        IMPORT  $IRQ_Exception_Function         

$IRQ_Label
        SUB     LR, LR, #4                      
        STMFD   SP!, {R0-R3, R12, LR}           
        MRS     R3, SPSR                        
        STMFD   SP, {R3, SP, LR}^               
                                                
        LDR     R2,  =OSIntNesting              ; OSIntNesting++
        LDRB    R1, [R2]
        ADD     R1, R1, #1
        STRB    R1, [R2]

        SUB     SP, SP, #4*3
        
        MSR     CPSR_c, #(NoInt | SYS32Mode)    
        CMP     R1, #1
        LDREQ   SP, =StackUsr
        
        BL      $IRQ_Exception_Function         
        
;       MSR     CPSR_c, #(NoInt | SYS32Mode)    
        LDR     R2, =OsEnterSum                 
        MOV     R1, #1
        STR     R1, [R2]

        BL      OSIntExit

        LDR     R2, =OsEnterSum                 
        MOV     R1, #0
        STR     R1, [R2]

        MSR     CPSR_c, #(NoInt | IRQ32Mode)    
        LDMFD   SP, {R3, SP, LR}^               
                                                
        LDR     R0, =OSTCBHighRdy
        LDR     R0, [R0]
        LDR     R1, =OSTCBCur
        LDR     R1, [R1]
        CMP     R0, R1

        ADD     SP, SP, #4*3                    ; 
        MSR     SPSR_cxsf, R3
        LDMEQFD SP!, {R0-R3, R12, PC}^         
        LDR     PC, =OSIntCtxSw                 
    MEND




IRQ_Handler HANDLER IRQ_Exception



Timer0_Handler  HANDLER Timer0_Exception

    END
;/*********************************************************************************************************
;**                            End Of File
;********************************************************************************************************/
