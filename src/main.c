#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
/* clang-format off */
#include <readline/history.h>
#include <readline/readline.h>
/* clang-format on */

#include "command.h"
#include "string.h"

extern void exit(int);

void signal_handler(int status) {
	(void)status;

	printf("\n");

	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

int main(void) {
	signal(SIGINT, signal_handler);

	while (true) {
		String input = string_init();

		if (!string_readline(&input)) {
			string_free(&input);

			exit(0);
		}

		string_trim_left(&input);

		if (input.len > 0) {
			Command command = command_parse(&input);

			CommandResult command_result = command_execute(&command);

			if (command_result == CR_NOTFOUND) {
				fprintf(stderr, "ysh: command not found: %s\n",
						command.args.values[0].values);

				command_free(&command);
				string_free(&input);

				exit(0); // We are in a child process not a parent process 
			} else if (command_result == CR_EXIT) {
				command_free(&command);
				string_free(&input);

				exit(0);
			};

			command_free(&command);
			string_free(&input);
		}
	}
}
