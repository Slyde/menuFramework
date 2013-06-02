/*
 * debug.c
 *
 *  Created on: May 31, 2013
 *      Author: syd
 */

#include <stdio.h>
#include <time.h>

#ifdef DEBUG
void debug_msg(char *file_name, int file_line, char *msg)
{

	time_t t;
	struct tm lt;
	char str[10];

	time(&t);
	lt = *localtime(&t);

	strftime(str, 9, "%H:%M:%S", &lt);

	fprintf(stderr, "%s %s:%d => %s\n", str, file_name, file_line, msg);

}
#else
void debug_msg(char *file_name, char *msg)
{

}
#endif
