#include "menu.h"
#include "term_cmd.h"
#include "debug.h"
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define SEPARATION_LINE_LENGTH	80
#define STATUS_FIELD_OFFSET		2
#define LIVE_SECTION_OFFSET		3
#define MAX_ACTION_LENGTH		3

static pthread_t menu_thr;

static struct menu_page *p_curPage;

static pthread_mutex_t menu_mut;

static struct cursor_pos actionFieldPos;
static struct cursor_pos liveSectionPos;
static struct cursor_pos statusFieldPos;

static int actionBufferEnable = 1;
static pthread_mutex_t actionBufferEnable_mut = PTHREAD_MUTEX_INITIALIZER;

static void (*p_input_handler)(char c) = NULL;
static pthread_mutex_t input_handler_mut = PTHREAD_MUTEX_INITIALIZER;

static char input_buffer[MAX_ACTION_LENGTH];

static char* p_getValueBuffer = NULL;
static unsigned char  getValueBufferLen = 0;
static unsigned char getValueBufferCursorPos = 0;
void (*p_getValueBufferReturnHandler)(int errno, char* retStr);

static void menu_show(void)
{
	int i;

	pthread_mutex_lock(&menu_mut);

	// clear page
	clear_page();

	actionFieldPos.line = 1; // set first offset
	liveSectionPos.line = LIVE_SECTION_OFFSET;
	liveSectionPos.col = 0;
	statusFieldPos.line = STATUS_FIELD_OFFSET;
	statusFieldPos.col = 0;

	// display the title
	set_highlight(1);

	printf("%s\n\r", p_curPage->p_page_title);

	set_highlight(0);

	printf("\n\n");

	actionFieldPos.line += 3;

	// display menu
	for(i=0; i<p_curPage->nb_items; i++)
	{
		// display line
		printf("%3d.\t%s\n\r", i+1, p_curPage->p_items_table[i]->p_item_text);

		actionFieldPos.line++;
	}

	// show the select action
	if (p_curPage->nb_items > 0)
	{
		printf("\n");
		actionFieldPos.line++;
		printf("Select action : ");
		actionFieldPos.col = strlen("Select action : ");

		statusFieldPos.line += actionFieldPos.line;

		// show the separation line between menu and page
		printf("\n\n\n\r");
		liveSectionPos.line = actionFieldPos.line + 4;
		for (i=0; i<SEPARATION_LINE_LENGTH; i++)
		{
			printf("=");
		}

		// enable buffer
		pthread_mutex_lock(&actionBufferEnable_mut);
		actionBufferEnable = 1;
		pthread_mutex_unlock(&actionBufferEnable_mut);
	}
	else
	{
		// disable buffer
		pthread_mutex_lock(&actionBufferEnable_mut);
		actionBufferEnable = 0;
		pthread_mutex_unlock(&actionBufferEnable_mut);
	}

#ifdef DEBUG
	{
		char buffer[50];

		sprintf(buffer, "actionField line : %d", actionFieldPos.line);
		debug(buffer);

		sprintf(buffer, "liveSection line : %d", liveSectionPos.line);
		debug(buffer);

		sprintf(buffer, "statusField line : %d", statusFieldPos.line);
		debug(buffer);
	}
#endif

	set_cursor_pos(actionFieldPos);

	fflush(stdout);

	pthread_mutex_unlock(&menu_mut);
}

void menu_status(char *status)
{
	pthread_mutex_lock(&menu_mut);

	// set the cursor position
	set_cursor_pos(statusFieldPos);

	clear_end_of_line();

	printf("\r");

	printf("%s", status);

	fflush(stdout);

	pthread_mutex_unlock(&menu_mut);
}

void menu_set_action_field(char *value)
{
	pthread_mutex_lock(&menu_mut);

	set_cursor_pos(actionFieldPos);

	clear_end_of_line();

	printf("%s", value);

	pthread_mutex_unlock(&menu_mut);
}

void menu_live_clear(void)
{
	pthread_mutex_lock(&menu_mut);

	set_cursor_pos(liveSectionPos);

	clear_end_of_page();

	pthread_mutex_unlock(&menu_mut);
}

void menu_live_clearline(int line)
{
	struct cursor_pos p;

	pthread_mutex_lock(&menu_mut);

	set_cursor_pos(liveSectionPos);

	p.line = line;
	p.col = 0;
	move_cursor(p);

	clear_end_of_line();

	pthread_mutex_unlock(&menu_mut);
}

void menu_live_print(struct cursor_pos pos, char *str)
{
	pthread_mutex_lock(&menu_mut);

	set_cursor_pos(liveSectionPos);

	move_cursor(pos);

	printf("%s", str);

	fflush(stdout);

	pthread_mutex_unlock(&menu_mut);
}

static void menu_get_value_input_handler(char c)
{
	int tmpLen;
	struct cursor_pos p={.line=0};

	tmpLen = strlen(p_getValueBuffer);
	{
		char str[50];
		sprintf(str, "New char received %x | Buffer len : %d", c, strlen(p_getValueBuffer));
		debug(str);
	}
	if( c >= 0x20 && c < 0x7F) // only allow displayable chars
	{
		// store the char
		p_getValueBuffer[tmpLen] = c;
		tmpLen++;
	}
	else if(c == 0x08 || c == 0x7F)
	{
		// remove the last char
		if(tmpLen > 0)
		{
			p_getValueBuffer[tmpLen -1] = 0;
		}
	}

	p.col=getValueBufferCursorPos;

	// display the buffer
	pthread_mutex_lock(&menu_mut);
	set_cursor_pos(statusFieldPos);
	move_cursor(p);
	clear_end_of_line();
	printf("%s", p_getValueBuffer);
	fflush(stdout);
	pthread_mutex_unlock(&menu_mut);

	// check for CR or LF
	if(c == 0x0A || c == 0x0D || tmpLen >= getValueBufferLen)
	{
		// make sure to force last buffer char to 0
		p_getValueBuffer[getValueBufferLen] = 0;

		// call the callback function
		if(p_getValueBufferReturnHandler != NULL)
			p_getValueBufferReturnHandler(0, p_getValueBuffer);

		// restore the default input handler
		menu_set_input_handler(NULL);

		// free the buffer
		free(p_getValueBuffer);
		p_getValueBuffer = NULL;
		getValueBufferLen = 0;
	}
}

void menu_get_value(char *message, unsigned char maxLen, void (*returnHandler)(int errno, char* retStr))
{
	debug("Entering menu_get_value()");

	// create the buffer
	if(p_getValueBuffer != NULL)
		free(p_getValueBuffer);

	p_getValueBuffer = malloc(maxLen+1);
	getValueBufferLen = maxLen;
	p_getValueBufferReturnHandler = returnHandler;
	getValueBufferCursorPos = strnlen(message, 255) + 1;

	if(p_getValueBuffer != NULL)
	{
		debug("Buffer is alocated, change the input handler");

		menu_status(message);

		memset(p_getValueBuffer, 0, getValueBufferLen);
		// set the input handler
		menu_set_input_handler(menu_get_value_input_handler);
	}
	else
	{
		char msg[] = "Cannot allocate buffer !!!";

		debug(msg);

		menu_status(msg);
		getValueBufferLen = 0;

		if(p_getValueBufferReturnHandler != NULL)
			p_getValueBufferReturnHandler(-1, NULL);
	}
}

void menu_exit(void)
{
	pthread_mutex_lock(&menu_mut);

	// call the current page exit
	if (p_curPage->p_page_exit != NULL)
		p_curPage->p_page_exit();

	// check the prev pointer
	if (p_curPage->p_prev != NULL)
	{
		struct menu_page *tmp = p_curPage;
		p_curPage = p_curPage->p_prev;
		tmp->p_prev = NULL;

		// show the new page
		menu_show();

		// call the page enter function
		if (p_curPage->p_page_enter)
			p_curPage->p_page_enter();
	}
	else
	{
		// exit
		pthread_cancel(menu_thr);
	}

	pthread_mutex_unlock(&menu_mut);
}

void menu_enter(struct menu_page *p_page)
{
	pthread_mutex_lock(&menu_mut);

	// save the previous page
	p_page->p_prev = p_curPage;

	// call the page exit function
	if (p_curPage->p_page_exit != NULL)
		p_curPage->p_page_exit();

	// points to the new page
	p_curPage = p_page;

	// show the new page
	menu_show();

	// call the page enter function
	if (p_curPage->p_page_enter)
		p_curPage->p_page_enter();

	pthread_mutex_unlock(&menu_mut);

}

static void menu_default_input_handler(char c)
{

	switch(c)
	{
		// test CR and LF
		case 0x0A:
		case 0x0D:
		{
			int actionNum;
			int tmpActionBufferEnable;

			// check if buffer is enabled
			pthread_mutex_lock(&actionBufferEnable_mut);
			tmpActionBufferEnable = actionBufferEnable;
			pthread_mutex_unlock(&actionBufferEnable_mut);

			if (!tmpActionBufferEnable)
				break;

			// execute the action

			// compute the number
			actionNum = atoi(input_buffer);

#ifdef DEBUG
			{
				char debugBuf[40];

				sprintf(debugBuf, "Selected action : %d", actionNum);
				debug(debugBuf);
			}
#endif

			if (actionNum > 0 && actionNum <= p_curPage->nb_items)
			{
				debug("Action is valid, call the item action");
				if (p_curPage->p_items_table[actionNum-1]->p_item_action != NULL)
					p_curPage->p_items_table[actionNum-1]->p_item_action();
				else
					debug("No action for this item");
			}
			else
			{
				menu_status("Invalid action !");
#ifdef DEBUG
				{
				char str[50];
				debug("Invalid action ! ");
				sprintf(str, "actionNum = %d | nb_items = %d", actionNum, p_curPage->nb_items);
				debug(str);
				}
#endif
			}

			// clear the buffer
			memset(input_buffer, 0, sizeof(input_buffer));
			set_cursor_pos(actionFieldPos);
			clear_end_of_line();
		}break;
		// test for backspace
		case 0x08:
		case 0x7F:
			if(strlen(input_buffer) > 0)
			{
				// delete the last char
				input_buffer[strlen(input_buffer)-1] = '\0';
				// redraw the buffer
				menu_set_action_field(input_buffer);
			}
			break;
		// store only numerical chars
		default:
		{
			int tmpActionBufferEnable;

			debug("Try to store new char in buffer");

			// check if buffer is enabled
			pthread_mutex_lock(&actionBufferEnable_mut);
			tmpActionBufferEnable = actionBufferEnable;
			pthread_mutex_unlock(&actionBufferEnable_mut);

			if (!tmpActionBufferEnable)
				break;

			if(c >='0' && c <= '9')
			{
				// check for buffer overflow
				if(strlen(input_buffer) <= MAX_ACTION_LENGTH)
				{
					input_buffer[strlen(input_buffer)] = c;
					// update the display
					menu_set_action_field(input_buffer);
				}
				else
				{
					debug("Action buffer full...");
				}
			}
		}break;
	}
}

void menu_set_input_handler(void (*handler)(char c))
{
	pthread_mutex_lock(&input_handler_mut);

	p_input_handler = handler;

	if (handler == NULL)
		memset(input_buffer,0,sizeof(input_buffer));

	pthread_mutex_unlock(&input_handler_mut);
}

static void* menu_thread(void* unused)
{
	menu_show();

	for(;;)
	{
		int c;

		// wait for chars
		c = getchar();
		{
			char buf[10];
			sprintf(buf,"0x%x",c);
			debug(buf);
		}

		// check for specific chars
		switch(c)
		{
		// test escape char
		case 0x1B:
			// exit the menu
			menu_exit();
			break;
		default:

			if (p_input_handler == NULL)
				menu_default_input_handler(c);
			else
				p_input_handler(c);
			break;

		}
	}


	return NULL;
}


void menu_start(struct menu_page *p_page)
{
	pthread_mutexattr_t mutexattr;

	// disable echo
	system("stty -echo raw");

	// hide cursor
	set_cursor_visible(0);

	// set first page
	p_curPage = p_page;

	// initialize mutex as reentrent
	pthread_mutexattr_init(&mutexattr);
	pthread_mutexattr_settype(&mutexattr, PTHREAD_MUTEX_RECURSIVE);
	pthread_mutex_init(&menu_mut, &mutexattr);
	pthread_mutex_init(&input_handler_mut, &mutexattr);
	pthread_mutexattr_destroy(&mutexattr);

	// clear input buffer
	memset(input_buffer, 0, sizeof(input_buffer));

	// start menu thread
	pthread_create(&menu_thr, NULL, menu_thread, NULL);
}

void menu_join(void)
{
	pthread_join(menu_thr, NULL);

	clear_end_of_page();

	printf("\n\r");

	system("stty echo cooked");

	set_cursor_visible(1);
}
