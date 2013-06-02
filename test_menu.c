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
#include "term_cmd.h"

static pthread_t item1_thr;
static volatile bool item1_thr_running = false;

static void* item1_thread(void* unused)
{
	int i;
	struct cursor_pos p ={.line=0,.col=0};
	char tmp[30];

	menu_status("Item1 action started...");

	menu_live_clear();

	menu_live_print(p, "Live display...\n\n");

	p.line = 2;

	for(i=0; i<5; i++)
	{
		sprintf(tmp, "\rThis is the %d loop...\n", i);
		menu_live_clearline(p.line);
		menu_live_print(p, tmp);
		sleep(1);
	}

	menu_status("Item1 action terminated...");

	item1_thr_running = false;

	menu_exit();

	return NULL;
}

void item1_action(void)
{
	pthread_attr_t attr;

	if( !item1_thr_running )
	{
		item1_thr_running = true;
		pthread_attr_init(&attr);
		pthread_attr_setdetachstate(&attr, 1);
		pthread_create(&item1_thr, &attr, item1_thread, NULL);
		pthread_attr_destroy(&attr);

		menu_enter(&item1Page);
	}
	else
	{
		debug("Thread is already initialized...");
		menu_status("Action 1 is running...");
	}
}

static char buffer[30];

void item2_display(void)
{
	struct cursor_pos p={0,0};
	char tmp[50];

	menu_live_clear();

	menu_live_print(p, "OK done...");

	p.line += 2;

	sprintf(tmp, "Hello %s", buffer);
	menu_live_print(p, tmp);
}

void item2_input_handler(char c)
{
	struct cursor_pos p={.line=0, .col=17};

	switch(c)
	{
	case '\n':
	case '\r':

		// display hello world...
		item2_display();

		// restore default handler
		menu_set_input_handler(NULL);

		break;

	default:
		// store the char in buffer
		if (strlen(buffer) < sizeof(buffer))
		{
			buffer[strlen(buffer)] = c;

			menu_live_print(p, buffer);
		}

		break;
	}

}


void item2_action(void)
{
	struct cursor_pos p={0,0};

	// change the input handler
	menu_set_input_handler(item2_input_handler);

	// display a message on live section
	menu_live_clear();
	menu_live_print(p, "Enter your name : ");

	memset(buffer, 0, sizeof(buffer));

	{
		char tmp[50];
		sprintf(tmp, "Size of buffer is %d", sizeof(buffer));
		debug(tmp);
	}
}
