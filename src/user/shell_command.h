#ifndef USER_HANDLER_H_
#define USER_HANDLER_H_

#define SHELL_COMMAND_NAME_MAX_LEN 0x20
typedef struct _SHELL_COMMAND_DESCRIPTOR {
	const char *name;
	int (*exec)(int argsc, char **argsv);
	const char * description;
	const char **help_message;
} SHELL_COMMAND_DESCRIPTOR;


#define DECLARE_SHELL_COMMAND_DESCRIPTOR(name, exec, desc_msg, help_msg) \
    static int exec(int argsc, char **argsv); \
    static const SHELL_COMMAND_DESCRIPTOR _descriptor __attribute__ ((section(".shell_commands"))) = { name, exec, desc_msg, &help_msg }; \


int shell_command_exec(SHELL_COMMAND_DESCRIPTOR *descriptor, int argsc, char **argsv);

SHELL_COMMAND_DESCRIPTOR *shell_command_find_descriptor(char *name);

/**iterate functions*/
SHELL_COMMAND_DESCRIPTOR *shell_command_descriptor_find_first(char *start_str, int length);

SHELL_COMMAND_DESCRIPTOR *shell_command_descriptor_find_next(SHELL_COMMAND_DESCRIPTOR * cur_desc, char *start_str, int length);

#endif /*USER_HANDLER_H_*/
