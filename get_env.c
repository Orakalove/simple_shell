#include "shell.h"

/**
 * get_location - returns the string array copy of our environ
 * @insert: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 * Return: Always 0
 */
char **get_location(input_t *insert)
{
	if (!insert->environ || insert->env_changed)
	{
		insert->environ = list_to_strings(insert->env);
		insert->env_changed = 0;
	}

	return (insert->environ);
}

/**
 * _unsetenv - Remove an environment variable
 * @insert: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 *  Return: 1 on delete, 0 otherwise
 * @var: the string env var property
 */
int _unsetenv(input_t *insert, char *var)
{
	list_t *node = insert->env;
	size_t i = 0;
	char *ptr;

	if (!node || !var)
		return (0);

	while (node)
	{
		ptr = starts_with(node->s, var);
		if (ptr && *ptr == '=')
		{
			insert->env_changed = delete_node_at_index(&(insert->env), i);
			i = 0;
			node = insert->env;
			continue;
		}
		node = node->next;
		i++;
	}
	return (insert->env_changed);
}

/**
 * _setenv - Initialize a new environment variable,
 *             or modify an existing one
 * @insert: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 * @var: the string env var property
 * @num: the string env var value
 *  Return: Always 0
 */
int _setenv(input_t *insert, char *var, char *num)
{
	char *store = NULL;
	list_t *node;
	char *ptr;

	if (!var || !num)
		return (0);

	store = malloc(_strlen(var) + _strlen(num) + 2);
	if (!store)
		return (1);
	_strcpy(store, var);
	_strcat(store, "=");
	_strcat(store, num);
	node = insert->env;
	while (node)
	{
		ptr = starts_with(node->s, var);
		if (ptr && *ptr == '=')
		{
			free(node->s);
			node->s = store;
			insert->env_changed = 1;
			return (0);
		}
		node = node->next;
	}
	add_node_end(&(insert->env), store, 0);
	free(store);
	insert->env_changed = 1;
	return (0);
}

