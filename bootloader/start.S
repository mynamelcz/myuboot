/* 关看门狗 */
/* 设置时钟 */
/* 设置CPU模式*/
/* 初始化SDRAM */
/* 重定位 从flash复制bootloader 到 链接地址*/
/* 清bss段*/
/*  执行main*/
#define WTD_CON         0x53000000
#define CLKDIVN         0x4c000014
#define MPLLCON         0x4c000004
#define S3C2440_MPLL_200MHZ     ((0x5c<<12)|(0x01<<4)|(0x02))
#define MEM_CTL_BASE    0480000000

.text
.global _start
_start:
/* 关看门狗 */
        ldr r0,= WTD_CON           /*看门狗地址   有‘=’ 的ldr 为伪汇编指令*/
        mov r1, #0
        str r1, [r0]
/* 设置时钟 */
        ldr r0, = CLKDIVN          /*时钟 CLKDIVN地址*/
        mov r1, #0x03
        str r1, [r0]
/* 设置CPU模式*/
        mrc p15, 0, ,r1, c1, c0, 0 /*读寄存器*/ 
        orr r1, r1, #0xc0000000
        mcr p15, 0, ,r1, c1, c0, 0 /*写寄存器*/
/* 设置时钟 */
        ldr r0, = MPLLCON 
        ldr r1, = S3C2440_MPLL_200MHZ     
        str r1, [r0]
/* 初始化SDRAM */
1:
        ldr	r0, = MEM_CTL_BASE    /*寄存器 基址*/
        adr r1, = sdram_config    /*数据起始地址*/
        add r3, r0, #(13*4)       /*最后一个寄存器的地址*/
        ldr r2, [r1], #4          /*取出数据到r2*/
        str r2, [r0], #4          /*写入寄存器*/
        cmp r0, r3                /*比较是否写完*/
        bne 1b                    /*不相等：1b:回跳到1标号  1f:前跳*/ 
/* 重定位 从flash复制bootloader 到 链接地址*/
        ldr sp, =0x34000000       /*初始化栈：64M 指到最高地址  栈向下增长*/
        bl  nand_init             /*初始化nandflash*/
        mov r0, #0                /*第一个参数：原地址*/
        ldr r1, =_start           /*第二个参数：目的地址*/
        ldr r2, =__bss_start      /*第三个参数: 代码长度*/
        sub r2, r2, r1            /*代码长度：祥见 .lds文件*/
        bl  copy_code_to_sdram    /*调用c 复制代码*/
/* 清bss段*/
        bl  clean_bss             
/*  执行main*/
        ldr lr, =halt             /*设置返回地址*/
        ldr pc, =main             /*跳转到 main*/
halt:
    b halt 
sdram_config:
    @ 存储控制器13个寄存器的设置值
    .long   0x22011110      @ BWSCON
    .long   0x00000700      @ BANKCON0
    .long   0x00000700      @ BANKCON1
    .long   0x00000700      @ BANKCON2
    .long   0x00000700      @ BANKCON3  
    .long   0x00000700      @ BANKCON4
    .long   0x00000700      @ BANKCON5
    .long   0x00018005      @ BANKCON6
    .long   0x00018005      @ BANKCON7
    .long   0x008C07A3      @ REFRESH
    .long   0x000000B1      @ BANKSIZE
    .long   0x00000030      @ MRSRB6
    .long   0x00000030      @ MRSRB7

