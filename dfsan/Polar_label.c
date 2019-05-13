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

static unsigned int offset_all = 0;

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
        dfsan_debug("assigning label %d to offest %d\n", L, i);
    }
    offset_all = MAX(offset_all, to);
    dfsan_debug("current max offset: %d\n", offset_all);
}

Polar_range has_label(dfsan_label L)
{
    Polar_range ans;
    ans.start = ans.end = -1;

    int i = 0;
    for (i = 0; i <= offset_all; i++)
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

Polar_range Polar_check(dfsan_label L, int id)
{
    if(id < 0)
    {
        dfsan_error("func_id is less than 0");
    }
    Polar_range ans = has_label(L);

    mkdir("/root/.Polar");
    char str[200] = "/root/.Polar/";
    char str1[200];
    
    sprintf(str1, "%d", id);
    strcat(str,str1);

    FILE* fd = fopen(str, "r");
    if(fd == NULL)
    {
        dfsan_debug("creating %s\n", str);
        fd = fopen(str, "w");
        fprintf(fd, "%d %d\n", ans.start, ans.end);
    }
    else
    {
        dfsan_debug("rewriting %s\n", str);
        int s, e;
        fscanf(fd, "%d %d", &s, &e);
        if(s == -1 || e == -1)
        {
            goto clean;
        }
        if(ans.start >= s && ans.end <= e)
        {
            fclose(fd);
            fd = fopen(str, "w");
            fprintf(fd, "%d %d\n", ans.start, ans.end);
        }
        else if(ans.start <= s && ans.end >= e)
        {
            
        }
        else
        {
            fclose(fd);
            fd = fopen(str, "w");
            fprintf(fd, "-1 -1\n");
        }
    }
clean:
    fclose(fd);
    dfsan_debug("id: %d \t start: %d \t end: %d\n", id, ans.start, ans.end);

    return ans;
}
