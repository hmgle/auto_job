#ifndef _AUTO_FIND_EXTREMUN_
#define _AUTO_FIND_EXTREMUN_
#define _in         /* 输入参数 */

#include <stdint.h>

int find_extremum_in_range(_in int min, _in int max,_in uint32_t (*f)(int));

#endif
