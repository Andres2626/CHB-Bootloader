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