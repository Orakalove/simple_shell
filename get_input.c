#include "shell.h"

/**
 * clear_input - initializes input_t struct
 * @insert: struct address
 */
void clear_input(input_t *insert)
{
	insert->arg = NULL;
	insert->argv = NULL;
	insert->path_name = NULL;
	insert->argc = 0;
}

/**
 * set_input - initializes input_t struct
 * @insert: struct address
 * @arv: argument vector
 */
void set_input(input_t *insert, char **arv)
{
	int i = 0;

	insert->fname = arv[0];
	if (insert->arg)
	{
		insert->argv = strtow(insert->arg, " \t");
		if (!insert->argv)
		{
			insert->argv = malloc(sizeof(char *) * 2);
			if (insert->argv)
			{
				insert->argv[0] = _strdup(insert->arg);
				insert->argv[1] = NULL;
			}
		}
		for (i = 0; insert->argv && insert->argv[i]; i++)
			;
		insert->argc = i;

		replace_alias(insert);
		replace_vars(insert);
	}
}

/**
 * free_input - frees input_t struct fields
 * @insert: struct address
 * @all: true if freeing all fields
 */
void free_input(input_t *insert, int all)
{
	ffree(insert->argv);
	insert->argv = NULL;
	insert->path_name = NULL;
	if (all)
	{
		if (!insert->cmd_buf)
			free(insert->arg);
		if (insert->env)
			free_list(&(insert->env));
		if (insert->hist)
			free_list(&(insert->hist));
		if (insert->alias)
			free_list(&(insert->alias));
		ffree(insert->environ);
			insert->environ = NULL;
		b_free((void **)insert->cmd_buf);
		if (insert->readfd > 2)
			close(insert->readfd);
		_putchar(BUF_FLUSH);
	}
}

