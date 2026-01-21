/* kernel.c -- C part of the kernel. */
    
#define VGA_MEM 0xB8000 // location of display memory (text mode)

void kmain(void)
{
    char *video = (char*)VGA_MEM;
    video[0] = 'H';
    video[1] = 0x07;
    video[2] = 'i';
    video[3] = 0x07;
    for (;;);
}
