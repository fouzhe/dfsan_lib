#ifndef _HAVE_LABEL_H
#define _HAVE_LABEL_H

#include <sanitizer/dfsan_interface.h>

void set_label(int from, int to, dfsan_label L);
int has_label(dfsan_label L, int pos);

#endif /* ! _HAVE_LABEL_H */