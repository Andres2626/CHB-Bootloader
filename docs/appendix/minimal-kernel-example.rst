==================================
APPENDIX A: Minimal kernel example
==================================

.. code-block:: gas

    # =========================================
    # kernel.S entry point of the kernel
    # =========================================

    .code32                             # use protected mode
    .section .text
    .global _start                      # set _start as global function
    _start:
        mov $0x90000, %esp              # setup stack (%esp can be arbitrary value)
        push %eax                       # push pointer of CHB structure
        
        call kmain                      # call to C kernel function

    /* if kmain returns call to stop (just in case) */
    stop:
        jmp stop                        # infinite loop

.. code-block:: C

    /* kernel.c -- C part of the kernel. */
    
    #define VGA_MEM 0xB8000 // location of display memory (text mode)
    
    /* kernel main entry */
    /* optional: The arguments can contain the pointer to the structure
    that CHB passes with the information obtained at startup time */
    void kmain(void)
    {
        char *video = (char*)VGA_MEM;
        video[0] = 'H'; // put 'h' to first display entry
        video[1] = 0x07; // set 'h' color attribute (gray text and black foreground)
        video[2] = 'i'; // put 'i' to second display entry
        video[3] = 0x07; // set 'i' color attribute (gray text and black foreground)
        for (;;); // infinite loop
    }
