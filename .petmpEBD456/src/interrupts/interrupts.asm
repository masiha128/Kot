[bits 64]

GLOBAL Entry_PageFault_Handler, Entry_DoubleFault_Handler, Entry_GPFault_Handler, Entry_KeyboardInt_Handler, Entry_MouseInt_Handler, Entry_PITInt_Handler
EXTERN PageFault_Handler, DoubleFault_Handler, GPFault_Handler, KeyboardInt_Handler, MouseInt_Handler, PITInt_Handler

%macro    PUSH_REG    0
    push    r15
    push    r14
    push    r13
    push    r12
    push    r11
    push    r10
    push    r9
    push    r8
    push    rbp
    push    rdi
    push    rsi
    push    rsp
    push    rdx
    push    rcx
    push    rbx
    push    rax
%endmacro
%macro    POP_REG        0
    pop    rax
    pop    rbx
    pop    rcx
    pop    rdx
    pop    rsp
    pop    rsi
    pop    rdi
    pop    rbp
    pop    r8
    pop    r9
    pop    r10
    pop    r11
    pop    r12
    pop    r13
    pop    r14
    pop    r15
%endmacro


Entry_PageFault_Handler:
    PUSH_REG
    call PageFault_Handler
    POP_REG
    iretq

Entry_DoubleFault_Handler:
    PUSH_REG
    call DoubleFault_Handler
    POP_REG
    iretq

Entry_GPFault_Handler:
    PUSH_REG
    call GPFault_Handler
    POP_REG
    iretq

Entry_KeyboardInt_Handler:
    PUSH_REG
    mov rdi, rsp
    call KeyboardInt_Handler
    POP_REG
	iretq

Entry_MouseInt_Handler:
	;cli
    ;PUSH_REG
    ;call MouseInt_Handler
    ;POP_REG
	;sti
    ;ret 

Entry_PITInt_Handler:
	;cli
    ;PUSH_REG
    
    ;mov rdi, rsp
    ;call PITInt_Handler  

    ;POP_REG
	;sti
    ;ret