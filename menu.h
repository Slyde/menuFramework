/*
 * menu.h
 *
 *  Created on: May 26, 2013
 *      Author: syd
 */

#ifndef MENU_H_
#define MENU_H_

#include "menu_types.h"
#include "term_cmd.h"

void menu_start(struct menu_page *p_page);

void menu_enter(struct menu_page *p_page);

void menu_exit(void);

void menu_join(void);

void menu_set_input_handler(void (*handler)(char c));
void menu_status(char *status);
void menu_live_clear(void);
void menu_live_clearline(int line);
void menu_live_print(struct cursor_pos pos, char *str);
void menu_get_value(char *message, unsigned char maxLen, void (*returnHandler)(int errno, char* retStr));

#endif /* MENU_H_ */
