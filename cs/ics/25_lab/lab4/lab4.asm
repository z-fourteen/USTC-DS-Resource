.ORIG x3000

;---------****** 数据初始化 ******--------------
        LDI R0 , INPUT_N    ; Load N into R0
        LDI R1 , INPUT_M    ; Load M into R1
        LD  R7 , STACK      ; 初始化R7，栈指针

;-------****** 计算Routes(N,M) ******-------------- 

        AND R3 , R3, #0     ; 初始化R3为0，存储Routes(N,M)结果
        
;---------当前节点访问---------

;特殊情况判断：
REC     ADD R0 , R0, #0     ;判断R0, R1是否为0
        BRz RES_INC
        ADD R1 , R1, #0
        BRz RES_INC
        
; 若不为叶子节点，构造入栈数据
; 数据域设计：[6]表示flag,[5:3]表示N，[2:0]表示M

        AND R5 , R5, #0     ;初始化R5
        ADD R5 , R5, #3     ; R5存储N左移位数
        ADD R2 , R0, #0
SHIFT   ADD R2 , R2, R2
        ADD R5 , R5, #-1
        BRp SHIFT
        
        AND R6 , R6, #0     ; 初始化R6为0，存储入栈数据
        ADD R6 , R1, R2     ; R6 = Flag + N(左移三位） + M,其中Flag初始为0
        ADD R7 , R7, #-1    ; 栈指针更新
        STR R6 , R7, #0     ; 新数据入栈
        BRnzp UPDATE
        
;若为叶子节点，节点数目增加，处理栈顶元素

RES_INC ADD R3 , R3, #1     ; 叶子节点计数加一

JUDGE   NOT R5 , R7         ; 判断栈是否为空
        ADD R5 , R5, #1
        LD  R2 , STACK
        ADD R5 , R5, R2
        BRz CAL             ; 栈空则计算结束，得到最终答案
        LDR R5 , R7, #0     ; 栈不空则处理栈顶元素
        LD  R4 , FLAG_MASK  ; 获取flag_mask
        AND R2 , R5, R4     ; 获取flag
        BRnp POP            ; 左右子树都遍历结束，栈顶元素弹出 
        ADD R5 , R5, R4     ; 否则更新栈顶元素的flag,标志着左子树遍历结束
        STR R5 , R7, #0
        BRnzp UPDATE
POP     ADD R7 , R7, #1
        BRnzp JUDGE
        

;--------- 访问节点更新 ---------
UPDATE  LDR R5 , R7, #0     ; 处理栈顶元素
    
        LD  R1 , M_MASK     ; 获取M
        AND R1 , R1, R5
    
        LD  R0 , N_MASK     ; 获取N左移三位之后的数字 
        AND R0 , R0, R5
           
        AND R2, R2, #0      ; 右移三位，获取N 
DIV8    
        ADD R0, R0, #-8
        BRn DIV8_END
        ADD R2, R2, #1
        BRnzp DIV8
DIV8_END 
        ADD R0, R2, #0      ; R0 = N
    
        LD  R4 , FLAG_MASK  
        AND R4 , R5, R4     ; 获取flag
        BRz LEFT
        ADD R1 , R1, #-1
        BRnzp REC
LEFT    ADD R0 , R0, #-1        
        BRnzp REC

        
;----------****** 计算最终结果 ******-----------     
CAL     LDI R0 , INPUT_N    ; Load N into R0
        LDI R1 , INPUT_M    ; Load M into R1
        ADD R2 , R0, R1     ; 计算Steps(N,M),存入R2
        AND R5 , R5, #0     ; 计算Routes(n,m)*5，存入R5
        AND R4 , R4, #0
        ADD R4 , R4, #5
AGAIN   ADD R5 , R5, R3
        ADD R4 , R4, #-1
        BRp AGAIN
        NOT R2 , R2        
        ADD R2 , R2, #1
        ADD R3 , R2, R5     ; 计算最终结果Routes(NMm)*5-Steps(N,M)
        STI R3 , RESULT     ;存储计算结果
        
        TRAP x25 ; Halt program
        
FLAG_MASK   .FILL x0040
M_MASK      .FILL x0007
N_MASK      .FILL x0038

STACK    .FILL x6000 ; Starting address of the stack
INPUT_N  .FILL x3100 ; Address for input N
INPUT_M  .FILL x3101 ; Address for input M
RESULT   .FILL x3200 ; Address to store the result

.END
