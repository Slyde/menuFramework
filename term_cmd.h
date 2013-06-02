/*
 * term_cmd.h
 *
 *  Created on: May 31, 2013
 *      Author: syd
 */

#ifndef TERM_CMD_H_
#define TERM_CMD_H_

struct cursor_pos
{
	int line;
	int col;
};

void clear_page(void);
void clear_end_of_page(void);
void clear_end_of_line(void);
void set_highlight(int enable);
void set_cursor_pos(struct cursor_pos pos);
struct cursor_pos get_cursor_pos(void);
void save_cursor_reference(void);
void restore_cursor_reference(void);
void backspace(void);
void move_cursor(struct cursor_pos relativePos);
void set_cursor_visible(int visible);

#endif /* TERM_CMD_H_ */
