/*
 * testConsole_menu.h
 *
 *  Created on: May 31, 2013
 *      Author: syd
 */

#ifndef TESTCONSOLE_MENU_H_
#define TESTCONSOLE_MENU_H_

#include "menu_types.h"

void item1_action(void);
void item2_action(void);
void item3_action(void);
void item4_action(void);


struct menu_item firstPage_item1 = {
		.p_item_text = "Test item action",
		.p_item_action = item1_action
};
struct menu_item firstPage_item2 = {
		.p_item_text = "Test submenu",
		.p_item_action = item2_action
};
struct menu_item firstPage_item3 = {
		.p_item_text = "Test live",
		.p_item_action = item3_action
};
struct menu_item firstPage_item4 = {
		.p_item_text = "Test input handler",
		.p_item_action = item4_action
};

struct menu_item *firstPageItemTable[] =
{
		&firstPage_item1,
		&firstPage_item2,
		&firstPage_item3,
		&firstPage_item4
};

char firstPageTitle[] = "First page ...";
struct menu_page firstPage = {
		.p_prev = NULL,
		.p_page_title = firstPageTitle,
		.p_items_table = (struct menu_item**)&firstPageItemTable,
		.nb_items = 4,
		.p_page_enter = NULL,
		.p_page_exit = NULL
};



#endif /* TESTCONSOLE_MENU_H_ */
