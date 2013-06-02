/*
 * debug.h
 *
 *  Created on: May 31, 2013
 *      Author: syd
 */

#ifndef DEBUG_H_
#define DEBUG_H_


#define debug(message) debug_msg(__FILE__, message)

void debug_msg(char *file_name, char *msg);

#endif /* DEBUG_H_ */
