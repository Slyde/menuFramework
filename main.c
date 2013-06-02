//#include "menu.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "menu.h"
#include "debug.h"

extern struct menu_page firstPage;

int main(void)
{
	debug("Application started");

	menu_start(&firstPage);

	debug("Menu started");

	menu_join();

	debug("Application terminated");

	return 0;
}
