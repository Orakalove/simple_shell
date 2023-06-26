#include "shell.h"

/**
 * _erratoi - converts a string to an integer
 * @str: the string to be converted
 * Return: 0 if no numbers in string, converted number otherwise
 *       -1 on error
 */
int _erratoi(char *str)
{
	int i = 0;
	unsigned long int result = 0;

	if (*str == '+')
		str++;  /* TODO: why does this make main return 255? */
	for (i = 0;  str[i] != '\0'; i++)
	{
		if (str[i] >= '0' && str[i] <= '9')
		{
			result *= 10;
			result += (str[i] - '0');
			if (result > INT_MAX)
				return (-1);
		}
		else
			return (-1);
	}
	return (result);
}

/**
 * create_error - prints an error message
 * @insert: the parameter & return info struct
 * @str: string containing specified error type
 * Return: 0 if no numbers in string, converted number otherwise
 *        -1 on error
 */
void create_error(input_t *insert, char *str)
{
	_error_puts(insert->fname);
	_error_puts(": ");
	print_d(insert->line_count, STDERR_FILENO);
	_error_puts(": ");
	_error_puts(insert->argv[0]);
	_error_puts(": ");
	_error_puts(str);
}

/**
 * print_d - function prints a decimal (integer) number (base 10)
 * @insert: the input
 * @fd: the filedescriptor to write to
 *
 * Return: number of characters printed
 */
int print_d(int insert, int fd)
{
	int (*__putchar)(char) = _putchar;
	int i, count = 0;
	unsigned int _abs_, current;

	if (fd == STDERR_FILENO)
		__putchar = _error_putchar;
	if (insert < 0)
	{
		_abs_ = -insert;
		__putchar('-');
		count++;
	}
	else
		_abs_ = insert;
	current = _abs_;
	for (i = 1000000000; i > 1; i /= 10)
	{
		if (_abs_ / i)
		{
			__putchar('0' + current / i);
			count++;
		}
		current %= i;
	}
	__putchar('0' + current);
	count++;

	return (count);
}

/**
 * convert_number - converter function, a clone of itoa
 * @num: number
 * @base: base
 * @flags: argument flags
 *
 * Return: string
 */
char *convert_number(long int num, int base, int flags)
{
	static char *array_n;
	static char buffer[50];
	char sign = 0;
	char *p;
	unsigned long n = num;

	if (!(flags & CONVERT_UNSIGNED) && num < 0)
	{
		n = -num;
		sign = '-';

	}
	array_n = flags & CONVERT_LOWERCASE ? "0123456789abcdef" : "0123456789ABCDEF";
	p = &buffer[49];
	*p = '\0';

	do	{
		*--p = array_n[n % base];
		n /= base;
	} while (n != 0);

	if (sign)
		*--p = sign;
	return (p);
}

/**
 * rm_comments - function replaces first instance of '#' with '\0'
 * @store: address of the string to modify
 *
 * Return: Always 0;
 */
void rm_comments(char *store)
{
	int i;

	for (i = 0; store[i] != '\0'; i++)
		if (store[i] == '#' && (!i || store[i - 1] == ' '))
		{
			store[i] = '\0';
			break;
		}
}

