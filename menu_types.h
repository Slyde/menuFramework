/*
 * menu_types.h
 *
 *  Created on: May 30, 2013
 *      Author: syd
 */

#ifndef MENU_TYPES_H_
#define MENU_TYPES_H_

struct menu_item
{
    char *p_item_text;
    void (*p_item_action)(void);	// function called when item is selected
};

struct menu_page
{
	struct menu_page *p_prev;		// store previous page for exiting
	char *p_page_title;			// string containing the page title
	struct menu_item **p_items_table; // table containing the menu items
	int nb_items;	// nb items in the menu
	void (*p_page_enter)(void);	// called when the page is entered
	void (*p_page_exit)(void);	// called when the page is exited
};


#endif /* MENU_TYPES_H_ */
