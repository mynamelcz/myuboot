#include "setup.h" 
/*1. 从 NAMD FLASH 读内核到内存*/
    //保证flash内已经烧入内核, 可以用uboot先烧写好
    //uImage = 64字节的头 + zImage
    //uImage存放位置：可以用mtd命令查看。0x60000
/*2. 设置参数*/
/*3. 跳转执行*/
static struct tag *params;

extern void puts(char *str);
extern void uart0_init(void);
extern void nand_read(unsigned int addr, unsigned char *buf, unsigned int len);


void setup_start_tag(void)
{
    params = (struct tag *) (0x30000100);    /*参数起始位置*/    
    params->hdr.tag = ATAG_CORE;
    params->hdr.size = tag_size(tag_core);

    params->u.core.flags = 0;
    params->u.core.pagesize = 0;
    params->u.core.rootdev = 0;

    params = tag_next(params);              /*指向下一个tag*/
}
void setup_memory_tag(void)
{
    params->hdr.tag = ATAG_MEM;
    params->hdr.size = tag_size(tag_mem32);

    params->u.mem.start = 0x30000000;
    params->u.mem.size = 64*1024*1024;

    params = tag_next(params);              /*指向下一个tag*/
}
unsigned int strlen(char *str)
{
    unsigned int i = 0;
    while(str[i])
    {
        i++;
    }
    return i;
}
void strcpy(char *dest, char *src)
{
    while((*dest++ = *src++) != '\0');
}
void setup_commandline_tag(char *cmdline)
{
    unsigned int len = strlen(cmdline) + 1;
    params->hdr.tag = ATAG_CMDLINE;
    params->hdr.size = ((sizeof(struct tag_header) + len + 3) >> 2); 
    strcpy(params->u.cmdline.cmdline, cmdline);
                
    params = tag_next(params);              /*指向下一个tag*/
}
void setup_end_tag(void)
{
    params->hdr.tag = ATAG_NONE;
    params->hdr.size = 0;
}
int main(void)
{
    void (*theKernel)(int zero, int arch, unsigned int params);
    /*0.设置串口*/
    uart0_init();
    /*1. 从 NAMD FLASH 读内核到内存*/
    puts("Copy kernel from nand\n");
    nand_read(0x60000+64,(unsigned char *)0x30008000, 0x200000);      /*先保证flash内已经烧录内核*/ 
    /*2. 设置参数*/
    puts("Set params\n");
    setup_start_tag();
    setup_memory_tag();
    setup_commandline_tag("noinitrd root=/dev/nfs nfsroot=192.168.1.114:/work/nfs_root/fs_mini_mdev ip=192.168.1.110:192.168.1.114:192.168.1.1:255.255.255.0::eth0:off init=/linuxrc console=ttySAC0");
    setup_end_tag();
    /*3. 跳转执行*/
    puts("Boot kernel\n");
    theKernel = (void(*)(int, int, unsigned int))0x30008000;  /*内核位置 开始启动*/
    theKernel(0, 362, 0x30000100);                    /*arch: 机器ID */
    puts("Boot  Error!\n");
    return -1;


}
