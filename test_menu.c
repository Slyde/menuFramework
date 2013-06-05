/*
 * testConsole_menu.c
 *
 *  Created on: May 31, 2013
 *      Author: syd
 */

#include <pthread.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "menu.h"
#include "debug.h"
#include "test_menu.h"


void item1_action(void)
{
	debug("Test item1_action and print to status bar");
	menu_status("Item action and status bar tested");
}

void item2_action(void)
{
	debug("Entering sub menu");
	menu_enter(&subMenuPage);
}

void item3_action(void)
{

}

void item4_action(void)
{

}

void item5_newValueHandler(int errno, char* str)
{
	char msg[255];

	debug("New value received");

	sprintf(msg, "New value received : errno : %d | value : %s", errno, str);

	menu_status(msg);
}

void item5_action(void)
{
	debug("Try th get value function");
	menu_get_value("Test the get value function :", 10, item5_newValueHandler);
}

void subMenu1_item1_action(void)
{
	debug("Entering a second sub menu level");
	menu_enter(&subMenu2Page);
}

