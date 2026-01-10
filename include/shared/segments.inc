.title segments.INC -- macros for convert linear address to seg:off

.macro linear_to_segment q, w, e, r

    mov \q, \e     
    shr $0x4, \e
    mov \r, \w
    mov \q, \e
    and $0xf, \e

.endm