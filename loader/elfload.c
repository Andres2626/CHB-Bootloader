
/*
* elfload.c -- simple elf loader
*
* Copyright (C) 2021 - 2025 andres26
*
* This file is distributed under the terms of the MIT license.
*/

#include <elf.h>
#include <fs/fs.h>
#include <sys/exec.h>

#include "lib/libc/ctype.h"
#include "lib/libc/errno.h"
#include "lib/libc/minmax.h"
#include "lib/libc/stdio.h"
#include "lib/libc/string.h"

int elf_load(void *input, u32t size, void **entry_point)
{   
    if (!input || !size)
        return SIGN(EINVAL);
    
    if (size < sizeof(Elf32_Ehdr))
        return SIGN(EINVAL);
    
    Elf32_Ehdr *header = (void*)input;
    bool chk = true;
    chk = chk && header->e_ident[EI_MAG0] == ELFMAG0;
    chk = chk && header->e_ident[EI_MAG1] == ELFMAG1;
    chk = chk && header->e_ident[EI_MAG2] == ELFMAG2;
    chk = chk && header->e_ident[EI_MAG3] == ELFMAG3;
    chk = chk && header->e_ident[EI_CLASS] == ELFCLASS32;
    chk = chk && header->e_ident[EI_DATA] == ELFDATA2LSB;
    chk = chk && header->e_ident[EI_VERSION] == EV_CURRENT;
    chk = chk && header->e_type == ET_EXEC;
    chk = chk && header->e_machine == EM_386;
    chk = chk && header->e_version == EV_CURRENT;
    chk = chk && size >= header->e_ehsize;
    chk = chk && header->e_phoff > 0;
    chk = chk && header->e_phnum > 0;
    chk = chk && header->e_phentsize == sizeof(Elf32_Phdr);
    chk = chk && (header->e_phoff + header->e_phnum * header->e_phentsize <= size);
    
    if (!chk)
        return SIGN(ENOEXEC);
    
    /* set entry_point */
    *entry_point = (void*)(Elf32_Addr)header->e_entry;
    
    /* get segments */
    Elf32_Phdr *segs = (Elf32_Phdr*)((u8t*)input + header->e_phoff);
    for (u32t i = 0; i < header->e_phnum; i++) {
        Elf32_Phdr *ph = &segs[i];
        
        if (ph->p_type != PT_LOAD)
            continue;
        
        if (ph->p_offset + ph->p_filesz > size)
            return SIGN(ENOEXEC);
        
        if (ph->p_memsz < ph->p_filesz)
            return SIGN(ENOEXEC);
        
        /* protect stage2 */
        if (ph->p_paddr < 0x23000)
            return SIGN(EFAULT);
        
        memcpy((void*)ph->p_paddr, (u8t*)input + ph->p_offset, ph->p_filesz);
        
        /* initialize BSS */
        if (ph->p_memsz > ph->p_filesz)           
            memset((void*)(ph->p_paddr + ph->p_filesz), 0, ph->p_memsz - ph->p_filesz);
             
    }
    
    return 0;
}