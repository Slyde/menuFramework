/*
 * debug.c
 *
 *  Created on: May 31, 2013
 *      Author: syd
 */

#include <stdio.h>
#include <time.h>
#include <stdarg.h>

void debug_msg(char *file_name, int file_line, const char *format, ...)
{
	va_list ap;

	time_t t;
	struct tm lt;
	char str[10];

	va_start(ap, format);

	time(&t);
	lt = *localtime(&t);

	strftime(str, 9, "%H:%M:%S", &lt);

	fprintf(stderr, "%s %s:%d => ", str, file_name, file_line);
	vfprintf(stderr, format, ap);
	fprintf(stderr, "\n");

	fflush(stderr);

}

