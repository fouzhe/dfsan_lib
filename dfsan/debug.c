#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

int dfsan_debug_on = 1;
int dfsan_warning_on = 1;
int dfsan_error_on = 1;

void dfsan_debug(char *fmt, ...)
{
	if (!dfsan_debug_on) return;

	printf("\x1b[1;92m [*] ");
	va_list args;
	va_start(args,fmt);
	vprintf(fmt,args);
	va_end(args);
	printf("\x1b[0m");
	fflush(stdout);
}

void dfsan_warning(char *fmt, ...)
{
	if (!dfsan_warning_on) return;

	printf("\033[1;35;40m WARNING: ");
	va_list args;
	va_start(args,fmt);
	vprintf(fmt,args);
	va_end(args);
	printf("\033[0m");
	fflush(stdout);
}

void dfsan_error(char *fmt, ...)
{
	if (!dfsan_error_on) return;

	printf("\033[1;31;40m ERROR: ");
	va_list args;
	va_start(args,fmt);
	vprintf(fmt,args);
	va_end(args);
	printf("\033[0m");
	fflush(stdout);
	exit(1);
}

__attribute__((constructor)) void debug_init()
{
	dfsan_debug_on = dfsan_debug_on || (getenv("DFSAN_DEBUG") && (strcmp(getenv("DFSAN_DEBUG"), "1") == 0));
	dfsan_warning_on = dfsan_warning_on || (getenv("DFSAN_WARNING") && (strcmp(getenv("DFSAN_WARNING"), "1") == 0));
	dfsan_error_on = dfsan_error_on || (getenv("DFSAN_ERROR") && (strcmp(getenv("DFSAN_ERROR"), "1") == 0));
}
