#include <errno.h>
#include <ncurses.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "command.h"
#include "string.h"

Command command_parse(String *input) {
	Strings args = string_tokenize_words(input);

	return (Command){.args = args};
}

void command_free(Command *command) { strings_free(&command->args); }

CommandResult command_execute_builtin(Command *command) {
	if (strcmp(command->args.values[0].values, "cd") == 0) {
		if (command->args.len > 1) {
			chdir(command->args.values[1].values);
		}

		return CR_OK;
	} else if (strcmp(command->args.values[0].values, "exit") == 0) {
		return CR_EXIT;
	}

	return CR_NOTFOUND;
}

bool command_execute_system(Command *command) {
	char **raw_args = strings_to_raw(&command->args);

	pid_t pid = fork();

	if (pid == -1) {
		printw("ysh: could not fork\n");
		refresh();
	} else if (pid == 0) {
		if (execvp(raw_args[0], raw_args) < 0) {
			if (errno == ENOENT || errno == ENOTDIR) {
				return CR_NOTFOUND;
			} else {
				printw("ysh: could not execute command: %m\n");
				refresh();
			}
		}
	} else {
		wait(NULL);

		addch('\n');
		refresh();
	}

	free(raw_args);

	return CR_OK;
}

CommandResult command_execute(Command *command) {
	CommandResult result = command_execute_builtin(command);
	if (result == CR_NOTFOUND)
		result = command_execute_system(command);

	return result;
}