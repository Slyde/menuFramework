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
	menu_status("Item action and status bar tested");
}

void item2_action(void)
{

}

void item3_action(void)
{

}

void item4_action(void)
{

}
