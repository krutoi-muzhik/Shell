#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdarg.h>

enum IOFD {
	infd = STDIN_FILENO,
	outfd = STDOUT_FILENO
};

enum SIZES {
	MAX_STRING_SIZE = 100,
	//MAX_CMD_SIZE = 10,
	MAX_CMD_NUM = 5,
	//MAX_ARG_SIZE = 10,
	MAX_ARG_NUM = 5
};

typedef struct comand {
	char *cmd;
	char **arg;
	size_t n_arg;
} cmd_t;

int InputSort (char *input_string, cmd_t *command);

