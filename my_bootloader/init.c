/*从flash复制代码到SDRAM
 *
 *若是从NandFlash 0地址对应于片内内存，所以可读可写
 *若是从NorFlash  0地址对应于flash 0地址，所以只可以直接读，不能直接写
 */
int isBootFromNorFlash(viod)
{
   volatile int *p = (volatile int *)0;
   int val = *p;
  *p = 0x123456;
   if(*p == 0x123456)
   {                /*可读可写 所以为NandFlash启动*/
    *p = val;       /*恢复为原来的值*/
    return 0;
   } 
   return 1;
}
int copy_code_to_sdram(unsigned char *src, unsigned char *dest, unsigned int len)
{
    int i=0;
    if(isBootFromNorFlash())
    {
        while(i < len)
        {
            dest[i] = src[i];
            i++;
        }
    }
    else
    {
        nand_init();
        nand_read(src, dest, len);
    }
}

void clean_bss(void)
{
    extern int __bss_start, __bss_end;
    int *p = &__bss_start;
    for(;p < &__bss_end; p++)
        *p = 0;   
}


