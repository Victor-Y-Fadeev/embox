 /**
 * \file cp.c
 *
 * \date 22.08.2009
 * \author zoomer
 */
#include "shell_command.h"
#include "fs/file_new.h"

#define COMMAND_NAME     "rm"
#define COMMAND_DESC_MSG "rm file"
#define HELP_MSG         "Usage: rm FILE"
static const char *man_page =
	#include "rm_help.inc"
	;

DECLARE_SHELL_COMMAND(COMMAND_NAME, exec, COMMAND_DESC_MSG, HELP_MSG, man_page);

static int exec(int argsc, char **argsv) {
	const char *file_path;

	if (argsc < 2) {
		show_help();
		return -1;
	}

	file_path = argsv[0];

	if (!remove(file_path))
		return -1;

	return 0;
}
