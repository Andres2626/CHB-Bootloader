
#include "lib/libc/stdlib.h"

PRIVATE void div_internal(u64t n, u32t base, u64t *q, u64t *r) 
{
	u64t _q = 0;
	u64t _r = 0;
	
    for (int i = 63; i >= 0; i--) {
        _r = (_r << 1) | ((n >> i) & 1);
        if (_r >= base) {
            _r -= base;
            _q |= ((u64t)1 << i);
        }
    }
    
	*q = _q;
	*r = _r;
}

u64t u64_div(u64t n, u32t base) 
{
	u64t q = 0;
	u64t r = 0;
	div_internal(n, base, &q, &r);
	return q;
}

u64t u64_mod(u64t n, u32t base) {
	u64t q = 0;
	u64t r = 0;
    div_internal(n, base, &q, &r);
    return r;
}