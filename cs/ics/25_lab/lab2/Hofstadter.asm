            .ORIG   x3000
;数据初始化
            LDI     R0, NUMBER  
            LEA     R6, QAD     ; R6 = Q array base address
;特殊情况：N <=2 直接存储
            ADD     R1, R0, #-2
            BRp     INIT
            AND     R4, R4, #0
            ADD     R4, R4, #1
            STI     R4, RESULT
            HALT
            
;循环前：基本数据赋值
INIT        AND     R3, R3, #0
            ADD     R3, R3, #1  ;R3 = Q[1]
            STR     R3, R6, #1  ;Q[1] <- 1
            
            ADD     R4, R3, #0  ;R4 = Q[2]
            STR     R4, R6, #2  ;Q[2] <- 1
            
            AND     R5, R5, #0
            ADD     R5, R5, #3  ;initiate counter
            
            
LOOP        NOT     R1, R3
            ADD     R1, R1, #1
            ADD     R1, R1, R5  ;R1 <- n − Q(n − 2)
            ADD     R1, R1, R6  ;转换为地址
            LDR     R1, R1, #0  ;R1 <- Q[n − Q(n − 2)]
            
            NOT     R2, R4
            ADD     R2, R2, #1
            ADD     R2, R2, R5  ;R2 <- n − Q(n − 1)
            ADD     R2, R2, R6  ;转换为地址
            LDR     R2, R2, #0  ;R2 <- Q[n − Q(n − 1)]
            
            ADD     R7, R1, R2
            ADD     R1, R5, R6  
            STR     R7, R1, #0  ;Q(n) = Q(n − Q(n − 1)) + Q(n − Q(n − 2))
            
            ADD     R3, R4, #0  ;update Q(n − 2)
            ADD     R4, R7, #0  ;update Q(n − 1)
            ADD     R5, R5, #1; ;counter increment
            
;循环结束条件判断     
            NOT     R7, R5
            ADD     R7, R7, #1
            ADD     R7, R7, R0  ;R7 = number - counter
            BRzp    LOOP     
            
;结果存储            
            STI      R4, RESULT
            HALT
            
            
NUMBER      .FILL   x3100
RESULT      .FILL   x3101
QAD         .BLKW   100

            .END
    

