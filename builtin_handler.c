#include "shell.h"

/**
 * shell_history - displays the history list, one command by line, preceded
 *              with line numbers, starting at 0.
 * @insert: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 *  Return: Always 0
 */
int shell_history(input_t *insert)
{
	print_list(insert->hist);
	return (0);
}

/**
 * unset_alias - sets alias to string
 * @insert: parameter struct
 * @s: the string alias
 *
 * Return: Always 0 on success, 1 on error
 */
int unset_alias(input_t *insert, char *s)
{
	char *ptr, c;
	int ret;

	ptr = _strchr(s, '=');
	if (!ptr)
		return (1);
	c = *ptr;
	*ptr = 0;
	ret = delete_node_at_index(&(insert->alias),
		get_node_index(insert->alias, node_starts_with(insert->alias, s, -1)));
	*ptr = c;
	return (ret);
}
/**
 * set_alias - sets alias to string
 * @insert: parameter struct
 * @str: the string alias
 *
 * Return: Always 0 on success, 1 on error
 */
int set_alias(input_t *insert, char *str)
{
	char *ptr;

	ptr = _strchr(str, '=');
	if (!ptr)
		return (1);
	if (!*++ptr)
		return (unset_alias(insert, str));

	unset_alias(insert, str);
	return (add_node_end(&(insert->alias), str, 0) == NULL);
}

/**
 * print_alias - prints an alias string
 * @node: the alias node
 *
 * Return: Always 0 on success, 1 on error
 */
int print_alias(list_t *node)
{
	char *ptr = NULL, *ar = NULL;

	if (node)
	{
		ptr = _strchr(node->s, '=');
		for (ar = node->s; ar <= ptr; ar++)
		_putchar(*ar);
		_putchar('\'');
		_puts(ptr + 1);
		_puts("'\n");
		return (0);
	}
	return (1);
}

/**
 * shell_alias - mimics the alias builtin (man alias)
 * @insert: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 *  Return: Always 0
 */
int shell_alias(input_t *insert)
{
	int i = 0;
	char *ptr = NULL;
	list_t *node = NULL;

	if (insert->argc == 1)
	{
		node = insert->alias;
		while (node)
		{
			print_alias(node);
			node = node->next;
		}
		return (0);
	}
	for (i = 1; insert->argv[i]; i++)
	{
		ptr = _strchr(insert->argv[i], '=');
		if (ptr)
			set_alias(insert, insert->argv[i]);
		else
			print_alias(node_starts_with(insert->alias, insert->argv[i], '='));
	}

	return (0);
}

