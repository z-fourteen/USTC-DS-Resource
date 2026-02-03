
;选择FSM法实现对输入数字的求模运算          
            
            .ORIG x3000
        
            AND R2, R2, #0      ;初始化R2, 存放模值X
            LD  R4, BASE        ;初始化R4， 存放BASE值#48，用于ASCLL码和数字的转化
            LD  R5, BASE_MINUS  ;初始化R5， 存放BASE值#-48，用于ASCLL码和数字的转化
            
            LEA R0, IN_MSG       ;输入提示语
            TRAP x22    
INPUT       LDI R1, KBSR        ;循环输入
            BRzp INPUT
            LDI R0, KBDR
ECHO        LDI R1, DSR         ;同步显示输入的字符
            BRzp ECHO
            STI R0, DDR
            ADD R0, R0, R5      ;将ASCLL码转为数字
            
            ADD R3, R0, #0      ;判断输入是否是0，1
            BRz MOD
            ADD R3, R0, #-1
            BRz MOD
            
            LEA R0, OUT_MSG       ;输出提示语
            TRAP x22 
            ADD R0, R2, R4      ;将结果转化为ASCLL码并输出在屏幕上
OUTPUT      LDI R1, DSR
            BRzp OUTPUT
            STI R0, DDR
            BRnzp DONE
            
MOD         ADD R3, R2, R2      ;更新模值, R3 <- 2X+0/ 2X+1
            ADD R3, R3, R0
AGAIN       ADD R3, R3, #-7     ;相减法求解模值
            BRzp    AGAIN
            ADD R2, R3, #7
            BRnzp   INPUT
            
            
DSR         .FILL xFE04
DDR         .FILL xFE06

KBSR        .FILL xFE00
KBDR        .FILL xFE02

BASE        .FILL #48
BASE_MINUS  .FILL #-48

IN_MSG      .STRINGZ "INPUT:"
OUT_MSG     .STRINGZ "OUTPUT:"

DONE        TRAP x25
    
            .END