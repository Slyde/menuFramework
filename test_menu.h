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


struct menu_item firstPage_item1 = {
		.p_item_text = "First item...",
		.p_item_action = item1_action
};
struct menu_item firstPage_item2 = {
		.p_item_text = "Second item...",
		.p_item_action = item2_action
};
struct menu_item firstPage_item3 = {
		.p_item_text = "Third item...",
		.p_item_action = NULL
};

struct menu_item *firstPageItemTable[] =
{
		&firstPage_item1,
		&firstPage_item2,
		&firstPage_item3
};

char firstPageTitle[] = "First page ...";
struct menu_page firstPage = {
		.p_prev = NULL,
		.p_page_title = firstPageTitle,
		.p_items_table = (struct menu_item**)&firstPageItemTable,
		.nb_items = 3,
		.p_page_enter = NULL,
		.p_page_exit = NULL
};

char item1ActionPageTitle[] = "Item1 action ...";
struct menu_page item1Page = {
		.p_prev = NULL,
		.p_page_title = item1ActionPageTitle,
		.p_items_table = NULL,
		.nb_items = 0,
		.p_page_enter = NULL,
		.p_page_exit = NULL
};


#endif /* TESTCONSOLE_MENU_H_ */
