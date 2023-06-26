#include "shell.h"

/**
 * add_node - adds a node to the start of the list
 * @head: address of pointer to head node
 * @s: str field of node
 * @n: node index used by history
 *
 * Return: size of list
 */
list_t *add_node(list_t **head, const char *s, int n)
{
	list_t *n_head;

	if (!head)
		return (NULL);
	n_head = malloc(sizeof(list_t));
	if (!n_head)
		return (NULL);
	_memset((void *)n_head, 0, sizeof(list_t));
	n_head->num = n;
	if (s)
	{
		n_head->s = _strdup(s);
		if (!n_head->s)
		{
			free(n_head);
			return (NULL);
		}
	}
	n_head->next = *head;
	*head = n_head;
	return (n_head);
}

/**
 * add_node_end - adds a node to the end of the list
 * @head: address of pointer to head node
 * @s: str field of node
 * @n: node index used by history
 *
 * Return: size of list
 */
list_t *add_node_end(list_t **head, const char *s, int n)
{
	list_t *n_node, *node;

	if (!head)
		return (NULL);

	node = *head;
	n_node = malloc(sizeof(list_t));
	if (!n_node)
		return (NULL);
	_memset((void *)n_node, 0, sizeof(list_t));
	n_node->num = n;
	if (s)
	{
		n_node->s = _strdup(s);
		if (!n_node->s)
		{
			free(n_node);
			return (NULL);
		}
	}
	if (node)
	{
		while (node->next)
			node = node->next;
		node->next = n_node;
	}
	else
		*head = n_node;
	return (n_node);
}

/**
 * print_list_str - prints only the str element of a list_t linked list
 * @first: pointer to first node
 *
 * Return: size of list
 */
size_t print_list_str(const list_t *first)
{
	size_t i = 0;

	while (first)
	{
		_puts(first->s ? first->s : "(nil)");
		_puts("\n");
		first = first->next;
		i++;
	}
	return (i);
}

/**
 * delete_node_at_index - deletes node at given index
 * @head: address of pointer to first node
 * @index: index of node to delete
 *
 * Return: 1 on success, 0 on failure
 */
int delete_node_at_index(list_t **head, unsigned int index)
{
	list_t *node, *tmp_node;
	unsigned int i = 0;

	if (!head || !*head)
		return (0);

	if (!index)
	{
		node = *head;
		*head = (*head)->next;
		free(node->s);
		free(node);
		return (1);
	}
	node = *head;
	while (node)
	{
		if (i == index)
		{
			tmp_node->next = node->next;
			free(node->s);
			free(node);
			return (1);
		}
		i++;
		tmp_node = node;
		node = node->next;
	}
	return (0);
}

/**
 * free_list - frees all nodes of a list
 * @head_pptr: address of pointer to head node
 *
 * Return: void
 */
void free_list(list_t **head_pptr)
{
	list_t *node, *next_node, *head;

	if (!head_pptr || !*head_pptr)
		return;
	head = *head_pptr;
	node = head;
	while (node)
	{
		next_node = node->next;
		free(node->s);
		free(node);
		node = next_node;
	}
	*head_pptr = NULL;
}

