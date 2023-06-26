#include "shell.h"

/**
 * hsh - main shell loop
 * @insert: the parameter & return info struct
 * @arv: the argument vector from main()
 *
 * Return: 0 on success, 1 on error, or error code
 */
int hsh(input_t *insert, char **arv)
{
	ssize_t r = 0;
	int builtin_ret = 0;

	while (r != -1 && builtin_ret != -2)
	{
		clear_input(insert);
		if (interactive(insert))
			_puts("simple_shell$ ");
		_error_putchar(BUF_FLUSH);
		r = get_input(insert);
		if (r != -1)
		{
			set_input(insert, arv);
			builtin_ret = find_builtin(insert);
			if (builtin_ret == -1)
				find_cmd(insert);
		}
		else if (interactive(insert))
			_putchar('\n');
		free_input(insert, 0);
	}
	write_history(insert);
	free_input(insert, 1);
	if (!interactive(insert) && insert->status)
		exit(insert->status);
	if (builtin_ret == -2)
	{
		if (insert->err_num == -1)
			exit(insert->status);
		exit(insert->err_num);
	}
	return (builtin_ret);
}

/**
 * find_builtin - finds a builtin command
 * @insert: the parameter & return info struct
 *
 * Return: -1 if builtin not found,
 * 0 if builtin executed successfully,
 * 1 if builtin found but not successful,
 * 2 if builtin signals exit()
 */
int find_builtin(input_t *insert)
{
	int i, built_in_ret = -1;
	builtin_T builtintbl[] = {
		{"exit", shell_exit},
		{"env", shell_env},
		{"help", shell_help},
		{"history", shell_history},
		{"setenv", shell_setenv},
		{"unsetenv", shell_unsetenv},
		{"cd", shell_cd},
		{"alias", shell_alias},
		{NULL, NULL}
	};

	for (i = 0; builtintbl[i].value; i++)
		if (_strcmp(insert->argv[0], builtintbl[i].value) == 0)
		{
			insert->line_count++;
			built_in_ret = builtintbl[i].func(insert);
			break;
		}
	return (built_in_ret);
}

/**
 * find_cmd - finds a command in PATH
 * @insert: the parameter & return info struct
 *
 * Return: void
 */
void find_cmd(input_t *insert)
{
	char *path_name = NULL;
	int i, k;

	insert->path_name = insert->argv[0];
	if (insert->linecount_flag == 1)
	{
		insert->line_count++;
		insert->linecount_flag = 0;
	}
	for (i = 0, k = 0; insert->arg[i]; i++)
		if (!is_delim(insert->arg[i], " \t\n"))
			k++;
	if (!k)
		return;

	path_name = find_path(insert, _getenv(insert, "PATH="), insert->argv[0]);
	if (path_name)
	{
		insert->path_name = path_name;
		fork_cmd(insert);
	}
	else
	{
		if ((interactive(insert) || _getenv(insert, "PATH=")
					|| insert->argv[0][0] == '/') && is_cmd(insert, insert->argv[0]))
			fork_cmd(insert);
		else if (*(insert->arg) != '\n')
		{
			insert->status = 127;
			create_error(insert, "not found\n");
		}
	}
}

/**
 * fork_cmd - forks a an exec thread to run cmd
 * @insert: the parameter & return info struct
 *
 * Return: void
 */
void fork_cmd(input_t *insert)
{
	pid_t child_pid;

	child_pid = fork();
	if (child_pid == -1)
	{
		/* TODO: PUT ERROR FUNCTION */
		perror("Error:");
		return;
	}
	if (child_pid == 0)
	{
		if (execve(insert->path_name, insert->argv, get_location(insert)) == -1)
		{
			free_input(insert, 1);
			if (errno == EACCES)
				exit(126);
			exit(1);
		}
		/* TODO: PUT ERROR FUNCTION */
	}
	else
	{
		wait(&(insert->status));
		if (WIFEXITED(insert->status))
		{
			insert->status = WEXITSTATUS(insert->status);
			if (insert->status == 126)
				create_error(insert, "Permission denied\n");
		}
	}
}

