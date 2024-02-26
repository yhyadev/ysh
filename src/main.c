#include <stdbool.h>
#include <stdio.h>

#include "command.h"
#include "string.h"

extern void exit(int);

int main(void) {
	while (true) {
		String input = string_init();

		if (!string_read_line(&input)) {
			string_free(&input);

			break;
		}

		string_trim_left(&input);

		if (input.len > 0) {
			Command command = command_parse(&input);

			CommandResult command_result = command_execute(&command);

			if (command_result == CR_NOTFOUND) {
				fprintf(stderr, "ysh: command not found: %s\n",
						command.args.values[0].values);
			} else if (command_result == CR_EXIT) {
				command_free(&command);

				string_free(&input);

				exit(0);
			};

			command_free(&command);
		}

		string_free(&input);
	}

	return 0;
}
