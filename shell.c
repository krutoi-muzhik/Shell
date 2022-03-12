#include "shell.h"

int
main (void) {
	char buffer[MAX_STRING_SIZE];
	cmd_t *command = (cmd_t *) calloc (MAX_CMD_NUM, sizeof (cmd_t));
	for (size_t i = 0; i < MAX_CMD_NUM; i++) {
		command[i].arg = (char **) calloc (MAX_ARG_NUM, sizeof (char *));
	}

	int n_cmd = InputSort (buffer, command);

	int pid;
	int pipefd[2];
	int tmp = 0;
	if (!n_cmd) {
		execvp (command[0].cmd, command[0].arg);
		exit (EXIT_SUCCESS);
	}

	for (size_t i = 0; i < n_cmd; i++) {
		if (pipe (pipefd) == -1) {
			perror ("pipe");
			exit (EXIT_FAILURE);
		}

		if ((pid = fork ()) == -1) {
			perror ("fork");
			exit (EXIT_FAILURE);
		}

		if (pid) {
			close (pipefd[1]);
			if (tmp != -1)
				close (tmp);
			tmp = pipefd[0];

			close (tmp);
			wait (NULL);
			_exit (EXIT_SUCCESS);

		} else {
			if (tmp != -1) {
				if (dup2 (tmp, infd) == -1) {
					perror ("dup2");
					exit (EXIT_FAILURE);
				}
			}
			if (i < n_cmd - 1) {
				if (dup2 (pipefd[1], outfd) == -1) {
					perror ("dup2");
					exit (EXIT_FAILURE);
				}
			}
			close (pipefd[0]);
			close (pipefd[1]);
			if (tmp != -1)
				close (tmp);

			execvp (command[i].cmd, command[i].arg);
			exit (EXIT_SUCCESS);
		}
	}
}

int
InputSort (char *input_string, cmd_t *command) {
	size_t n_bytes = read (infd, input_string, MAX_STRING_SIZE);
	if (n_bytes == -1) {
		perror ("read");
		exit (EXIT_FAILURE);
	}

	size_t i = 0, n_cmd = 0, cmd_flag = 1, arg_flag = 0;

	input_string[n_bytes - 1] = '\0';
	while (i < n_bytes - 1) {
		if (input_string[i] == ' ') {
			input_string[i] = '\0';
			i++;
			continue;
		}
		if (input_string[i] == '|') {
			cmd_flag = 1;
			arg_flag = 0;
			n_cmd++;
			i++;
			continue;
		}
		if (cmd_flag) {
			command[n_cmd].cmd = &input_string[i];
			command[n_cmd].arg[command[n_cmd].n_arg] = &input_string[i];
			command[n_cmd].n_arg = 1;
			while (!isspace (input_string[i]) && (input_string[i] != '\0'))
				i++;
			cmd_flag = 0;
			arg_flag = 1;
			continue;
		}
		if (arg_flag) {
			command[n_cmd].arg[command[n_cmd].n_arg] = &input_string[i];
			command[n_cmd].n_arg++;
			while (!isspace (input_string[i]) && (input_string[i] != '\0'))
				i++;
			continue;
		}
	}

	return n_cmd;
}