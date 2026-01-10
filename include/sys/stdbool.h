
#ifndef _STDBOOL_H_
#define _STDBOOL_H_ 1

#ifndef __cplusplus

#define bool _Bool
#define true 1
#define false 0
#else
#define _Bool        bool
#define bool        bool
#define false        false
#define true        true
#endif /* !__cplusplus */

#define __bool_types_are_defined 1

#endif /* !_STDBOOL_H_ */