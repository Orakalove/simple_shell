#include "shell.h"

/**
 * list_len - determines length of linked list
 * @first: pointer to first node
 *
 * Return: size of list
 */
size_t list_len(const list_t *first)
{
	size_t i = 0;

	while (first)
	{
		first = first->next;
		i++;
	}
	return (i);
}

/**
 * list_to_strings - returns an array of strings of the list->str
 * @head_ptr: pointer to first node
 *
 * Return: array of strings
 */
char **list_to_strings(list_t *head_ptr)
{
	list_t *node = head_ptr;
	size_t i = list_len(head_ptr), j;
	char **str_ptrs;
	char *str;

	if (!head_ptr || !i)
		return (NULL);
	str_ptrs = malloc(sizeof(char *) * (i + 1));
	if (!str_ptrs)
		return (NULL);
	for (i = 0; node; node = node->next, i++)
	{
		str = malloc(_strlen(node->s) + 1);
		if (!str)
		{
			for (j = 0; j < i; j++)
				free(str_ptrs[j]);
			free(str_ptrs);
			return (NULL);
		}

		str = _strcpy(str, node->s);
		str_ptrs[i] = str;
	}
	str_ptrs[i] = NULL;
	return (str_ptrs);
}


/**
 * print_list - prints all elements of a list_t linked list
 * @first: pointer to first node
 *
 * Return: size of list
 */
size_t print_list(const list_t *first)
{
	size_t i = 0;

	while (first)
	{
		_puts(convert_number(first->num, 10, 0));
		_putchar(':');
		_putchar(' ');
		_puts(first->s ? first->s : "(nil)");
		_puts("\n");
		first = first->next;
		i++;
	}
	return (i);
}

/**
 * node_starts_with - returns node whose string starts with prefix
 * @node_ptr: pointer to list head
 * @prefix: string to match
 * @c: the next character after prefix to match
 *
 * Return: match node or null
 */
list_t *node_starts_with(list_t *node_ptr, char *prefix, char c)
{
	char *ptr = NULL;

	while (node_ptr)
	{
		ptr = starts_with(node_ptr->s, prefix);
		if (ptr && ((c == -1) || (*ptr == c)))
			return (node_ptr);
		node_ptr = node_ptr->next;
	}
	return (NULL);
}

/**
 * get_node_index - gets the index of a node
 * @head_ptr: pointer to list head
 * @node_ptr: pointer to the node
 *
 * Return: index of node or -1
 */
ssize_t get_node_index(list_t *head_ptr, list_t *node_ptr)
{
	size_t i = 0;

	while (head_ptr)
	{
		if (head_ptr == node_ptr)
			return (i);
		head_ptr = head_ptr->next;
		i++;
	}
	return (-1);
}

