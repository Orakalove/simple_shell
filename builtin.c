#include "shell.h"


/**
 * shell_exit - process termination for shell
 * @input: Structure containing an array of arguments. Used to maintain
 * constant function prototype.
 * Return: exits with a given exit status
 * (0) if input.argv[0] != "exit"
 */
int shell_exit(input_t *input)
{
	int exitcheck;

	/* if there is argument */
	if (input->argv[1])
	{
		exitcheck = _erratoi(input->argv[1]);
		if (exitcheck == -1)
		{
			input->status = 2;
			create_error(input, "illegal number: ");
			_error_puts(input->argv[1]);
			_error_putchar('\n');
			return (1);
		}
		input->err_num = _erratoi(input->argv[1]);
		return (-2);
	}
	input->err_num = -1;
	return (-2);
}

/**
 * shell_cd - change the current directory of the process
 * @input: structure containing variable argument use for
 * func prototype.
 * Return: Always 0 on succes.
 */
int shell_cd(input_t *input)
{
	char *str, *dir, buffer[1024];
	int chdir_ret;

	str = getcwd(buffer, 1024);
	if (!str)
		_puts("TODO: >>getcwd failure emsg here<<\n");
	if (!input->argv[1])
	{
		dir = _getenv(input, "HOME=");
		if (!dir)
			chdir_ret = /* TODO: what should this be? */
				chdir((dir = _getenv(input, "PWD=")) ? dir : "/");
		else
			chdir_ret = chdir(dir);
	}
	else if (_strcmp(input->argv[1], "-") == 0)
	{
		if (!_getenv(input, "OLDPWD="))
		{
			_puts(str);
			_putchar('\n');
			return (1);
		}
		_puts(_getenv(input, "OLDPWD=")), _putchar('\n');
		chdir_ret = /* TODO: what should this be? */
			chdir((dir = _getenv(input, "OLDPWD=")) ? dir : "/");
	}
	else
		chdir_ret = chdir(input->argv[1]);
	if (chdir_ret == -1)
	{
		create_error(input, "can't cd to ");
		_error_puts(input->argv[1]), _error_putchar('\n');
	}
	else
	{
		_setenv(input, "OLDPWD", _getenv(input, "PWD="));
		_setenv(input, "PWD", getcwd(buffer, 1024));
	}
	return (0);
}

/**
 * shell_help - Display information about shellby builtin command
 * @insert: Structure containing array arguments. Used to maintain
 * constant function prototype.
 * Return: Always 0
 */
int shell_help(input_t *insert)
{
	char **argv_array;

	argv_array = insert->argv;
	_puts("help call works. Function not yet implemented \n");
	if (0)
		_puts(*argv_array); /* temp att_unused workaround */
	return (0);
}

