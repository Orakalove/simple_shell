#include "shell.h"

/**
 * input_buf - buffers chained commands
 * @input: parameter struct
 * @store: address of buffer
 * @len: address of len var
 *
 * Return: bytes read
 */
ssize_t input_buf(input_t *insert, char **store, size_t *len)
{
	ssize_t r = 0;
	size_t len_p = 0;

	if (!*len) /* if nothing left in the buffer, fill it */
	{
		/*bfree((void **)input->cmd_buf);*/
		free(*store);
		*store = NULL;
		signal(SIGINT, sigintHandler);
#if USE_GETLINE
		r = getline(store, &len_p, stdin);
#else
		r = _getline(insert, store, &len_p);
#endif
		if (r > 0)
		{
			if ((*store)[r - 1] == '\n')
			{
				(*store)[r - 1] = '\0'; /* remove trailing newline */
				r--;
			}
			insert->linecount_flag = 1;
			rm_comments(*store);
			history_list(insert, *store, insert->histcount++);
			/* if (_strchr(*buf, ';')) is this a command chain? */
			{
				*len = r;
				insert->cmd_buf = store;
			}
		}
	}
	return (r);
}

/**
 * get_input - gets a line minus the newline
 * @insert: parameter struct
 *
 * Return: bytes read
 */
ssize_t get_input(input_t *insert)
{
	static char *store; /* the ';' command chain buffer */
	static size_t i, j, len;
	ssize_t r = 0;
	char **store_ptr = &(insert->arg), *ptr;

	_putchar(BUF_FLUSH);
	r = input_buf(insert, &store, &len);
	if (r == -1) /* EOF */
		return (-1);
	if (len) /* we have commands left in the chain buffer */
	{
		j = i; /* init new iterator to current buf position */
		ptr = store + i; /* get pointer for return */

		check_chain(insert, store, &j, i, len);
		while (j < len) /* iterate to semicolon or end */
		{
			if (is_chain(insert, store, &j))
				break;
			j++;
		}

		i = j + 1; /* increment past nulled ';'' */
		if (i >= len) /* reached end of buffer? */
		{
			i = len = 0; /* reset position and length */
			insert->cmd_buf_type = CMD_NORM;
		}

		*store_ptr = ptr; /* pass back pointer to current command position */
		return (_strlen(ptr)); /* return length of current command */
	}

	*store_ptr = store; /* else not a chain, pass back buffer from _getline() */
	return (r); /* return length of buffer from _getline() */
}

/**
 * read_buf - reads a buffer
 * @insert: parameter struct
 * @buf: buffer
 * @size: size
 *
 * Return: r
 */
ssize_t read_buf(input_t *insert, char *buf, size_t *size)
{
	ssize_t r = 0;

	if (*size)
		return (0);
	r = read(insert->readfd, buf, READ_BUF_SIZE);
	if (r >= 0)
		*size = r;
	return (r);
}

/**
 * _getline - gets the next line of input from STDIN
 * @insert: parameter struct
 * @ptr: address of pointer to buffer, preallocated or NULL
 * @length: size of preallocated ptr buffer if not NULL
 *
 * Return: s
 */
int _getline(input_t *insert, char **ptr, size_t *length)
{
	static char buf[READ_BUF_SIZE];
	static size_t i, len;
	size_t k;
	ssize_t r = 0, s = 0;
	char *p = NULL, *new_p = NULL, *c;

	p = *ptr;
	if (p && length)
		s = *length;
	if (i == len)
		i = len = 0;

	r = read_buf(insert, buf, &len);
	if (r == -1 || (r == 0 && len == 0))
		return (-1);

	c = _strchr(buf + i, '\n');
	k = c ? 1 + (unsigned int)(c - buf) : len;
	new_p = _realloc(p, s, s ? s + k : k + 1);
	if (!new_p) /* MALLOC FAILURE! */
		return (p ? free(p), -1 : -1);

	if (s)
		_strncat(new_p, buf + i, k - i);
	else
		_strncpy(new_p, buf + i, k - i + 1);

	s += k - i;
	i = k;
	p = new_p;

	if (length)
		*length = s;
	*ptr = p;
	return (s);
}

/**
 * sigintHandler - blocks ctrl-C
 * @sig_num: the signal number
 *
 * Return: void
 */
void sigintHandler(__attribute__((unused))int sig_num)
{
	_puts("\n");
	_puts("simple_shell$ ");
	_putchar(BUF_FLUSH);
}

