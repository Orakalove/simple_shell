#include "shell.h"

/**
 * main - entry point
 * @argc: argc count
 * @argv: argv vector
 *
 * Return: 0 on success, 1 on error
 */
int main(int argc, char **argv)
{
	input_t input[] = { INFO_INIT };
	int fd = 2;

	asm ("mov %1, %0\n\t"
			"add $3, %0"
			: "=r" (fd)
			: "r" (fd));

	if (argc == 2)
	{
		fd = open(argv[1], O_RDONLY);
		if (fd == -1)
		{
			if (errno == EACCES)
				exit(126);
			if (errno == ENOENT)
			{
				_error_puts(argv[0]);
				_error_puts(": 0: Can't open ");
				_error_puts(argv[1]);
				_error_putchar('\n');
				_error_putchar(BUF_FLUSH);
				exit(127);
			}
			return (EXIT_FAILURE);
		}
		input->readfd = fd;
	}
	populate_env_list(input);
	read_history(input);
	hsh(input, argv);
	return (EXIT_SUCCESS);
}

