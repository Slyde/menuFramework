/*
 * debug.h
 *
 *  Created on: May 31, 2013
 *      Author: syd
 */

#ifndef DEBUG_H_
#define DEBUG_H_


#define debug(message) debug_msg(__FILE__, __LINE__, message)

void debug_msg(char *file_name, int file_line, char *msg);

#endif /* DEBUG_H_ */
