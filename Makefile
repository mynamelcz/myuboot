CC			= arm-linux-gcc
LD			= arm-linux-ld
AR			= arm-linux-ar 
OBJCOPY		= arm-linux-objcopy 
OBJDUMP		= arm-linux-objdump	

CFLAGS		:= -Wall -o2
CPPFLAG 	:= -nostdinc -fno-builtin

objs := start.o init.o boot.o uart.o

boot.bin: $(objs)
	$(LD)	-Tboot.lds -o boot.elf	$^
	$(OBJCOPY) -O binary -S boot.elf $@
	$(OBJDUMP) -D -m arm boot.elf > boot.dis

%.o: %.c
	$(CC) $(CPPFLAG) $(CFLAGS) -c -o $@ $<

%.o: %.S
	$(CC) $(CPPFLAG) $(CFLAGS) -c -o $@ $<

clean:
	rm -f *.o *.bin *.elf *.dis
