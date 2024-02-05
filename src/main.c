#include <ctype.h>
#include <ncurses.h>
#include <stdbool.h>

#include "command.h"
#include "string.h"

#define KEY_CTRL_D 4
#define KEY_CTRL_C 3
#define KEY_DEL 127

void main_loop(void) {
	bool typing = false;

	String input = string_init();

	while (true) {
		if (getcury(stdscr) >= LINES - 1) {
			clear();
		}

		if (!typing) {
			printw("$ ");
            refresh();

			typing = true;

			continue;
		}

		int input_ch = getch();

		if (input_ch == KEY_CTRL_D) {
			break;
		}

		if (input_ch == KEY_CTRL_C) {
			addch('\n');
			refresh();

			string_free(&input);
			input = string_init();

			typing = false;

			continue;
		}

		if (input_ch == KEY_DEL) {
			if (input.len > 0) {
				string_pop(&input);

				move(getcury(stdscr), getcurx(stdscr) - 1);
				delch();
			}

			continue;
		}

		if (input_ch == '\n') {
			addch('\n');
			refresh();

			string_trim_left(&input);

			if (input.len > 0) {
				Command command = command_parse(&input);

				CommandResult command_result = command_execute(&command);

				if (command_result == CR_NOTFOUND) {
					printw("ysh: command not found: %s\n",
						   command.args.values[0].values);
					refresh();
				} else if (command_result == CR_EXIT) {
					command_free(&command);

					break;
				};

				command_free(&command);
			}

			string_free(&input);
			input = string_init();

			typing = false;

			continue;
		}

		if (isprint(input_ch)) {
			string_push(&input, input_ch);

			addch(input_ch);
			refresh();
		}
	}

	string_free(&input);
}

int main(void) {
	initscr();

	raw();

	noecho();

	main_loop();

	endwin();

	return 0;
}
