#pragma once

#include <stddef.h>

#include "string.h"

typedef struct {
	Strings args;
} Command;

typedef enum { CR_OK, CR_NOTFOUND, CR_EXIT } CommandResult;

Command command_parse(String *input);
void command_free(Command *command);
CommandResult command_execute(Command *command);
