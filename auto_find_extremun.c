#include <stdint.h>
#include "auto_find_extremun.h"

int find_extremum_in_range(_in int min, _in int max,_in uint32_t (*f)(int))
{
    uint32_t f_ret0, f_retl, f_retr;
    uint32_t f_max;
    int f_max_x;
    int tmp;
    uint32_t f_tmp_sum;
    int i;

    if (min > max) {
        tmp = min;
        min = max;
        max = tmp;
    }

    if (min == max)
        return min;
    else if (max - min == 2) {  /* 特殊情况 */
        f_retl = f(min);
        f_ret0 = f(min + 1);
        f_retr = f(min + 2);

        if (f_retl > f_ret0 && f_retl > f_retr)
            return min;
        else if (f_ret0 > f_retr)
            return min + 1;
        else
            return min + 2;
    }

    /*
     * 左边界
     */
    f_max_x = min;
    f_ret0 = f(min);
    f_retl = f_ret0;
    f_retr = f(min + 1);
    f_max = f_ret0 + f_retl + f_retr;
    
    for (i = min + 1; i < max; i++) {
        f_retl = f_ret0;
        f_ret0 = f_retr;
        f_retr = f(i + 1);
        f_tmp_sum = f_ret0 + f_retl + f_retr;

        if (f_max < f_tmp_sum) {
            f_max = f_tmp_sum;
            f_max_x = i;
        }
    }

    /*
     * 右边界
     */
    f_retl = f_ret0;
    f_ret0 = f_retr;
    f_tmp_sum = f_ret0 + f_retl + f_retr;
    if (f_max < f_tmp_sum) {
        f_max_x = max;
    }

    return f_max_x;
}
