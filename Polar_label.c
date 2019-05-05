#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <stdint.h>
#include <inttypes.h>
#include <sanitizer/dfsan_interface.h>
#include "debug.h"
#include "config.h"
#include "Polar_label.h"

static dfsan_label mask[MAP_SIZE];

static unsigned int offset_max = 0;

#  define MAX(_a,_b) ((_a) > (_b) ? (_a) : (_b))

void set_label(int from, int to, dfsan_label L)
{
    if(from < 0)
    {
        dfsan_error("The start location of label to set is %d (less than 0) in func %s\n", from, __func__);
        from = 0;
    }

    if(to >= MAP_SIZE)
    {
        dfsan_error("The end location of label to set is %d (more than MAP_SIZE - 1) in func %s\n", to, __func__);
        to = MAP_SIZE - 1;
    }

    int i = 0;
    for(i = from; i <= to; i += granularity) 
    {
        mask[i] = L;
    }
    offset_max = MAX(offset_all, to);
}

Polar_range has_label(dfsan_label L)
{
    Polar_range ans;
    ans.start = ans.end = -1;

    int i = 0;
    for (i = 0; i <= offset_max; i++)
    {
        if(dfsan_has_label(L, mask[i]))
        {
            if(ans.start == -1)
            {
                ans.start = i;
            }
            ans.end = i;
        }
    }

    return ans;
}
