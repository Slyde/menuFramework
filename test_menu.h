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

/* -------------------- PAGE 1 ----------------------*/

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


/* -------------------- PAGE 1.1 ----------------------*/

void subMenu1_item1_action(void);

struct menu_item subMenu1_item1 = {
		.p_item_text = "Go deeper...",
		.p_item_action = subMenu1_item1_action
};

struct menu_item subMenu1_item2 = {
		.p_item_text = "Dummy menu",
		.p_item_action = NULL
};

struct menu_item subMenu1_item3 = {
		.p_item_text = "Dummy menu 2",
		.p_item_action = NULL
};

struct menu_item *subMenu1ItemTable[] =
{
		&subMenu1_item1,
		&subMenu1_item2,
		&subMenu1_item3
};

char subMenu1Title[] = "A sub menu ...";
struct menu_page subMenuPage = {
		.p_prev = NULL,
		.p_page_title = subMenu1Title,
		.p_items_table = (struct menu_item**)&subMenu1ItemTable,
		.nb_items = 3,
		.p_page_enter = NULL,
		.p_page_exit = NULL
};

/* -------------------- PAGE 1.1.1 ----------------------*/

struct menu_item subMenu2_item1 = {
		.p_item_text = "Dummy menu ",
		.p_item_action = NULL
};

struct menu_item subMenu2_item2 = {
		.p_item_text = "Dummy menu 2",
		.p_item_action = NULL
};

struct menu_item *subMenu2ItemTable[] =
{
		&subMenu2_item1,
		&subMenu2_item2
};

char subMenu2Title[] = "A second level of sub menu ...";
struct menu_page subMenu2Page = {
		.p_prev = NULL,
		.p_page_title = subMenu2Title,
		.p_items_table = (struct menu_item**)&subMenu2ItemTable,
		.nb_items = 2,
		.p_page_enter = NULL,
		.p_page_exit = NULL
};


#endif /* TESTCONSOLE_MENU_H_ */
