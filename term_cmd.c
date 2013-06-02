/*
 * term_cmd.c
 *
 *  Created on: May 31, 2013
 *      Author: syd
 */

#include <stdio.h>
#include "term_cmd.h"

void clear_page(void)
{
	printf("\033[2J");
	printf("\033[H");
}

void clear_end_of_page(void)
{
	printf("\033[J");
}

void clear_end_of_line(void)
{
	printf("\033[K");
}

void set_highlight(int enable)
{
	if(enable)
		printf("\033[1m");
	else
		printf("\033[0m");
}

void save_cursor_reference(void)
{
	printf("\0337");
}

void restore_cursor_reference(void)
{
	printf("\0338");
}

void set_cursor_pos(struct cursor_pos pos)
{
	printf("\033[%d;%dH", pos.line, pos.col);
}

void backspace(void)
{
	// move cursor back 1 char
	printf("\033[1D");
	// print space
	printf(" ");
	// move cursor back again
	printf("\033[1D");
}

void move_cursor(struct cursor_pos relativePos)
{
	if (relativePos.col > 0)
		printf("\033[%dC",relativePos.col);
	else if (relativePos.col < 0)
		printf("\033[%dD", -relativePos.col);

	if (relativePos.line > 0)
		printf("\033[%dB", relativePos.line);
	else if (relativePos.line < 0)
		printf("\033[%dA", -relativePos.line);
}

void set_cursor_visible(int visible)
{
	if (visible)
		printf("\033[?25h");
	else
		printf("\033[?25l");
}
