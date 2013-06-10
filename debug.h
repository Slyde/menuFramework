/*
 * debug.h
 *
 *  Created on: May 31, 2013
 *      Author: syd
 */

#ifndef DEBUG_H_
#define DEBUG_H_

#ifdef DEBUG
#define debug(message, ...) debug_msg(__FILE__, __LINE__, message, ##__VA_ARGS__)
#else
#define debug(message, ...)
#endif

void debug_msg(char *file_name, int file_line, const char *format, ...);

#endif /* DEBUG_H_ */
