#ifndef _HAVE_DEBUG_H
#define _HAVE_DEBUG_H

void dfsan_debug(char *, ...);
void dfsan_warning(char *, ...);
void dfsan_error(char *fmt, ...);

#endif /* ! _HAVE_DEBUG_H */