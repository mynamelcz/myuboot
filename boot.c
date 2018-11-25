#include "setup.h" 
/*1. 从 NAMD FLASH 读内核到内存*/
    //保证flash内已经烧入内核, 可以用uboot先烧写好
    //uImage = 64字节的头 + zImage
    //uImage存放位置：可以用mtd命令查看。0x60000
/*2. 设置参数*/
/*3. 跳转执行*/
static struct tag *params;

void setup_start_tag(void)
{
    params = (struct tag *) (0x30000100);    /*参数起始位置*/    
    params
 
}


void main(void)
{
    void (*theKernel)(int zero, int arch, uint params);
    /*1. 从 NAMD FLASH 读内核到内存*/
    nand_read(0x60000+64, 0x30008000, 0x200000);      /*先保证flash内已经烧录内核*/ 
    /*2. 设置参数*/
    setup_start_tag();
    setup_memory_tag();
    setup_commandline_tag();
    setup_end_tag();
    /*3. 跳转执行*/
    theKernel = (void(*)(int, int, unit))0x30008000;  /*内核位置 开始启动*/
    theKernel();
}
