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

static dfsan_label mask[MAP_SIZE];

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
}

int has_label(dfsan_label L, int pos)
{
    if(pos < 0 || pos >= MAP_SIZE)
    {
        dfsan_error("The variable pos is not in correct range in func %s\n",  __func__);
        return 0;
    }
    
    pos = pos / granularity * granularity;

    return dfsan_has_label(L, mask[pos]);
}
