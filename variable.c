#include "shell.h"

/**
 * is_chain - test if current char in buffer is a chain delimeter
 * @insert: the parameter struct
 * @store: the char buffer
 * @ptr: address of current position in buf
 *
 * Return: 1 if chain delimeter, 0 otherwise
 */
int is_chain(input_t *insert, char *store, size_t *ptr)
{
	size_t j = *ptr;

	if (store[j] == '|' && store[j + 1] == '|')
	{
		store[j] = 0;
		j++;
		insert->cmd_buf_type = CMD_OR;
	}
	else if (store[j] == '&' && store[j + 1] == '&')
	{
		store[j] = 0;
		j++;
		insert->cmd_buf_type = CMD_AND;
	}
	else if (store[j] == ';') /* found end of this command */
	{
		store[j] = 0; /* replace semicolon with null */
		insert->cmd_buf_type = CMD_CHAIN;
	}
	else
		return (0);
	*ptr = j;
	return (1);
}

/**
 * check_chain - checks we should continue chaining based on last status
 * @insert: the parameter struct
 * @store: the char buffer
 * @ptr: address of current position in buf
 * @i: starting position in buf
 * @len: length of buf
 *
 * Return: Void
 */
void check_chain(input_t *insert, char *store, size_t *ptr, size_t i,
		size_t len)
{
	size_t j = *ptr;

	if (insert->cmd_buf_type == CMD_AND)
	{
		if (insert->status)
		{
			store[i] = 0;
			j = len;
		}
	}
	if (insert->cmd_buf_type == CMD_OR)
	{
		if (!insert->status)
		{
			store[i] = 0;
			j = len;
		}
	}

	*ptr = j;
}

/**
 * replace_alias - replaces an aliases in the tokenized string
 * @insert: the parameter struct
 *
 * Return: 1 if replaced, 0 otherwise
 */
int replace_alias(input_t *insert)
{
	int i;
	list_t *node;
	char *ptr;

	for (i = 0; i < 10; i++)
	{
		node = node_starts_with(insert->alias, insert->argv[0], '=');
		if (!node)
			return (0);
		free(insert->argv[0]);
		ptr = _strchr(node->s, '=');
		if (!ptr)
			return (0);
		ptr = _strdup(ptr + 1);
		if (!ptr)
			return (0);
		insert->argv[0] = ptr;
	}
	return (1);
}

/**
 * replace_vars - replaces vars in the tokenized string
 * @insert: the parameter struct
 *
 * Return: 1 if replaced, 0 otherwise
 */
int replace_vars(input_t *insert)
{
	int i = 0;
	list_t *node;

	for (i = 0; insert->argv[i]; i++)
	{
		if (insert->argv[i][0] != '$' || !insert->argv[i][1])
			continue;

		if (!_strcmp(insert->argv[i], "$?"))
		{
			replace_string(&(insert->argv[i]),
					_strdup(convert_number(insert->status, 10, 0)));
			continue;
		}
		if (!_strcmp(insert->argv[i], "$$"))
		{
			replace_string(&(insert->argv[i]),
					_strdup(convert_number(getpid(), 10, 0)));
			continue;
		}
		node = node_starts_with(insert->env, &insert->argv[i][1], '=');
		if (node)
		{
			replace_string(&(insert->argv[i]),
					_strdup(_strchr(node->s, '=') + 1));
			continue;
		}
		replace_string(&insert->argv[i], _strdup(""));

	}
	return (0);
}

/**
 * replace_string - replaces string
 * @old: address of old string
 * @new: new string
 *
 * Return: 1 if replaced, 0 otherwise
 */
int replace_string(char **old, char *new)
{
	free(*old);
	*old = new;
	return (1);
}

