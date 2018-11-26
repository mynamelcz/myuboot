/* 关看门狗 */
/* 设置时钟 */
/* 设置CPU模式*/
/* 初始化SDRAM */
/* 重定位 从flash复制bootloader 到 链接地址*/
/* 清bss段*/
/*  执行main*/
改进：
1.开机启动比较慢
    a.提高时钟频率，200MHz==》400MHz (没有什么效果)
        设置时钟分频:CLKDIVN
        设置 MPLLCON
    b.启动ICACHE (icache)            (速度明显提高)
        参考UBOOT设置
        void icache_enable (void)


