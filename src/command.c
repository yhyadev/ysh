#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
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
	if (string_equal_cstr(&command->args.values[0], "cd")) {
		if (command->args.len > 1) {
			chdir(command->args.values[1].values);
		}

		return CR_OK;
	} else if (string_equal_cstr(&command->args.values[0], "exit")) {
		return CR_EXIT;
	}

	return CR_NOTFOUND;
}

bool command_execute_system(Command *command) {
	char **raw_args = strings_to_raw(&command->args);

	pid_t pid = fork();

	if (pid == -1) {
		fprintf(stderr, "ysh: could not fork\n");
	} else if (pid == 0) {
		if (execvp(raw_args[0], raw_args) < 0) {
			if (errno == ENOENT || errno == ENOTDIR) {
				return CR_NOTFOUND;
			} else {
				fprintf(stderr, "ysh: could not execute command: %m\n");
			}
		}
	} else {
		wait(NULL);
	}

	free(raw_args);

	return CR_OK;
}

CommandResult command_execute_teleport(Command *command) {
	chdir(command->args.values[0].values);

	return CR_OK;
}

CommandResult command_execute(Command *command) {
	CommandResult result = command_execute_builtin(command);

	if (result == CR_NOTFOUND && (string_is_dir(&command->args.values[0]) &&
								  string_is_path(&command->args.values[0])))
		result = command_execute_teleport(command);

	if (result == CR_NOTFOUND)
		result = command_execute_system(command);

	return result;
}
